//
//  common.hpp
//  dumpMacho
//
//  Created by jmpews on 20/02/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#ifndef common_hpp
#define common_hpp

#include <stdio.h>
#include <mach/task_info.h>
#include <mach/task.h>
#include <mach-o/loader.h>
#include <mach-o/dyld_images.h>
#include <mach/mach_vm.h>

task_t pid2task(unsigned int pid);

bool
readTaskMemory (task_t t,
                   vm_address_t addr,
                   void* buf,
                   unsigned long* size);

vm_address_t memorySearch(task_t task, vm_address_t start, vm_address_t end, char *data, unsigned long size);
vm_address_t memorySearchDyld(task_t task, vm_address_t start,char *data, unsigned long size);

#endif /* common_hpp */
