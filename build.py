from os import system
from poyo import parse_string, PoyoException


def compileCpp(input, output, define_list):
    l = "aarch64-elf-g++ -Isystem -Isystem/include -O2 -march=armv8-a -mtune=cortex-a53 -mlittle-endian -mcmodel=small -Wall -Wno-strict-aliasing -fno-builtin -nostdinc -nostdlib -fno-exceptions -fno-rtti -std=c++11 -c "
    l = l + input + " -o " + output
    for define in define_list:
        l += " -D" + define
    system(l)


requirement_map = {
    "uart": "AKAL_APPLICATION_TARGET_REQUIRE_UART"
}
pp_define = []
yamlstring = open("application/application.yaml", "r").read()
config = parse_string(yamlstring)
for requirement in config["require"]:
    if requirement in requirement_map:
        pp_define.append(requirement_map[requirement])

if config["target"].startswith("rpi3"):
    pp_define.append("AKAL_APPLICATION_TARGET_RPI3")
    if config["target"].endswith("32"):
        pp_define.append("AKAL_APPLICATION_TARGET_32BIT")
    elif config["target"].endswith("64"):
        pp_define.append("AKAL_APPLICATION_TARGET_64BIT")

compileCpp("./application/main.cc", "./build/main.o", pp_define)
system(
    "aarch64-elf-g++ -Wall -ffreestanding -nostdinc -nostdlib -c ./system/src/board/rpi3/startup.S -o ./build/startup.o")
system(
    "aarch64-elf-g++ -Wall -ffreestanding -nostdinc -nostdlib -c ./system/src/board/rpi3/entry.S -o ./build/entry.o")
system(
    "aarch64-elf-ld -r -b binary -o ./build/font.o system/resources/font.psf")
system(
    "aarch64-elf-ld -nostdlib -nostartfiles ./build/startup.o ./build/entry.o ./build/main.o ./build/font.o -T ./system/src/board/rpi3/link.ld -o ./build/kernel8.elf")
system("aarch64-elf-objcopy -O binary ./build/kernel8.elf ./build/kernel8.img")
