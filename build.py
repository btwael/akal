from os import system
from poyo import parse_string, PoyoException


def compileCpp(input, output, define_list):
    l = "clang -target aarch64-none-elf -Isystem -Isystem/include -std=c++11 -Wall -O2 -nostdlib -nostdinc -nostdinc++ -ffreestanding -c "
    l = l + input + " -o " + output
    for define in define_list:
        l += " -D" + define
    system(l)


requirement_map = {
    "uart": "AKAL_TARGET_REQUIRE_UART"
}
pp_define = []
yamlstring = open("application/application.yaml", "r").read()
config = parse_string(yamlstring)
for requirement in config["require"]:
    if requirement in requirement_map:
        pp_define.append(requirement_map[requirement])

if config["target"].startswith("rpi3"):
    pp_define.append("AKAL_APPLICATION_TARGET_RPI")
    if config["target"].endswith("32"):
        pp_define.append("AKAL_APPLICATION_TARGET_32BIT")
    elif config["target"].endswith("64"):
        pp_define.append("AKAL_APPLICATION_TARGET_64BIT")

compileCpp("./application/main.cc", "./build/main.o", pp_define)
system(
    "clang -target aarch64-none-elf -Wall -ffreestanding -nostdinc -nostdlib -c ./system/src/board/rpi3/startup.S -o ./build/startup.o")
system(
    "aarch64-none-elf-ld -nostdlib -nostartfiles ./build/startup.o ./build/main.o -T ./system/src/board/rpi3/link.ld -o ./build/kernel8.elf")
system("aarch64-none-elf-objcopy -O binary ./build/kernel8.elf ./build/kernel8.img")
system("qemu-system-aarch64 -M raspi3 -kernel ./build/kernel8.img -serial null -serial stdio")
