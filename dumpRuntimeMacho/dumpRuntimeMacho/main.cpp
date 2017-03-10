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
#include "macho_rt.hpp"
#include "macho_fd.hpp"

using namespace machort;

int main(int argc, const char * argv[]) {
//    task_t targetTask = mach_task_self();
//    task_t remoteTask;
    pid_t  remotePid;
    remotePid = getpid();
    MachOFile machoFile;
//    machoFile.setPid(28307);
    machoFile.setPid(remotePid);
    machoFile.parse_macho();
    

    machofd::MachoFD dyld;
    dyld.setPath(machoFile.m_dyld_path);
    dyld.parse_file();
//    machoFile.m_dyld_vm_addr = dyld.m_vm_addr;
    if(machoFile.searchDyldImageLoadAddress(dyld.m_vm_addr)) {
        std::cout << "[+] DyldImageLoadAddress: 0x" << std::hex << machoFile.m_dyld_load_addr << std::endl;
    }
    
    return 0;
}

