This tool allows setting backlight level of A4TECH wired mouse (USB connected), e.g. A4TECH V7M, A60 etc.

bldlight is fork of maxmati/a4tech-bloody-linux-driver. Goals of it are personal needs(have pet project, add support of author's mouse), clear code base and improve UX.

Differences by original repository:
1. compiling with only make(without CMake)
2. add support of Bloody A60

Planned improves:
1. migrating to C
2. add install script and using without sudo
3. realize classic use style with arguments, without capturing input
4. add setting sensitivity

Problems:
1. settings are cleaning after reconnecting mouse

## Ubuntu 24.04 requirements

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

Add PID to mouselist.hpp:
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
