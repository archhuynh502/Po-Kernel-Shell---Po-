check arch-poshell or whatever i lowk forget for the shell utilities.
remember : this is beta testing : things might not work im still debugging
Po Shell 6th definitive Release
1-4 was simple tinker toys i made in batch
5 got corrupted
6th should be the first C-made release that works
written in assembly and C mainly

Requirements : Linux distrobution or WSL on host device (10+)
to build the ISO you need gcc, nasm, ld, make, xorriso
how to install on each package manger :
debian/ubuntu based : sudo apt install gcc nasm ld make xorriso
arch based : sudo pacman -Sy gcc nasm ld make xorriso
fedora based : sudo dnf install gcc nasm ld make xorriso
flatpak : sudo flatpak install gcc nasm ld make xorriso
i do not have limine in here so you will have to install it manually, sorry!
advisory : place limine in a folder just like kernel or shell, in the root directory. 

so heres the structure for the OS 
[the standard files]
Makefile                         
boot.o
kernel.o
po_os.elf
limine.cfg
linker.ld
boot.asm
[ kernel ] 
process.c
timer.s
kernel.c
process.h
memory.c
memory.h
syscall.c
syscall.h
timer.c
devices.c
devices.h
fs.c
fs.h
context.S
idt.h
idt.c
types.h
[ shell ] 
utils.c
utils.h
shell.c
history.c
history.h
shell.h
parser.c
parser.h
commands.c
commands.h
------- section 2 ---- [ How To Install Po Kernel ] --------------
WINDOWS
the first thing is make a folder, can be called anything aslong you know the commands for it, i reccomend PoShellV6 or whatever, idc
now make two directories : one shell and one kernel, place the standard fiels in the root (PoShellV6), place kernel files in kernel, and shell in shell.
Place your limine folder into the poshellv6 folder aswell, and then make sure you can run bootstrap via "chmod +X ./bootstrap"
then run ./bootstrap
once you ran bootstrap run make and build the kernel.
once you have po_os.elf
run this
xorriso -as mkisofs \
  -b boot/limine.sys \
  -no-emul-boot \
  -boot-load-size 4 \
  -boot-info-table \
  -o po_os.iso \
  iso_root/
once you have ran that, you can use QEMU or virtualbox to run the kernel. 
beta testing : definitely most likely will fail :) 
