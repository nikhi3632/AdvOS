OUTPUT_ARCH( "riscv" )

ENTRY( _entry )

SECTIONS
{
  /* CSE 536: (Task 2.2) Ensure the correct entry address based on where -kernel in QEMU jumps. */
  /* CSE 536: (Task 2.2) Keep track of each program section's ending address. */
  /* CSE 536: (Task 2.2) Keep track of the binary's ending address. */
  /* Section has multiple segments. */

  . = 0x80000000;

  .text : {
    *(.text)
  }
  PROVIDE(ecode = .);

  .data : {
    *(.data)
  }
  PROVIDE(edata = .);

  .rodata : {
    *(.rodata)
  }
  PROVIDE(erodata = .);

  .bss : {
    *(.bss)
  }
  PROVIDE(ebss = .);

  PROVIDE(end = .);
}