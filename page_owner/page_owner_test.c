#include <linux/init.h>
#include <linux/module.h>

#include <linux/errno.h>
#include <linux/string.h>

#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>

/* 
 * Another special macro (MODULE_LICENSE) is used to tell the kernel that this 
 * module bears a free license; without such a declaration, the kernel 
 * complains when the module is loaded.
 */
MODULE_LICENSE("Dual BSD/GPL");

/*
 * Module parameter
 */
static char *alloc_way = "alloc";	//alloc/vmalloc
module_param(alloc_way, charp, S_IRUGO);

static int page_order = 0;
module_param(page_order, int, S_IRUGO);

static struct page *alloc_page = NULL;
static void *vmalloc_addr = NULL;

//if want to use static, must as "static noline int xxx"
//because if static function only be called once, gcc will compile it as inline
int alloc_page_owner(void)
{
	void *page_addr;
	
	// alloc
	alloc_page = alloc_pages(GFP_KERNEL, page_order);
	if (!alloc_page) {
		printk(KERN_ERR "alloc_pages failed\n");
		return -ENOMEM;
	}
		
	// use the pages (we have tested, don't need to use the page)
	//page_addr = page_address(alloc_page);
	//memset(page_addr, 0, (1 << page_order) * PAGE_SIZE);
	return 0;
}

int vmalloc_page_owner(void)
{
	//alloc
	vmalloc_addr = vmalloc((1 << page_order) * PAGE_SIZE);
	if (!vmalloc_addr) {
		printk(KERN_ERR "vmalloc failed\n");
		return -ENOMEM;
	}
	
	//use the pages (we have tested, don't need to use the page)
	
	return 0;
}

static int __init hello_init(void)
{
	int ret = 0;
	
	printk(KERN_ALERT "Use [%s] way to alloc [%d] pages\n", alloc_way, 1 << page_order);
	
	if (!strcmp(alloc_way, "alloc")) {
		ret = alloc_page_owner();
	} else if (!strcmp(alloc_way, "vmalloc")) {
		ret = vmalloc_page_owner();
	} else {
		printk(KERN_ERR "Invalid way %s\n", alloc_way);
		return -EINVAL;
	}
	
	return ret;
}

static void hello_exit(void)
{
	if (alloc_page)
		__free_pages(alloc_page, page_order);
	
	if (vmalloc_addr)
		vfree(vmalloc_addr);
	
	printk(KERN_ALERT "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("John LiuXin");
MODULE_DESCRIPTION("Example of page owner");