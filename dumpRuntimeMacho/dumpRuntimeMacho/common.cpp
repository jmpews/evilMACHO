//
//  common.cpp
//  dumpMacho
//
//  Created by jmpews on 20/02/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//
#include <iostream>

#include "common.hpp"
#include <stdlib.h>
bool
readTaskMemory (task_t t,
                   vm_address_t addr,
                   void* buf,
                   unsigned long* size) {

    vm_size_t dataCnt = *size;

        
    kern_return_t kr = vm_read_overwrite(t, addr, dataCnt, (vm_address_t)buf, (vm_size_t *)size);
    
    if (kr) {
//        fprintf (stderr, "Unable to read target task's memory @%p - kr 0x%x\n" , (void *) addr, kr);
        return false;
    }
    
    return true;
}

task_t pid2task(unsigned int pid) {
    task_t t;
    task_for_pid(mach_task_self(), pid, &t);
    return t;
}

/* brute force search dyld*/
vm_address_t memorySearchDyld(task_t task, vm_address_t start,char *data, unsigned long size) {
    
    unsigned long search_block_size = 0x1000;
    //search align by memory page
    vm_address_t addr  = start & ~(search_block_size - 1);
    
    unsigned long dyld_aslr_top_limit = ((1 << 16) << 12) + 0x100000000;
    vm_address_t end = dyld_aslr_top_limit;
    
    char *buf = (char *)malloc(size);
    unsigned long tmp_size;
    tmp_size = size;
    
    while(end > addr) {
        if(readTaskMemory(task, addr, buf, &tmp_size)) {
            if(tmp_size != size) {
                std::cout << "readTaskMemory size not match." << std::endl;
                return 0;
            }
            if(!memcmp(buf, data, size))
                return addr;
        }
        
        tmp_size = size;
        addr += search_block_size;
    }
    return 0;
}

vm_address_t memorySearch(task_t task, vm_address_t start, vm_address_t end, char *data, unsigned long size) {
    
    unsigned long search_block_size = 0x1000;
    vm_address_t addr  = start;
    
    char *buf = (char *)malloc(search_block_size + size);
    unsigned long tmp_size;
    tmp_size = search_block_size + size - 1;
    
    while(end > addr + size || (!end)) {

        if(!readTaskMemory(task, addr, buf, &tmp_size)) {
            return 0;
        }
        
        if(tmp_size != (search_block_size + size - 1)) {
            std::cout << "readTaskMemory size not match.";
            return 0;
        }
        for(char *p = buf; p < buf + search_block_size; p++){
            if(!memcmp(p, data, size)) {
                return addr + p - buf;
            }
        }

        tmp_size = search_block_size + size - 1;
        addr += search_block_size;
        if( end && addr + tmp_size > end) {
            tmp_size = end - addr;
        }
    }
    return 0;
}
