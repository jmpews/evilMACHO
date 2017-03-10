## dumpRuntimeMacho

#### Dump at IOS
```
jmpewsde-iPhone:/usr/bin root# dumpRuntimeMacho
[+] found load address: 0x100004000
--------------------------------------------------
[*] dump macho:
[+] macho: Arch-64
SEGMENT: __PAGEZERO, cmdsize: 48, vmaddr: 0x4000, vmsize: 100000000

SEGMENT: __TEXT, cmdsize: 228, vmaddr: 0x100004000, vmsize: 8000
	section: __text, address: 0x100009858, size: 2128
	section: __stubs, address: 0x10000b980, size: 15c
	section: __stub_helper, address: 0x10000badc, size: 15c
	section: __gcc_except_tab__TEXT, address: 0x10000bc38, size: b4
	section: __cstring, address: 0x10000bcec, size: 228
	section: __unwind_info, address: 0x10000bf14, size: ec

SEGMENT: __DATA, cmdsize: 188, vmaddr: 0x10000c000, vmsize: 4000
	section: __got, address: 0x10000c000, size: 30
	section: __la_symbol_ptr, address: 0x10000c030, size: e8
	section: __mod_init_func, address: 0x10000c118, size: 20
	section: __bss, address: 0x10000c138, size: 4

SEGMENT: __LINKEDIT, cmdsize: 48, vmaddr: 0x100010000, vmsize: 2000

[+] string table: 0x1000107e0
[+] symbol table: 0x100010d60
[+] dyld path: /usr/lib/dyld
[+] /usr/lib/dyld vm_addr: 0x120000000
[*] start dyld search at 0x120000000
[*] dyld load address check 0x1200e4000
--------------------------------------------------
[*] dump macho:
[+] macho: Arch-64
SEGMENT: __TEXT, cmdsize: 228, vmaddr: 0x1400e4000, vmsize: 30000
	section: __text, address: 0x1400e5000, size: 23fec
	section: __cstring, address: 0x140108fec, size: 581a
	section: __gcc_except_tab__TEXT, address: 0x14010e808, size: e50
	section: __const, address: 0x14010f660, size: 2c0
	section: __eh_frame, address: 0x14010f920, size: 0
	section: __unwind_info, address: 0x14010f920, size: 808

SEGMENT: __DATA, cmdsize: 278, vmaddr: 0x140114000, vmsize: 38000
	section: __all_image_info__DATA, address: 0x140114000, size: 130
	section: __got, address: 0x140114130, size: 10
	section: __const, address: 0x140114140, size: 1570
	section: __crash_info, address: 0x1401156b0, size: 40
	section: __data, address: 0x1401156f0, size: c40
	section: __common, address: 0x140116330, size: 448
	section: __bss, address: 0x140117000, size: 34028

SEGMENT: __LINKEDIT, cmdsize: 48, vmaddr: 0x14014c000, vmsize: 18000

[+] string table: 0x1201512e8
[+] symbol table: 0x120155b90
[+] DyldImageLoadAddress: 0x1200e4000
jmpewsde-iPhone:/usr/bin root#
```

#### Dump at macOS
```
[+] found load address: 0x100000000
--------------------------------------------------
[*] dump macho:
[+] macho: Arch-64
SEGMENT: __PAGEZERO, cmdsize: 48, vmaddr: 0x0, vmsize: 100000000

SEGMENT: __TEXT, cmdsize: 278, vmaddr: 0x100000000, vmsize: a000
	section: __text, address: 0x1000006e0, size: 90d0
	section: __stubs, address: 0x1000097b0, size: d2
	section: __stub_helper, address: 0x100009884, size: 15a
	section: __gcc_except_tab__TEXT, address: 0x1000099e0, size: 204
	section: __cstring, address: 0x100009be4, size: 228
	section: __unwind_info, address: 0x100009e0c, size: 108
	section: __eh_frame, address: 0x100009f18, size: e0

SEGMENT: __DATA, cmdsize: 138, vmaddr: 0x10000a000, vmsize: 1000
	section: __nl_symbol_ptr, address: 0x10000a000, size: 10
	section: __got, address: 0x10000a010, size: 20
	section: __la_symbol_ptr, address: 0x10000a030, size: 118

SEGMENT: __LINKEDIT, cmdsize: 48, vmaddr: 0x10000b000, vmsize: b000

[+] string table: 0x10000ba50
[+] symbol table: 0x10000e450
[+] dyld path: /usr/lib/dyld
[+] /usr/lib/dyld vm_addr: 0x0
[*] start dyld search at 0x100016000
[*] dyld load address check 0x100016000
--------------------------------------------------
[*] dump macho:
[+] macho: Arch-64
SEGMENT: __TEXT, cmdsize: 228, vmaddr: 0x16000, vmsize: 3e000
	section: __text, address: 0x17000, size: 2a2fa
	section: __cstring, address: 0x41300, size: 73db
	section: __gcc_except_tab__TEXT, address: 0x486dc, size: 16e0
	section: __const, address: 0x49dc0, size: 2d8
	section: __unwind_info, address: 0x4a098, size: a5c
	section: __eh_frame, address: 0x4aaf8, size: 8770

SEGMENT: __DATA, cmdsize: 278, vmaddr: 0x54000, vmsize: 37000
	section: __all_image_info__DATA, address: 0x54000, size: 130
	section: __got, address: 0x54130, size: 30
	section: __const, address: 0x54160, size: 16f0
	section: __crash_info, address: 0x55850, size: 40
	section: __data, address: 0x55890, size: e38
	section: __common, address: 0x566d0, size: 460
	section: __bss, address: 0x56b40, size: 34111

SEGMENT: __LINKEDIT, cmdsize: 48, vmaddr: 0x8b000, vmsize: 16000

[+] string table: 0x10008d3f8
[+] symbol table: 0x1000920a0
[+] _dlopen: 0x100022f24
[+] DyldImageLoadAddress: 0x100016000
```

