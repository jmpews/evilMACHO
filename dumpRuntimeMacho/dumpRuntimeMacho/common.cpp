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
#include <csignal>

bool
readTaskMemory (task_t t, vm_address_t addr, void* buf, unsigned long len) {
    vm_size_t dataCnt = len;
    kern_return_t kr = vm_read_overwrite(t, addr, len, (vm_address_t)buf, (vm_size_t *)&dataCnt);
    
    if (kr) {
        // fprintf (stderr, "Unable to read target task's memory @%p - kr 0x%x\n" , (void *) addr, kr);
        return false;
    }
    if (len != dataCnt) {
        warnx("rt_read size return not match!");
        return false;
    }
    
    return true;
}

char* readTaskString (task_t t, vm_address_t addr) {
    char x = '\0';
    vm_address_t end;
    char *str = NULL;
    
    end = memorySearch(t, addr, addr+0x1000, &x, 1);
    if(!end) {
        return NULL;
    }
    str = (char *)malloc(end-addr);
    if(readTaskMemory(t, addr, str, end-addr))
        return str;
    
    return NULL;
}

task_t pid2task(unsigned int pid) {
    task_t t;
    task_for_pid(mach_task_self(), pid, &t);
    return t;
}

vm_address_t memorySearch(task_t task, vm_address_t start, vm_address_t end, char *data, unsigned long len) {
    
    vm_address_t addr  = start;
    char *buf = (char *)malloc(len);
    while(end > addr) {
        if(readTaskMemory(task, addr, buf, len))
            if(!memcmp(buf, data, len)) {
                return addr;
            }
        addr += len;
    }
    return 0;
    
//    unsigned long search_block_size = 0x1000;
//    vm_address_t addr  = start;
//    
//    char *buf = (char *)malloc(search_block_size + len);
//    unsigned long search_len;
//    search_len =  search_block_size;
//    
//    while(end >= addr + len || (!end)) {
//
//        if(readTaskMemory(task, addr, buf, search_len + len - 1)) {
//            if(len == 1) {
//                std::cout << "memorySearch: " << buf << std::endl;
//            }
//            for(char *p = buf; p < buf + search_len; p++){
//
//                if(!memcmp(p, data, len)) {
//                    return addr + p - buf;
//                }
//            }
//        } else {
//            if(len == 1) {
//                sleep(-1);
//                std::cout << "memorySearch: error" << std::endl;
//            }
//        }
//        
//        addr += search_block_size;
//    }
//    
//    search_len = end - addr - (len - 1);
//    if(search_len >0 && readTaskMemory(task, addr, buf, search_len + len - 1)) {
//        for(char *p = buf; p < buf + search_len; p++){
//            if(!memcmp(p, data, len)) {
//                return addr + p - buf;
//            }
//        }
//    }
//    return 0;
}
