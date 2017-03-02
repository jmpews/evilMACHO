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

        
    kern_return_t kr = vm_read_overwrite(t,
                                         addr,
                                         dataCnt,
                                         (vm_address_t)buf,
                                         (vm_size_t *)size);
    
    if (kr) {
        fprintf (stderr, "Unable to read target task's memory @%p - kr 0x%x\n" ,
                 (void *) addr, kr);
        return false;
    }
    
    return true;
}

task_t pid2task(unsigned int pid) {
    task_t t;
    task_for_pid(mach_task_self(), pid, &t);
    return t;
}

vm_address_t memorySearch(task_t task, vm_address_t start, vm_address_t end, char *data, unsigned long size) {
    char *result = NULL;
    vm_address_t addr  = start;
    unsigned long search_block_size = 0x1000;
    
    //search align by memory page
    //vm_address_t start_addr = start & page_align;
    
    char *buf = (char *)malloc(search_block_size + size);
    unsigned long tmp_size;
    tmp_size = search_block_size + size - 1;
    
    while((end > addr + size) || (!end) ) {
        //TODO: check size of return
        if(!readTaskMemory(task, addr, buf, &tmp_size))
            return 0;
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
            if( end && (addr + tmp_size > end)) {
                tmp_size = end - addr;
            }
    }
    return 0;
}
