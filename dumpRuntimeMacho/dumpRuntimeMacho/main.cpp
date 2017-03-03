//
//  main.cpp
//  dumpMacho
//
//  Created by jmpews on 20/02/2017.
//  Copyright © 2017 spiderzz. All rights reserved.
//

#include <iostream>
#include <unistd.h>

#include "common.hpp"
#include "macho.hpp"

using namespace macho;

int main(int argc, const char * argv[]) {
    task_t targetTask = mach_task_self();
    task_t remoteTask;
    pid_t  remotePid;
    remotePid = getpid();
    MachOFile machoFile;
    machoFile.setPid(remotePid);
    machoFile.parse_macho();
    
    vm_address_t dyld_load_address = machoFile.searchDyldImageLoadAddress();
    if(dyld_load_address)
        std::cout << "[+] DyldImageLoadAddress: 0x" << std::hex << dyld_load_address << std::endl;
    return 0;
}

