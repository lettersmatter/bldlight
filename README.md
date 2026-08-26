This utility is fork of maxmati/a4tech-bloody-linux-driver. Goal of it is personal needs(have pet project, add support of author's mouse), clear code base(move project to C, compiling using only with make) and improve UX(classic use style like utility managed by arguments, without capturing input). Maybe add setting sensivity

This tool allows setting backlight level of A4TECH wired mouse (USB connected), e.g. A4TECH V7M, A60 etc.

# Build instructions

## Ubuntu 24.04
```
sudo apt install g++ make libusb-1.0-0-dev
git clone https://github.com/lettersmatter/bldlight
cd ./bldlight
make
```

## Some commands for debug

Get USB PID of device:
```
lsusb -d 09da: | cut -d ' ' -f6 | cut -d ':' -f2
```

## Edits to add another mouse

Add PID to mouselist.hpp

add line with PID and add name to COMPATIBLE_PIDS variable as all the other mouses.

add 3 lines: case, name and break; as all the other mouses.


## Run example
```
sudo ./bldlight

Available devices:
36:Bloody V7
Enter device address: 36
Current backlight level: 3
Select backlight level(0-3):0
```
