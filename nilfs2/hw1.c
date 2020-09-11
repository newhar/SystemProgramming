
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "hw1"

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

extern struct circular_queue cq;

static int hw1_open(struct inode *inode, struct file *file) {
    printk(KERN_ALERT "<hw1_file_open>\n");
    return 0;
}

static ssize_t hw1_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos) {
    int i, start_index;
    struct file *filp;
    char tmp[19];

    start_index = cq.idx;
    
    
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);
    
    //open file
    filp = filp_open("/tmp/result.csv", O_WRONLY|O_CREAT, 0644);
    if(IS_ERR(filp)) {
        printk("<file_open_error>\n");
	    set_fs(old_fs);
	
	    return count;
    }
    else {
        printk("<file_open_success>\n");
    }
    
    printk(KERN_ALERT "<hw1_write>\n");

	
    for(i = start_index + 1; i != start_index; i++) {
	i%=2000;
	
		if(cq.tp_buf[i])
		if(user_buffer)
		if(strncmp(cq.tp_buf[i], user_buffer, count-1) == 0)
		if(cq.bk_buf[i]!=0) {
			printk(KERN_INFO "%d:[%lld] %lld\n", i, cq.tm_buf[i], cq.bk_buf[i]);
			
			snprintf(tmp, 19, "%lld", cq.tm_buf[i]);
			vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
			vfs_write(filp, ", ", 2, &filp->f_pos);
	        
			snprintf(tmp, 19, "%lld", cq.bk_buf[i]);
			
			vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
			vfs_write(filp, ", ", 2, &filp->f_pos);
			
			printk("%s\n", cq.tp_buf[i]);
			
			snprintf(tmp, 19, "%s", cq.tp_buf[i]);
			vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
			vfs_write(filp, "\n", 1, &filp->f_pos);

			cq.tm_buf[i]=0;
			cq.bk_buf[i]=0;
			cq.tp_buf[i]=NULL;
		}
   }

    printk(KERN_ALERT "<hw1_write_complete>\n");

	//close file
    filp_close(filp, NULL);

    //restore kernel memory setting
    set_fs(old_fs);



    return count;
}

static const struct file_operations hw1_proc_fops = {
    .owner = THIS_MODULE,
    .open = hw1_open,
    .write = hw1_write,
};

static int __init hw1_init(void) {
    printk(KERN_ALERT "<hw1_module_up>\n");
    proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
    proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &hw1_proc_fops);
    return 0;
}

static void __exit hw1_exit(void) {
    printk(KERN_ALERT "<hw1_module_down>\n");

    return;
}

module_init(hw1_init);
module_exit(hw1_exit);

MODULE_AUTHOR("kyj,phj");
MODULE_DESCRIPTION("System Programming");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
