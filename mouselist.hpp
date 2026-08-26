#include <map>
#include <iostream>
#include <libusb-1.0/libusb.h>

static const int A4TECH_VID = 0x09da;

static const int BLOODY_A60_PID = 0x3e1d, BLOODY_J95S_PID = 0xfee3, BLOODY_RT5_PID = 0x7f1b, BLOODY_V8M_PID = 0x1094, 
	BLOODY_R8_PID = 0x7c10, BLOODY_V5_PID = 0x172A, BLOODY_V7_PID = 0xF613, BLOODY_V8_PID = 0x11F5, BLOODY_R7_PID = 0x1485,
	BLOODY_R8_1_PID = 0x14ee, BLOODY_R3_PID = 0x1a5a, BLOODY_AL9_PID = 0xf633, BLOODY_R70_PID = 0xf643, BLOODY_A7_PID = 0x7e36,
	BLOODY_A9_PID = 0x1003;

static const int COMPATIBLE_PIDS[] = { 
    BLOODY_A60_PID, BLOODY_J95S_PID, BLOODY_RT5_PID, BLOODY_V8M_PID, BLOODY_R8_PID,
	BLOODY_V5_PID, BLOODY_V7_PID, BLOODY_V8_PID, BLOODY_R7_PID, BLOODY_R8_1_PID,
	BLOODY_R3_PID, BLOODY_AL9_PID, BLOODY_R70_PID, BLOODY_A7_PID, BLOODY_A9_PID
};

std::string get_name(const int mouse_pid) {
	switch (mouse_pid) {
		case BLOODY_A60_PID:
		    return "Bloody A60";
        case BLOODY_V5_PID:
            return "Bloody V5";
        case BLOODY_V7_PID:
            return "Bloody V7";
        case BLOODY_V8_PID:
            return "Bloody V8";
        case BLOODY_R7_PID:
            return "Bloody R7";
        case BLOODY_R8_1_PID:
            return "Bloody R8-1";
        case BLOODY_R3_PID:
            return "Bloody R3";
        case BLOODY_AL9_PID:
            return "Bloody AL9";
        case BLOODY_R70_PID:
            return "Bloody R70";
        case BLOODY_A7_PID:
            return "Bloody A7";
        case BLOODY_A9_PID:
            return "Bloody A9";
        case BLOODY_RT5_PID:
            return "Bloody RT5";
        case BLOODY_V8M_PID:
            return "Bloody V8M";
        case BLOODY_R8_PID:
            return "Bloody R8";
        case BLOODY_J95S_PID:
            return "Bloody J95S";
        default:
            return "Unknown";
        }
}

static const size_t COMPATIBLE_PIDS_SIZE = sizeof(COMPATIBLE_PIDS) / sizeof(COMPATIBLE_PIDS[0]);

static const int A4TECH_MAGIC = 0x07;

static const int BACKLIGHT_OPCODE = 0x11, BACKLIGHT_WRITE = 0x80, BACKLIGHT_READ = 0x00;

#include "mouseclass.hpp"
