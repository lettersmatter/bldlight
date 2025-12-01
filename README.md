This utility is fork of maxmati/a4tech-bloody-linux-driver. Goal of it is personal needs(have pet project, add support of author's mouse), clear code base(move project to C, update using instruments) and improve UX(in perfect, add test auto support of all potential supported bloody mouse). in fact after this improvments utility will compile and launch without additional settings on much mouses

This tool allows setting backlight level of A4TECH wired mouse (USB connected), e.g. A4TECH V7M, A60 etc.

# Build instructions
## Fedora 29
```
dnf install gcc-c++ cmake libusbx-devel
git clone https://github.com/maxmati/a4tech-bloody-linux-driver
cd ./a4tech-bloody-linux-driver
cmake .
make
```

## Ubuntu 24.04
```
sudo apt install g++ cmake libusb-1.0-0-dev pkg-config
git clone https://github.com/lettersmatter/bldlight
cd ./bldlight
cmake .
make
```

## Add new bloody devices

1. Get USB PID of device:
```
lsusb -d 09da: | cut -d ' ' -f6 | cut -d ':' -f2
```

2. Add that PID to Mouce.h & Mouce.cpp

Mouse.h: add line with PID and add name to COMPATIBLE_PIDS variable as all the other mouses.

Mouse.cpp: add 3 lines: case, name and break; as all the other mouses.


## Run example
```
sudo ./bloody
Available devices:
36:Bloody V7
Enter device address: 36
Current backlight level: 3
Select backlight level(0-3):0
```
