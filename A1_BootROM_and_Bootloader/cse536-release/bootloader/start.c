/* These files have been taken from the open-source xv6 Operating System codebase (MIT License).  */
/* Modifications made by Adil Ahmad. */

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "buf.h"

#define SYSINFOADDR 0x80080000

void main();
void timerinit();

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char bl_stack[STSIZE * NCPU];

// Task: Collect and store hardware information
struct sys_info {
  // HW info collected from CPU registers
  uint64 vendor;
  uint64 arch;
  uint64 impl;

  // Bootloader binary addresses
  uint64 bl_start;
  uint64 bl_end;

  // Accessible DRAM addresses
  uint64 dr_start;
  uint64 dr_end;
};
struct sys_info* sys_info_ptr;

extern void _entry(void);

// entry.S jumps here in machine mode on stack0.
void
start()
{
  // keep each CPU's hartid in its tp register, for cpuid().
  int id = r_mhartid();
  w_tp(id);

  // set M Previous Privilege mode to Supervisor, for mret.
  unsigned long x = r_mstatus();
  x &= ~MSTATUS_MPP_MASK;
  x |= MSTATUS_MPP_S;
  w_mstatus(x);

  // disable paging for now.
  w_satp(0);

  // delegate all interrupts and exceptions to supervisor mode.
  w_medeleg(0xffff);
  w_mideleg(0xffff);
  w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

  // CSE 536: Task 2.4
  //  Enable R/W/X access to all parts of the address space,[2-56] 
  //  except for the upper 10 MB (0 - 117 MB) using PMP, TOR => A = 1
  w_pmpaddr0(0x87500000 >> 2);
  w_pmpcfg0(0xf);

  // CSE 536: Task 2.5
  // Load the kernel binary to its correct location
  uint64 kernel_entry_addr = 0;
  uint64 kernel_load_addr  = 0;
  uint64 kernel_size       = 0;

  // CSE 536: Task 2.5.1
  // Find the loading address of the kernel binary
  kernel_load_addr  = find_kernel_load_addr();

  // CSE 536: Task 2.5.2
  // Find the kernel binary size and copy it to the load address
  kernel_size       = find_kernel_size();
  memmove((void*)kernel_load_addr, (const void*)(RAMDISK + 4096), kernel_size - 4096); //4KB

  // CSE 536: Task 2.5.3
  // Find the entry address and write it to mepc
  kernel_entry_addr = find_kernel_entry_addr();
  w_mepc(kernel_entry_addr);

  // CSE 536: Task 2.6
  // Provide system information to the kernel
  sys_info_ptr = (struct sys_info* )SYSINFOADDR;
  sys_info_ptr->vendor = r_vendor();
  sys_info_ptr->arch = r_architecture();
  sys_info_ptr->impl = r_implementation();
  sys_info_ptr->bl_start = 0x80000000;
  sys_info_ptr->bl_end = (long unsigned int)&end;
  sys_info_ptr->dr_start = 0x80000000;
  sys_info_ptr->dr_end = 0x88000000; //dr_start + 128MB
  
  // CSE 536: Task 2.5.3
  // Jump to the OS kernel code
  // switch to supervisor mode and jump to main().
  asm volatile("mret");
}