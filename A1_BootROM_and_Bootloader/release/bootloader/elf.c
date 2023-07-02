#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "buf.h"
#include "elf.h"

#include <stdbool.h>

struct elfhdr* kernel_elfhdr = (struct elfhdr*)RAMDISK;
struct proghdr* kernel_phdr;

uint64
find_kernel_load_addr(void)
{
    // CSE 536: task 2.5.1
    kernel_phdr = (struct proghdr*)(RAMDISK + kernel_elfhdr->phoff + kernel_elfhdr->phentsize);
    return kernel_phdr->vaddr;
}

uint64
find_kernel_size(void)
{
    // CSE 536: task 2.5.2
    // For kernel1 272184 + (18*64) = 273336(filesystem size verified) which is
    // kernel_elfhdr->shoff + (kernel_elfhdr->shnum * kernel_elfhdr->shentsize)
    return kernel_elfhdr->shoff + (kernel_elfhdr->shnum * kernel_elfhdr->shentsize);
}

uint64
find_kernel_entry_addr(void)
{
    // CSE 536: task 2.5.3
    return kernel_elfhdr->entry;
}