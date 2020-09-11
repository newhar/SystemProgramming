#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "hw2"
#define PORT 33333

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;








unsigned int server_port[5]={33333,4444,5555,6666,7777};



unsigned int inet_addr(char *str)
{
	int i,j,k,l;
	char arr[4];
	sscanf(str,"%d.%d.%d.%d",&i,&j,&k,&l);
	arr[0]=i;
	arr[1]=j;
	arr[2]=k;
	arr[3]=l;

	return *(unsigned int *)arr;
}




static unsigned int pre_my_hook_fn(void *priv,
								struct sk_buff *skb,
								const struct nf_hook_state *state)
{
	struct iphdr *iph;
	struct tcphdr *tcph;

	if(!skb) return NF_ACCEPT;

	iph=ip_hdr(skb);
	
	unsigned int sp,dp;
	char saddr[16],daddr[16];

	tcph=tcp_hdr(skb);
	sp=htons((unsigned short int)tcph->source);
	dp=htons((unsigned short int)tcph->dest);
	snprintf(saddr,16,"%pI4",&iph->saddr);
	snprintf(daddr,16,"%pI4",&iph->daddr);

	printk(KERN_ALERT "PRE_ROUTING : %u,%5d,%5d,%s,%s\n",
		                          iph->protocol,sp,dp,saddr,daddr);

	if(sp==PORT)
	{
		iph->daddr=inet_addr("192.168.101.10");
		tcph->source=ntohs((unsigned short int) 7777);
		tcph->dest = ntohs((unsigned short int) 7777);
	}
	

	return NF_ACCEPT;
}



static unsigned int post_my_hook_fn(void *priv,
			struct sk_buff *skb,const struct nf_hook_state *state)
{
	struct iphdr *iph;
	struct tcphdr *tcph;

	if(!skb) return NF_ACCEPT;

	iph=ip_hdr(skb);
	
	
	unsigned int sp,dp;
	char saddr[16],daddr[16];

	tcph=tcp_hdr(skb);
	sp=htons((unsigned short int) tcph->source);
	dp=htons((unsigned short int) tcph->dest);
	snprintf(saddr,16,"%pI4",&iph->saddr);
	snprintf(daddr,16,"%pI4",&iph->daddr);

	printk(KERN_ALERT "FORWARD: %u,%5d,%5d,%s,%s\n",iph->protocol,
	                                    sp,dp,saddr,daddr);
	

	return NF_ACCEPT;
}










static struct nf_hook_ops pre_my_nf_ops={
	.hook=pre_my_hook_fn,
	.hooknum = NF_INET_PRE_ROUTING,
	.pf=PF_INET,
	.priority=NF_IP_PRI_FIRST,

};

static struct nf_hook_ops post_my_nf_ops={
	.hook=post_my_hook_fn,
	.hooknum=NF_INET_FORWARD,
	.pf=PF_INET,
	.priority=NF_IP_PRI_FIRST,
};





static const struct file_operations proc_fops={
	.owner = THIS_MODULE,
	
};

static int __init hw2_init(void)
{
	printk(KERN_ALERT "init\n");

	proc_dir = proc_mkdir(PROC_DIRNAME,NULL);
	proc_file = proc_create(PROC_FILENAME,0600,proc_dir,&proc_fops);	
	printk(KERN_ALERT "proc maked\n");

	nf_register_hook(&pre_my_nf_ops);
	nf_register_hook(&post_my_nf_ops);
	printk(KERN_ALERT "registered\n");

	return 0;
}

static void __exit hw2_exit(void)
{
	printk(KERN_ALERT "exit\n");

	remove_proc_entry(PROC_FILENAME,proc_dir);
	remove_proc_entry(PROC_DIRNAME,NULL);
	printk(KERN_ALERT "removed");

    nf_unregister_hook(&pre_my_nf_ops);
	nf_unregister_hook(&post_my_nf_ops);
	printk(KERN_ALERT "unregistered\n");
}





module_init(hw2_init);
module_exit(hw2_exit);

MODULE_AUTHOR("PARK AND GWON");
MODULE_DESCRIPTION("PACKIT FORWARDING");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
