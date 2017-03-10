//
//  macho_fd.cpp
//  dumpRuntimeMacho
//
//  Created by jmpews on 09/03/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#include "macho_fd.hpp"
#include "common.hpp"

namespace machofd {
    MachoFD::MachoFD()
    : m_fd(-1)
    , m_isDyldLinker(false)
    , m_header(NULL)
    , m_header64(NULL)
    {
    }
    MachoFD::~MachoFD() {
        if(m_is64bit) {
        }
    }
    bool MachoFD::checkInit() {
        if(m_fd > 0) {
            
            return true;
        }
        std::cout << "macho not be init." << std::endl;
        return false;
    }
    
    bool MachoFD::setPath(char* path) {
        m_path = path;
        return true;
    }
    /* Verify that address + offset + length is within bounds. */
    const void* MachoFD::macho_offset(const void *address, size_t offset, size_t length) {
        void *result = ((uint8_t *) address) + offset;
        return macho_read(result, length);
    }
    
    /* Verify that the given range is within bounds. */
    const void* MachoFD::macho_read(const void *address, size_t length) {
        if ((((uint8_t *) address) - ((uint8_t *) m_input.data)) + length > m_input.length) {
            warnx("Short read parsing Mach-O input");
            return NULL;
        }
        
        return address;
    }
    bool MachoFD::parse_LC_SEGMENT_64(load_command_info_t* load_cmd_info)
    {
        struct segment_command_64* seg_cmd_64 = (struct segment_command_64*)load_cmd_info->fileoff;
        segment_command_64_info_t* info = new segment_command_64_info_t();
        if (info == NULL) {
            return false;
        }
        
        info->cmd_type = load_cmd_info->cmd.cmd;
        info->cmd = seg_cmd_64;
        
        m_segment_command_64_infos.push_back(info);
        
        if (strcmp(seg_cmd_64->segname, "__TEXT") == 0) {
            m_vm_addr = seg_cmd_64->vmaddr;
            std::cout << "[+] " << m_path << " vm_addr: 0x" << m_vm_addr << std::endl;
        }
        
        // Section Headers
        for (uint32_t nsect = 0; nsect < seg_cmd_64->nsects; ++nsect)
        {
            struct section_64* section = (struct section_64*)macho_offset(seg_cmd_64, sizeof(struct segment_command_64) + nsect * sizeof(struct section_64), sizeof(struct section_64));
            if (section == NULL) {
                return false;
            }
            
            info->section_64s.push_back(section);
            m_section_64s.push_back(section);
        }
        
        load_cmd_info->ptr = info;
        
        return true;
    }
    
    bool MachoFD::parse_load_commands()
    {
        const struct load_command* cmd = (const struct load_command*)macho_offset(m_header, m_header_size, sizeof(struct load_command));
        if (cmd == NULL) {
            return false;
        }
        
        uint32_t ncmds = m_header->ncmds;
        
        /* Get the load commands */
        for (uint32_t i = 0; i < ncmds; i++) {
            /* Load the full command */
            uint32_t cmdsize = cmd->cmdsize;
            cmd = (const struct load_command*)macho_read(cmd, cmdsize);
            if (cmd == NULL) {
                return false;
            }
            
            load_command_info_t load_cmd_info;
            load_cmd_info.fileoff = (void *)cmd;
            memcpy(&(load_cmd_info.cmd), cmd, sizeof(struct load_command));
            load_cmd_info.ptr = NULL;
            
            m_load_command_infos.push_back(load_cmd_info);
            
            /* Load the next command */
            cmd = (const struct load_command*)macho_offset(cmd, cmdsize, sizeof(struct load_command));
            if (cmd == NULL) {
                return false;
            }
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
            }
        }
        return true;
    }
    
    bool MachoFD::parse_universal()
    {
        uint32_t nfat = OSSwapBigToHostInt32(m_fat_header->nfat_arch);
        const struct fat_arch* archs = (const struct fat_arch*)macho_offset(m_fat_header, sizeof(struct fat_header), sizeof(struct fat_arch));
        if (archs == NULL) {
            return false;
        }
        
        //printf("Architecture Count: %d\n", nfat);
        for (uint32_t i = 0; i < nfat; i++) {
            const struct fat_arch* arch = (const struct fat_arch*)macho_read(archs + i, sizeof(struct fat_arch));
            if (arch == NULL)
                return false;
            
            /* Fetch a pointer to the architecture's Mach-O header. */
            size_t length = OSSwapBigToHostInt32(arch->size);
            const void *data = macho_offset(m_input.data, OSSwapBigToHostInt32(arch->offset), length);
            if (data == NULL)
                return false;
            
            fat_arch_info_t fat_arch_info;
            memcpy(&(fat_arch_info.arch), arch, sizeof(struct fat_arch));
            fat_arch_info.ptr = NULL;
            fat_arch_info.input.length = length;
            fat_arch_info.input.data = data;
            
            m_fat_arch_infos.push_back(fat_arch_info);
        }
        
        std::vector<fat_arch_info_t>::iterator iter;
        fat_arch_info_t* arch;
        for(iter = m_fat_arch_infos.begin(); iter != m_fat_arch_infos.end(); iter++) {
            arch = &(*iter);
            switch (OSSwapBigToHostInt32(arch->arch.cputype)) {
                case CPU_TYPE_X86_64:
                    parse_macho(&(arch->input));
                    break;
                case CPU_TYPE_ARM64:
                    parse_macho(&(arch->input));
                    break;
                default:
                    break;
            }
        }
        return true;
    }
    
    
    /* Parse a Mach-O header */
    bool MachoFD::parse_macho(const macho_input_t *input) {
        
        /* Read the file type. */
        const uint32_t* magic = (const uint32_t*)macho_read(input->data, sizeof(uint32_t));
        if (magic == NULL) {
            return false;
        }
        
        switch (*magic) {
                
            case MH_MAGIC:
                m_header_size = sizeof(*m_header);
                m_header = (const struct mach_header*)macho_read(input->data, m_header_size);
                if (m_header == NULL) {
                    return false;
                }
                break;
                
                
            case MH_MAGIC_64:
                m_header_size = sizeof(*m_header64);
                m_header64 = (const struct mach_header_64*)macho_read(input->data, m_header_size);
                if (m_header64 == NULL)
                    return false;
                
                /* The 64-bit header is a direct superset of the 32-bit header */
                m_header = (struct mach_header *)m_header64;
                
                m_is64bit = true;
                parse_load_commands();
                break;
                
            case FAT_CIGAM:
            case FAT_MAGIC:
                m_fat_header = (const struct fat_header*)macho_read(input->data, sizeof(struct fat_header));
                m_is_universal = true;
                parse_universal();
                break;
                
            default:
                warnx("Unknown Mach-O magic: 0x%x", *magic);
                return false;
        }
        return true;
    }
    
    
    bool MachoFD::parse_file()
    {
        m_fd = open(m_path, O_RDONLY);
        if (m_fd < 0) {
            return false;
        }
        
        struct stat stbuf;
        if (fstat(m_fd, &stbuf) != 0) {
            return false;
        }
        
        /* mmap */
        m_input.data = mmap(NULL, stbuf.st_size, PROT_READ, MAP_FILE|MAP_PRIVATE, m_fd, 0);
        if (m_input.data == MAP_FAILED) {
            return false;
        }
        
        m_input.length = stbuf.st_size;
        
        /* Parse */
        return parse_macho(&m_input);
    }
}
