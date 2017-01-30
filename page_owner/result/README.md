## page_owner_full.txt

    cat /sys/kernel/debug/page_owner > page_owner_full.txt


## page_owner_sort
you need to build this tool by your own, on PC (not cross compile).  

    cd tools/vm  
    make page_owner_sort

## sorted_page_owner.txt

### parse result.  
    grep -v ^PFN page_owner_full.txt > page_owner.txt  
    ./page_owner_sort page_owner.txt sorted_page_owner.txt

### check the result
search "do_init_module" in sorted_page_owner.txt, will see the following content.  

    1 times:
    Page allocated via order 0, mask 0x24200c0
     [<bf84803d>] 0xbf84803d
     [<c0009713>] do_one_initcall+0x9b/0x198
     [<c00fb215>] do_init_module+0x4d/0x310
     [<c00a116b>] load_module+0x16eb/0x1b80
     [<c00a17c7>] SyS_finit_module+0x77/0x9c
     [<c000ed21>] ret_fast_syscall+0x1/0x52
     [<ffffffff>] 0xffffffff

can not translate "bf84803d" to function name, bacause save_stack_trace only search the sysmbol
for kernel code itself, don't contain kernel module code.
