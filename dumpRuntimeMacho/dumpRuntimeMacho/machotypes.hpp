#ifndef macho_hpp
#define macho_hpp

#include <iostream>
#include <vector>
// macho
#include <mach-o/arch.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld_images.h>
#include <mach/task.h>
#include <mach/vm_types.h>

typedef struct macho_input {
    const void* data;
    size_t      length;
    uint64_t    baseOffset;
} macho_input_t;

typedef struct fat_arch_info {
    struct fat_arch arch;
    macho_input_t   input;
    const void*     ptr;
} fat_arch_info_t;

typedef std::vector<fat_arch_info_t> fat_arch_infos_t;

typedef struct load_command_info {
    struct load_command cmd;
    vm_address_t vm_addr;
    void* fileoff;
    void* ptr;
} load_command_info_t;
typedef std::vector<load_command_info_t> load_command_infos_t;


typedef std::vector<struct section_64*> section_64s_t;
typedef struct segment_command_64_info {
    uint32_t                            cmd_type;
    struct segment_command_64*    cmd;
    section_64s_t                       section_64s;
} segment_command_64_info_t;

typedef std::vector<segment_command_64_info_t*> segment_command_64_infos_t;
#endif
