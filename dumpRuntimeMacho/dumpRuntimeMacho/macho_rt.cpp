//
//  macho_rt.cpp
//  dumpRuntimeMacho
//
//  Created by jmpews on 09/03/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#include "macho_rt.hpp"
#include "common.hpp"

namespace machort {
    MachOFile::MachOFile()
    : m_pid(-1)
    , m_task(-1)
    , m_isDyldLinker(false)
    , m_map_end(MACHO_LOAD_ADDRESS)
    , m_load_addr(MACHO_LOAD_ADDRESS)
    , m_header(NULL)
    , m_header64(NULL)
    {
    }
    
    MachOFile::~MachOFile() {
        if(m_is64bit)
            free(m_header64);
    }
    bool MachOFile::checkInit() {
        if(m_pid > 0 || m_task >0 ) {
            return true;
        }
        std::cout << "macho not be init." << std::endl;
        return false;
    }
    bool MachOFile::rt_read(vm_address_t addr, void* buf, size_t len) {
        return readTaskMemory(m_task, addr, buf, len);
        
    }
    
    char* MachOFile::rt_readString(vm_address_t addr) {
        return readTaskString(m_task, addr);
    }
    
    bool MachOFile::searchBinLoadAddress() {
        //search align by memory page
        size_t search_block_size = 0x1000;
        
        vm_address_t addr  = MACHO_LOAD_ADDRESS;
        
        size_t aslr_top_limit = ((1 << 16) << 12) + 0x100000000;
        vm_address_t end = addr + aslr_top_limit;
        
        char ch;
        while(end > addr) {
            if(rt_read(addr, &ch, 1)) {
                m_load_addr = addr;
                std::cout << "[+] BinLoadAddress: 0x" << std::hex << addr << std::endl;
                return true;
            }
            
            addr += search_block_size;
        }
        return false;
    }
    
    
    void MachOFile::setPid(int pid) {
        this->m_pid = pid;
        this->m_task = pid2task(pid);
    }
    
    bool MachOFile::parse_macho() {
        if(!searchBinLoadAddress())
            return false;
        std::cout << "[*] start dump macho:" << std::endl;
        if(!parse_header())
            return false;
        parse_load_commands();
        return true;
    }
    
    bool MachOFile::parse_header() {
        if(!checkInit())
            return false;
        
        uint32_t magic;
        if(!rt_read(m_load_addr, &magic, sizeof(magic)))
            return false;
        switch (magic) {
            case MH_MAGIC_64:
                m_is64bit = true;
                m_header64 = (struct mach_header_64*)malloc(sizeof(struct mach_header_64));
                if(!rt_read(m_load_addr, m_header64, sizeof(struct mach_header_64)))
                    return false;
                
                std::cout << "[+] macho: Arch-" << "64" << std::endl;
                break;
            default:
                std::cout << "Only support x86_64." << std::endl;
                return false;
        }
        return true;
    }
    
    bool MachOFile::parse_load_commands() {
        uint32_t ncmds = 0;
        vm_address_t addr = m_load_addr;
        
        if(m_is64bit) {
            ncmds = m_header64->ncmds;
            addr += sizeof(mach_header_64);
        }
        else
            return false;
        
        for(int i = 0; i < ncmds; i++) {
            load_command_info_t lci;
            rt_read(addr, &(lci.cmd), sizeof(struct load_command));
            
            lci.vm_addr = addr;
            
            /* add to vector STL */
            m_load_command_infos.push_back(lci);
            
            /* move to next load_command */
            addr += lci.cmd.cmdsize;
            
        }
        
        /* iterate the load commands */
        std::vector<load_command_info_t>::iterator iter;
        load_command_info_t *load_cmd_info;
        for(iter = m_load_command_infos.begin(); iter != m_load_command_infos.end(); iter++) {
            load_cmd_info = &(*iter);
            switch (load_cmd_info->cmd.cmd) {
                case LC_SEGMENT_64:
                    if(!parse_LC_SEGMENT_64(load_cmd_info))
                        return false;
                    break;
                case LC_ID_DYLINKER:
                    m_isDyldLinker = true;
                    break;
                case LC_SYMTAB:
                    if(!parse_LC_SYMTAB(load_cmd_info))
                        return false;
                    break;
                case LC_LOAD_DYLINKER:
                    if(!parse_LC_LOAD_DYLINKER(load_cmd_info))
                        return false;
                    break;
                default:
                    break;
            }
            
        }
        return true;
    }
    
