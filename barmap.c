#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/mm.h>

 
#define DEV_NAME "barmap"

struct dentry  *file1;
 
 
int my_mmap(struct file *filp, struct vm_area_struct *vma)
{
    pr_info("barmap mmap called");
    unsigned long bar_start = 0x208fe0000000UL; // Starting physical address of BAR space
    unsigned long bar_size = 256UL*1024*1024; // BAR size in bytes
    unsigned long size   = vma->vm_end - vma->vm_start;
    int ret;

    if(size <= bar_size) {
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
        ret = io_remap_pfn_range(vma, vma->vm_start,
                     bar_start >> PAGE_SHIFT,
                     size, vma->vm_page_prot);
        if(ret) {
            pr_info("io_remap_pfn_range failed");
            return -EAGAIN;
        }
    } else {
        pr_info("mmap region size exceeds BAR size");
        return -EINVAL;
    }

    pr_info("barmap mmap successful");
    return 0;
}
 
int my_close(struct inode *inode, struct file *filp)
{
    pr_info("barmap close");
    return 0;
}
 
int my_open(struct inode *inode, struct file *filp)
{
    pr_info("barmap open");
    return 0;
}
 
static const struct file_operations my_fops = {
    .open = my_open,
    .release = my_close,
    .mmap = my_mmap,
};
 
static int __init mmapexample_module_init(void)
{
    file1 = debugfs_create_file_unsafe(DEV_NAME, 0644, NULL, NULL, &my_fops);
    return 0;
}
 
static void __exit mmapexample_module_exit(void)
{
    debugfs_remove(file1);
 
}
 
module_init(mmapexample_module_init);
module_exit(mmapexample_module_exit);
MODULE_AUTHOR("Midhul");
MODULE_LICENSE("GPL");