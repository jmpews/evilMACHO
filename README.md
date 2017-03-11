## evilMACHO

Malicious use of macho, such as dump-runtime-macho, function-hook.

#### 1. [evilMACHO/dumpRuntimeMacho](https://github.com/jmpews/evilMACHO/tree/master/dumpRuntimeMacho)

dump runtime macho file and dyld load address.

such as:

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