    bool MachOFile::parse_LC_LOAD_DYLINKER(load_command_info_t* load_cmd_info) {
        struct dylinker_command* dy_cmd;
        dy_cmd = (struct dylinker_command *)malloc(sizeof(struct dylinker_command));
        rt_read(load_cmd_info->vm_addr, dy_cmd, sizeof(struct dylinker_command));
        load_cmd_info->ptr = dy_cmd;
        m_dyld_path = rt_readString(load_cmd_info->vm_addr+dy_cmd->name.offset);
        std::cout << "[+] dyld path: " << m_dyld_path << std::endl;
        return true;
    }
    
    bool MachOFile::parse_LC_SYMTAB(load_command_info_t* load_cmd_info) {
        struct symtab_command* sym_cmd;
        
        sym_cmd = (struct symtab_command*)malloc(sizeof(struct symtab_command));
        rt_read(load_cmd_info->vm_addr, sym_cmd, sizeof(struct symtab_command));
        load_cmd_info->ptr = sym_cmd;
        
        m_symtab_addr = m_link_edit_bias + sym_cmd->symoff;
        m_strtab_addr = m_link_edit_bias + sym_cmd->stroff;
        
        std::cout << "[+] string table: 0x" << std::hex << m_symtab_addr << std::endl;
        
        std::cout << "[+] symbol table: 0x" << std::hex << m_strtab_addr << std::endl;
        
        struct nlist_64* nl;
        nl = (struct nlist_64*)malloc(sizeof(struct nlist_64));
        vm_address_t addr = m_symtab_addr;
        for(int i = 0; i < sym_cmd->nsyms; i++) {
            rt_read(addr, nl, sizeof(struct nlist_64));
            if(nl->n_un.n_strx > 1) {
                
                char *sym_name = rt_readString(m_strtab_addr+nl->n_un.n_strx);
                if(sym_name) {
                    if(!strcmp(sym_name, "_dlopen")) {
                        std::cout << "[+] found function _dlopen: 0x" << std::hex << (m_load_addr+nl->n_value) << std::endl;
                    }
                    //                if(nl->n_type == N_FUN) {
                    //                    std::cout << "[+] function: " << sym_name << ", address: 0x" << std::hex << nl->n_value << std::endl;
                    //                }
                    //                if(nl->n_type & 0x1e) {
                    //                    std::cout << "[+] extern function: " << sym_name << ", address: 0x" << std::hex << nl->n_value << std::endl;
                    //                }
                    free(sym_name);
                } else {
                    // Generate an interrupt
                    std::cout << "[*] symbol read error" << std::endl;
                }
            }
            addr += sizeof(struct nlist_64);
        }
        
        return true;
    }
    
