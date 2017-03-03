//
//  macho.cpp
//  dumpMacho
//
//  Created by jmpews on 20/02/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#include "macho.hpp"
#include "common.hpp"

namespace macho {
    MachOFile::MachOFile()
    : m_fd(-1)
    , m_pid(-1)
    , m_task(-1)
    , m_isDyldLinker(false)
    , m_map_end(MACHO_LOAD_ADDRESS)
    , m_load_address(MACHO_LOAD_ADDRESS)
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
    
    bool MachOFile::getLoadAddress() {
        unsigned long search_block_size = 0x1000;
        //search align by memory page
        vm_address_t addr  = MACHO_LOAD_ADDRESS;
        
        unsigned long aslr_top_limit = ((1 << 16) << 12) + 0x100000000;
        vm_address_t end = aslr_top_limit;
        
        unsigned long size = 1;
        char ch;
        
        while(end > addr) {
            if(readTaskMemory(m_task, addr, &ch, &size)) {
                m_load_address = addr;
                std::cout << "[+] found load address: 0x" << std::hex << addr << std::endl;
                return true;
            }
            
            size = 1;
            addr += search_block_size;
        }
        return false;
    }
    
    
    void MachOFile::setPid(int pid) {
        this->m_pid = pid;
        this->m_task = pid2task(pid);
    }
    
    bool MachOFile::parse_macho() {
        if(!getLoadAddress())
            return false;
        if(!parse_header())
            return false;
        parse_load_commands();
        return true;
    }
    
    bool MachOFile::parse_header() {
        if(!checkInit())
            return false;
        
        unsigned long len;
        
        uint32_t magic;
        len = sizeof(uint32_t);
        if(!readTaskMemory(m_task,
                       m_load_address,
                       &magic,
                       &len))
            return false;
        switch (magic) {
            case MH_MAGIC_64:
                m_is64bit = true;
                len = sizeof(struct mach_header_64);
                m_header64 = (struct mach_header_64*)malloc(len);
                if(!readTaskMemory(m_task, m_load_address, m_header64, &len))
                    return false;
                
                std::cout << "--------------------------------------------------" << std::endl;
                std::cout << "[*] dump macho:" << std::endl;
                std::cout << "[+] macho: Arch-" << "64" << std::endl;
                break;
            default:
                std::cout << "Only support x86_64." << std::endl;
                return false;
        }
        return true;
    }
    
    bool MachOFile::parse_load_commands() {
        unsigned long len = 0;
        uint32_t ncmds = 0;
        struct load_command * tmp_cmd = NULL;
        vm_address_t tmp_addr = m_load_address;
        
        if(m_is64bit) {
            ncmds = m_header64->ncmds;
            tmp_addr += sizeof(mach_header_64);
        }
        else
            return false;
        
        for(int i = 0; i < ncmds; i++) {
            len = sizeof(struct load_command);
            tmp_cmd = (struct load_command*)malloc(len);
            readTaskMemory(m_task, tmp_addr, tmp_cmd, &len);
            
            /* add to vector STL */
            load_command_info_t load_cmd_info;
            load_cmd_info.cmd_type = tmp_cmd->cmd;
            load_cmd_info.cmd = tmp_cmd;
            load_cmd_info.vm_addr = tmp_addr;
            m_load_command_infos.push_back(load_cmd_info);
            
            /* move to next load_command */
            tmp_addr += tmp_cmd->cmdsize;
            
        }
        
        /* iterate the load commands */
        std::vector<load_command_info_t>::iterator iter;
        load_command_info_t *tmp_load_cmd_info;
        for(iter = m_load_command_infos.begin(); iter != m_load_command_infos.end(); iter++) {
            tmp_load_cmd_info = &(*iter);
            switch (tmp_load_cmd_info->cmd_type) {
                case LC_SEGMENT_64:
                    if(!parse_LC_SEGMENT_64(tmp_load_cmd_info))
                        return false;
                    break;
                case LC_ID_DYLINKER:
                    m_isDyldLinker = true;
                default:
                    break;
            }
            
        }
        return true;
    }
    
