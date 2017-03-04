//
//  macho.hpp
//  dumpMacho
//
//  Created by jmpews on 20/02/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#ifndef macho_hpp
#define macho_hpp

#include <iostream>

#include <stdio.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <syslog.h>
#include <stdbool.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld_images.h>
#include <mach/task.h>
#include <mach/vm_types.h>
#include <vector>

#endif /* macho_hpp */

#define MACHO_LOAD_ADDRESS 0x100000000

namespace macho {
    typedef struct load_command_info {
        uint32_t cmd_type;
        vm_address_t vm_addr;
        const struct load_command* cmd;
        void* cmd_info;
    } load_command_info_t;
    
    typedef std::vector<load_command_info_t> load_command_infos_t;
    
    
    class MachOFile{
    public:
        MachOFile();
        ~MachOFile();
        
        vm_address_t m_load_address;
        vm_address_t m_dyld_load_address;
        vm_address_t m_link_edit_bias;
        bool m_is64bit;
        bool m_isDyldLinker;
        struct mach_header*       m_header;
        struct mach_header_64*    m_header64;
        load_command_infos_t    m_load_command_infos;
        
        bool parse_macho();
        bool parse_header();
        bool parse_LC_SEGMENT_64(load_command_info_t* load_cmd_info);
        bool parse_load_commands();
        bool parse_LC_SYMTAB(load_command_info_t* load_cmd_info);
        
        bool getLoadAddress();
        void setPid(int pid);
        bool checkInit();
        vm_address_t searchDyldImageLoadAddress();
        vm_address_t memorySearchDyld();
        bool check_dyld(vm_address_t addr);
    
    private:
        int m_pid;
        int m_fd;
        vm_address_t m_map_end;
        task_t m_task;
    };

    class DyldFile: public MachOFile {
        //TODO:
    };
    
}

