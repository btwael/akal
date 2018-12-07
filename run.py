from os import system

system("qemu-system-aarch64 -M raspi3 -kernel ./build/kernel8.img -serial stdio -serial null")