    bool MachOFile::parse_LC_SEGMENT_64(load_command_info_t* load_cmd_info) {
        struct segment_command_64* seg_cmd;
        unsigned long len;
        
        /* get segment command header */
        len = sizeof(struct segment_command_64);
        seg_cmd = (struct segment_command_64*)malloc(len);
        readTaskMemory(m_task, load_cmd_info->vm_addr, seg_cmd, &len);
        load_cmd_info->cmd_info = seg_cmd;
        
        // set vm map end
        if(seg_cmd->vmaddr - MACHO_LOAD_ADDRESS + m_load_address + seg_cmd->vmsize > m_map_end) {
            m_map_end = seg_cmd->vmaddr - MACHO_LOAD_ADDRESS + m_load_address + seg_cmd->vmsize;
        }
        
        std::cout
        << "SEGMENT: " << (seg_cmd->segname)
        << ", cmdsize: " << (seg_cmd->cmdsize)
        << ", vmaddr: 0x" << std::hex << (seg_cmd->vmaddr - MACHO_LOAD_ADDRESS + m_load_address)
        << ", vmsize: " << (seg_cmd->vmsize)
        << std::endl;
        
        
        /* iterate dump section */
        struct section_64 *tmp_section;
        vm_address_t tmp_sect_addr = load_cmd_info->vm_addr + sizeof(struct segment_command_64);
        for(uint32_t nsect = 0; nsect < seg_cmd->nsects; nsect++) {
            len = sizeof(struct section_64);
            tmp_section = (struct section_64*)malloc(len);
            readTaskMemory(m_task, tmp_sect_addr, tmp_section, &len);
            
            //TODO: use?
            std::cout
            << "\tsection: " << (tmp_section->sectname)
            << ", address: 0x" << std::hex << (tmp_section->addr - MACHO_LOAD_ADDRESS + m_load_address)
            << ", size: " << (tmp_section->size)
            << std::endl;
            
            free(tmp_section);
            tmp_sect_addr += sizeof(struct section_64);
        }
        std::cout << std::endl;
        return true;
    }
    
    /* brute force search dyld*/
    vm_address_t MachOFile::searchDyldImageLoadAddress() {
        vm_address_t start_addr, addr;
        if(m_map_end == 0)
            start_addr = MACHO_LOAD_ADDRESS;
        else
            start_addr = m_map_end;
        
        unsigned long search_block_size = 0x1000;
        
        uint32_t magic_64 = MH_MAGIC_64;
        
        //search align by memory page
        start_addr  = start_addr & ~(search_block_size - 1);
        
        // LINE: xnu-3789.41.3:mach_loader.c:383 dyld_aslr_offset
        // LINE: xnu-3789.41.3:mach_loader.c:649
        // slide = vm_map_round_page(slide + binresult->max_vm_addr, effective_page_mask);
        
        unsigned long aslr_top_limit = ((1 << 16) << 12);
        vm_address_t end = aslr_top_limit + start_addr;
        
        unsigned long size = sizeof(uint32_t);
        char *buf = (char *)malloc(sizeof(uint32_t));
        unsigned long tmp_size;
        tmp_size = size;
        
        addr = start_addr;
        
        std::cout << "[*] start dyld search at 0x" << std::hex << addr << std::endl;
        
        while(end > addr) {
            if(readTaskMemory(m_task, addr, buf, &tmp_size)) {
                if(tmp_size != size) {
                    std::cout << "readTaskMemory size not match." << std::endl;
                    return 0;
                }
                if((!memcmp(buf, &magic_64, size)) && check_dyld(addr))
                    break;
            }
            
            tmp_size = size;
            addr += search_block_size;
        }

        if(addr < end) {
            m_dyld_load_address = addr;
            return true;
        } else {
            m_dyld_load_address = 0;
            return false;
        }
    }
    
    bool MachOFile::check_dyld(vm_address_t addr) {
        MachOFile dyld;
        dyld.m_load_address = addr;
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
