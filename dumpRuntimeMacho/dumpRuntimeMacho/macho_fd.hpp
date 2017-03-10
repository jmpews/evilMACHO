//
//  macho_fd.hpp
//  dumpRuntimeMacho
//
//  Created by jmpews on 09/03/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#ifndef macho_fd_hpp
#define macho_fd_hpp

#include <stdio.h>
#include <iostream>

#include <stdio.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <syslog.h>
#include <stdbool.h>
#include <mach-o/arch.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld_images.h>
#include <mach/task.h>
#include <mach/vm_types.h>
#include <vector>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <err.h>

#include <libkern/OSAtomic.h>
#include <mach-o/swap.h>

#include "machotypes.hpp"

#endif /* macho_hpp */

#define MACHO_LOAD_ADDRESS 0x100000000

namespace machofd {
    class MachoFD {
    public:
        MachoFD();
        ~MachoFD();
        
        bool m_is64bit;
        bool m_isDyldLinker;
        bool m_is_universal;
        
        const void* macho_read(const void *address, size_t length);
        const void* macho_offset(const void *address, size_t offset, size_t length);
        
        
        const NXArchInfo*               m_archInfo;
        vm_address_t m_vm_addr;
        macho_input_t                   m_input;
        const struct mach_header*       m_header;
        const struct mach_header_64*    m_header64;
        
        const struct fat_header*        m_fat_header;
        fat_arch_infos_t                m_fat_arch_infos;
        
        size_t m_header_size;
        load_command_infos_t    m_load_command_infos;
        segment_command_64_infos_t      m_segment_command_64_infos;
        section_64s_t                   m_section_64s;
        
        
        bool parse_universal();
        bool parse_file();
        bool parse_macho(const macho_input_t *input);
        bool parse_load_commands();
        bool parse_LC_SYMTAB(load_command_info_t* load_cmd_info);
        bool parse_LC_SEGMENT_64(load_command_info_t* load_cmd_info);
        
        load_command_info_t* getLoadCommand(uint32_t cmd_type, char *cmd_name);
        
        bool setPath(char *path);
        bool checkInit();
        
    private:
        int m_fd;
        char* m_path;
    };
}