    bool MachOFile::parse_LC_SEGMENT_64(load_command_info_t* load_cmd_info) {
        struct segment_command_64* seg_cmd_64;
        unsigned long len;
        
        /* get segment command header */
        len = sizeof(struct segment_command_64);
        seg_cmd_64 = (struct segment_command_64*)malloc(len);
        rt_read(load_cmd_info->vm_addr, seg_cmd_64, sizeof(struct segment_command_64));
        
        segment_command_64_info_t* info = new segment_command_64_info_t();
        if (info == NULL) {
            return false;
        }
        info->cmd_type = load_cmd_info->cmd.cmd;
        info->cmd = seg_cmd_64;
        
        m_segment_command_64_infos.push_back(info);
        
        if (strcmp(seg_cmd_64->segname, "__TEXT") == 0) {
            m_slide = m_load_addr - seg_cmd_64->vmaddr;
        }
        
        /*
         set link edit bias
         how to calculate it?
         ref: dyld-421.2/src/ImageLoaderMachO.cpp
         ImageLoaderMachO::parseLoadCmds
         fLinkEditBase = (uint8_t*)(segActualLoadAddress(i) - segFileOffset(i));
         */
        
        if(strcmp(seg_cmd_64->segname, "__LINKEDIT") == 0) {
            //load_command_info_t* tmp;
            //tmp = getLoadCommand(LC_SEGMENT_64, "__TEXT");
            //vm_address_t text_vm_addr = ((struct segment_command_64*)tmp)->vmaddr;
            m_link_edit_bias = seg_cmd_64->vmaddr + m_slide - seg_cmd_64->fileoff;
        }
        
        // set vm map end
        if(seg_cmd_64->vmaddr - MACHO_LOAD_ADDRESS + m_load_addr + seg_cmd_64->vmsize > m_map_end) {
            m_map_end = seg_cmd_64->vmaddr - MACHO_LOAD_ADDRESS + m_load_addr + seg_cmd_64->vmsize;
        }
        
        std::cout
        << "    +segment: " << (seg_cmd_64->segname)
        << ", cmdsize: " << (seg_cmd_64->cmdsize)
        << ", vmaddr: 0x" << std::hex << (seg_cmd_64->vmaddr - MACHO_LOAD_ADDRESS + m_load_addr)
        << ", vmsize: " << (seg_cmd_64->vmsize)
        << std::endl;
        
        
        /* iterate dump section */
        struct section_64 *section;
        vm_address_t sect_addr = load_cmd_info->vm_addr + sizeof(struct segment_command_64);
        for(uint32_t nsect = 0; nsect < seg_cmd_64->nsects; nsect++) {
            len = sizeof(struct section_64);
            section = (struct section_64*)malloc(len);
            rt_read(sect_addr, section, sizeof(section_64));
            
            //TODO: use?
            std::cout
            << "      -section: " << (section->sectname)
            << ", address: 0x" << std::hex << (section->addr - MACHO_LOAD_ADDRESS + m_load_addr)
            << ", size: " << (section->size)
            << std::endl;
            
            info->section_64s.push_back(section);
            m_section_64s.push_back(section);
            sect_addr += sizeof(struct section_64);
        }
        
        load_cmd_info->ptr = info;
        return true;
    }
    
    
    load_command_info_t* MachOFile::getLoadCommand(uint32_t cmd_type, char *cmd_name) {
        /* iterate the load commands */
        std::vector<load_command_info_t>::iterator iter;
        load_command_info_t *load_cmd_info;
        for(iter = m_load_command_infos.begin(); iter != m_load_command_infos.end();
            iter++) {
            load_cmd_info = &(*iter);
            if((cmd_type != 0) && (cmd_type != load_cmd_info->cmd.cmd))
                continue;
            
            if((load_cmd_info->cmd.cmd== LC_SEGMENT_64)
               && (cmd_name != NULL)
               && load_cmd_info->ptr
               && (!strcmp(((struct segment_command_64*)(load_cmd_info->ptr))->segname, cmd_name)))
                return load_cmd_info;
            
        }
        return NULL;
    }
    
    /* brute force search dyld*/
    bool MachOFile::searchDyldImageLoadAddress(vm_address_t dyld_vm_addr) {
        vm_address_t start_addr, addr, end_addr;
        uint64_t search_block_size = 0x1000;
        uint32_t magic_64 = MH_MAGIC_64;
        unsigned long aslr_top_limit = ((1 << 16) << 12);
        
        if(m_map_end == 0)
            start_addr = MACHO_LOAD_ADDRESS;
        else
            start_addr = m_map_end;
        
        if(dyld_vm_addr) {
            end_addr = dyld_vm_addr + aslr_top_limit;
            start_addr = dyld_vm_addr;
        } else {
            start_addr = start_addr;
            end_addr = (start_addr+aslr_top_limit) & (~(search_block_size-1));
        }
        
        //search align by memory page
        
        // LINE: xnu-3789.41.3:mach_loader.c:383 dyld_aslr_offset
        // two condition!!!
        // LINE: xnu-3789.41.3:mach_loader.c:649
        // LINE: xnu-3789.41.3:mach_loader.c:718
        
        // slide = vm_map_round_page(slide + binresult->max_vm_addr, effective_page_mask);
        // LINE: xnu-3789.41.3:mach_loader.c:1364
        // vm_offset = scp->vmaddr + slide;
        
        char *buf = (char *)malloc(sizeof(uint32_t));
        
        addr = start_addr;
        
        std::cout << "[*] start dyld search at 0x" << std::hex << addr << std::endl;
        
        while(end_addr > addr) {
            if(rt_read(addr, buf, sizeof(uint32_t))) {
                if((!memcmp(buf, &magic_64, sizeof(uint32_t))) && check_dyld(addr))
                    break;
            }
            
            addr += search_block_size;
        }
        
        if(addr < end_addr) {
            m_dyld_load_addr = addr;
            return true;
        } else {
            m_dyld_load_addr = 0;
            return false;
        }
    }
    
    bool MachOFile::check_dyld(vm_address_t addr) {
        MachOFile dyld;
        dyld.m_load_addr = addr;
        dyld.m_task = m_task;
        
        std::cout << "[*] dyld load address check 0x" << std::hex << addr << std::endl;
        if(!dyld.parse_header())
            return false;
        if(dyld.m_is64bit) {
            if(dyld.m_header64->filetype != MH_DYLINKER)
                return false;
            if(!dyld.parse_load_commands())
                return false;
            if(!dyld.m_isDyldLinker)
                return false;
        } else {
            return false;
        }
        
        return true;
    }
}
