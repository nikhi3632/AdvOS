# CSE 536: Assignment - A Tale of a Boot ROM and a Bootloader

Please refer to the assignment handout posted on Canvas for a list of tasks.

### A. Installing xv6 pre-requisites (OPTIONAL)

Please skip this step if you've already completed assignment #0. Reach out to the TAs if you have any installation issues or use the Apporto Cloud VMs.

#### Linux/WSL

1. Navigate to the install/linux-wsl folder
2. Install RISC-V QEMU: `./linux-qemu.sh`
3. Install RISC-V toolchain:`./linux-toolchain.sh`
4. Add installed binaries to path: `source .add-linux-paths`

#### MacOS

1. Navigate to the install/mac folder
2. Install RISC-V QEMU: `./mac-qemu.sh`
3. Install RISC-V toolchain: `./mac-toolchain.sh`
4. Add installed binaries to path: `source .add-mac-paths`

### B. Running the Bootloader

Navigate to main folder and run `make qemu`

### C. FAQs

1. While running linux-qemu.sh, if you run into `ERROR: glib-2.48 gthread-2.0 is required to compile QEMU`, then :

- Run this command in the terminal.  `sudo apt install libglib2.0-dev`. This is caused due to a newer version of your linux distro([link](https://github.com/Xilinx/qemu/issues/40)). 

2. linux-qemu.sh : `../meson.build:328:2: ERROR: Dependency "pixman-1" not found, tried pkgconfig`

- Can be resolved by running `sudo apt install libpixman-1-dev` ([link](https://stackoverflow.com/a/39916441))

### Acknowledgement

We are using the xv6 Operating System for programming assignments. xv6 is a teaching-focused OS designed by some incredible folks at MIT ([link](https://github.com/mit-pdos/xv6-riscv.git)). This README explains how to setup QEMU and a GNU RISC-V toolchain, needed for running xv6, as well how to boot up a QEMU VM with xv6.