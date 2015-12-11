#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "/usr/src/linux-headers-3.4.108-bananian/arch/arm/include/asm/cacheflush.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lakshmanan");
MODULE_DESCRIPTION("A Simple Hello World module");

const size_t mem_size = 1 << 20;
char g_mem[1 << 20];
int j,k;
int sum = 0;
int toggle = 540000;
/////////////////////rand() generator///////////////////////////////////
unsigned int t1 = 0, t2 = 0;
long start, end;
static unsigned int rand(void)
{
    unsigned int b;
    b = t1 ^ (t1 >> 2) ^ (t1 >> 6) ^ (t1 >> 7);
    t1 = (t1 >> 1) | (~b << 31);

    b = (t2 << 1) ^ (t2 << 2) ^ (t1 << 3) ^ (t2 << 4);
    t2 = (t2 << 1) | (~b >> 31);
      return t1 ^ t2;
}

static char* pick_addr(void){
    size_t offset = (rand() << 12)% mem_size;
    return (char *) g_mem + offset;
}
static int __init hello_init(void)
{
    int i;
    uint32_t *addrs[10];
    
    for (i = 0; i < mem_size; ++i){
        g_mem[i] = 0xff;
    }
    for(i = 0; i < 10; i++){
        addrs[i] = (uint32_t*) pick_addr();
    }
    start = rdtsc();
    for(i = 0; i < toggle; i++){
        printk(KERN_INFO "In round i %d, flush cache vmap completes",i);
        for(j = 0; j < 10; j++)
            sum = sum + *addrs[j];
        for(k = 0; k < 10; k++){
            flush_cache_vmap(*addrs[k], *addrs[k]+1);
         }
    }
    end = rdtsc();
    printk(KERN_INFO "Toggle 540000 times\n, using %l ticks", end - start);
    for(i = 0; i < mem_size; ++i){
        if (g_mem[i] != 0xff){
            printk(KERN_INFO "bit flips\n");
        }
        
    }
    return 0;
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
