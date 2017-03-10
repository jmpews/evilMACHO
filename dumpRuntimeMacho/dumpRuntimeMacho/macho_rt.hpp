//
//  macho_rt.hpp
//  dumpRuntimeMacho
//
//  Created by jmpews on 09/03/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#ifndef macho_rt_hpp
#define macho_rt_hpp

#include <stdio.h>

#include <iostream>

#include <stdio.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <syslog.h>
#include <stdbool.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld_images.h>
#include <mach/task.h>
#include <mach/vm_types.h>

#include "machotypes.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>



#include <libkern/OSAtomic.h>
#include <mach-o/swap.h>

#endif /* macho_rt_hpp */

#define MACHO_LOAD_ADDRESS 0x100000000

namespace machort {
    class MachOFile{
    public:
        MachOFile();
        ~MachOFile();

        /* runtime read */
        bool rt_read(vm_address_t addr, void* buf, size_t len);
        char* rt_readString(vm_address_t addr);
        
        /* Runtime Var */
        vm_address_t m_load_addr;
        size_t m_slide;
        vm_address_t m_dyld_vm_addr;
        vm_address_t m_dyld_load_addr;
        vm_address_t m_link_edit_bias;
        vm_address_t m_symtab_addr;
        vm_address_t m_strtab_addr;
        char* m_dyld_path;
        
        /* Runtime Method */
        void setPid(int pid);
        bool searchBinLoadAddress();
        bool searchDyldImageLoadAddress(vm_address_t dyld_vm_addr);
        vm_address_t memorySearchDyld();
        
        bool m_is64bit;
        bool m_isDyldLinker;

        struct mach_header*       m_header;
        struct mach_header_64*    m_header64;
        load_command_infos_t    m_load_command_infos;
        segment_command_64_infos_t      m_segment_command_64_infos;
        section_64s_t                   m_section_64s;

        bool parse_macho();
        bool parse_header();
        bool parse_load_commands();
        bool parse_LC_SYMTAB(load_command_info_t* load_cmd_info);
        bool parse_LC_LOAD_DYLINKER(load_command_info_t* load_cmd_info);
        bool parse_LC_SEGMENT_64(load_command_info_t* load_cmd_info);
        
        load_command_info_t* getLoadCommand(uint32_t cmd_type, char *cmd_name);
        
        bool checkInit();
        bool check_dyld(vm_address_t addr);
    
    private:
        int m_pid;
        vm_address_t m_map_end;
        task_t m_task;
    };
}
