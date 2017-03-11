## dumpRuntimeMacho

## Make & Install

#### IOS

```
$ export THEOS_DEVICE_IP=192.168.2.110 THEOS_DEVICE_PORT=22
$ cd dumpRuntimeMacho/dumpRuntimeMacho
$ make do FINALPACKAGE=1
```

#### macOS

```
1. use Xcode open project.
2. edit Product -> Scheme -> `Debug Process as root`
3. build & run
```

## Example:
#### dump at IOS
```
jmpewsde-iPhone:/usr/bin root# ./dumpRuntimeMacho
[*] target pid: 28251
[+] BinLoadAddress: 0x100098000
[*] start dump macho:
[+] macho: Arch-64
    +segment: __PAGEZERO, cmdsize: 48, vmaddr: 0x98000, vmsize: 100000000
    +segment: __TEXT, cmdsize: 228, vmaddr: 0x100098000, vmsize: 8000
      -section: __text, address: 0x10009d7b0, size: 21a0
      -section: __stubs, address: 0x10009f950, size: 174
      -section: __stub_helper, address: 0x10009fac4, size: 174
      -section: __gcc_except_tab__TEXT, address: 0x10009fc38, size: b4
      -section: __cstring, address: 0x10009fcec, size: 222
      -section: __unwind_info, address: 0x10009ff10, size: f0
    +segment: __DATA, cmdsize: 188, vmaddr: 0x1000a0000, vmsize: 4000
      -section: __got, address: 0x1000a0000, size: 30
      -section: __la_symbol_ptr, address: 0x1000a0030, size: f8
      -section: __mod_init_func, address: 0x1000a0128, size: 20
      -section: __bss, address: 0x1000a0148, size: 4
    +segment: __LINKEDIT, cmdsize: 48, vmaddr: 0x1000a4000, vmsize: 2000
[+] string table: 0x1000a4820
[+] symbol table: 0x1000a4de0
[+] dyld path: /usr/lib/dyld
[+] /usr/lib/dyld vm_addr: 0x120000000
[*] start dyld search at 0x120000000
[*] dyld load address check 0x12000c000
[+] macho: Arch-64
[+] DyldImageLoadAddress: 0x12000c000
```

#### dump at macOS
```
λ : ./dumpRuntimeMacho
[*] target pid: 7185
[+] BinLoadAddress: 0x10ade0000
[*] start dump macho:
[+] macho: Arch-64
    +segment: __PAGEZERO, cmdsize: 48, vmaddr: 0xade0000, vmsize: 100000000
    +segment: __TEXT, cmdsize: 278, vmaddr: 0x10ade0000, vmsize: b000
      -section: __text, address: 0x10ade1480, size: 9320
      -section: __stubs, address: 0x10adea7a0, size: de
      -section: __stub_helper, address: 0x10adea880, size: 16e
      -section: __gcc_except_tab__TEXT, address: 0x10adea9f0, size: 204
      -section: __cstring, address: 0x10adeabf4, size: 222
      -section: __unwind_info, address: 0x10adeae18, size: 108
      -section: __eh_frame, address: 0x10adeaf20, size: e0
    +segment: __DATA, cmdsize: 138, vmaddr: 0x10adeb000, vmsize: 1000
      -section: __nl_symbol_ptr, address: 0x10adeb000, size: 10
      -section: __got, address: 0x10adeb010, size: 20
      -section: __la_symbol_ptr, address: 0x10adeb030, size: 128
    +segment: __LINKEDIT, cmdsize: 48, vmaddr: 0x10adec000, vmsize: b000
[+] string table: 0x10adecab0
[+] symbol table: 0x10adef530
[+] dyld path: /usr/lib/dyld
[+] /usr/lib/dyld vm_addr: 0x0
[*] start dyld search at 0x10adf7000
[*] dyld load address check 0x10ae2b000
[+] macho: Arch-64
[+] DyldImageLoadAddress: 0x10ae2b000
```

