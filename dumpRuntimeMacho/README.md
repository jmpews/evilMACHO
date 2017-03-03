## dumpRuntimeMacho

```
[+] found load address: 0x100000000
--------------------------------------------------
[*] dump macho:
[+] macho: Arch-64
SEGMENT: __PAGEZERO, cmdsize: 48, vmaddr: 0x0, vmsize: 100000000

SEGMENT: __TEXT, cmdsize: 278, vmaddr: 0x100000000, vmsize: 4000
	section: __text, address: 0x1000006d0, size: 32f0
	section: __stubs, address: 0x1000039c0, size: b4
	section: __stub_helper, address: 0x100003a74, size: 128
	section: __gcc_except_tab__TEXT, address: 0x100003b9c, size: 170
	section: __cstring, address: 0x100003d0c, size: 161
	section: __unwind_info, address: 0x100003e70, size: c8
	section: __eh_frame, address: 0x100003f38, size: c8

SEGMENT: __DATA, cmdsize: 138, vmaddr: 0x100004000, vmsize: 1000
	section: __nl_symbol_ptr, address: 0x100004000, size: 10
	section: __got, address: 0x100004010, size: 20
	section: __la_symbol_ptr, address: 0x100004030, size: f0

SEGMENT: __LINKEDIT, cmdsize: 48, vmaddr: 0x100005000, vmsize: 7000

[*] start dyld search at 0x10000c000
[*] dyld load address check 0x10000c000
--------------------------------------------------
[*] dump macho:
[+] macho: Arch-64
SEGMENT: __TEXT, cmdsize: 228, vmaddr: 0xc000, vmsize: 3e000
	section: __text, address: 0xd000, size: 2a2fa
	section: __cstring, address: 0x37300, size: 73db
	section: __gcc_except_tab__TEXT, address: 0x3e6dc, size: 16e0
	section: __const, address: 0x3fdc0, size: 2d8
	section: __unwind_info, address: 0x40098, size: a5c
	section: __eh_frame, address: 0x40af8, size: 8770

SEGMENT: __DATA, cmdsize: 278, vmaddr: 0x4a000, vmsize: 37000
	section: __all_image_info__DATA, address: 0x4a000, size: 130
	section: __got, address: 0x4a130, size: 30
	section: __const, address: 0x4a160, size: 16f0
	section: __crash_info, address: 0x4b850, size: 40
	section: __data, address: 0x4b890, size: e38
	section: __common, address: 0x4c6d0, size: 460
	section: __bss, address: 0x4cb40, size: 34111

SEGMENT: __LINKEDIT, cmdsize: 48, vmaddr: 0x81000, vmsize: 16000

[+] DyldImageLoadAddress: 0x10000c000
Program ended with exit code: 0
```
