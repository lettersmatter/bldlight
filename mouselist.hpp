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

class Mouse {
public:
    ~Mouse();
    void init();
    void listDevices();
    bool selectDevice(int address);
    int setBackLightLevel(uint8_t level);
    uint8_t getBackLightLevel();
private:
    std::map<int, libusb_device_handle*> devices;
    libusb_device_handle* currentDevice = nullptr;
    libusb_context* context = nullptr;

    int writeToMouse(uint8_t data[], size_t size);
    int readFromMouse(uint8_t* request, size_t requestSize, uint8_t* response, size_t responseSize);
    void discoverDevices();
    bool isCompatibleDevice(libusb_device_descriptor &desc);
};

using std::cout;
using std::endl;

void Mouse::init() {
    int ret = libusb_init(&context);
    if(ret < 0) {
        cout << "Init Error " << ret << endl;
        return;
    }
    libusb_set_debug(context, LIBUSB_LOG_LEVEL_INFO);
    discoverDevices();
}

void Mouse::discoverDevices() {
    libusb_device** devs;
    ssize_t cnt = libusb_get_device_list(context, &devs);
    if(cnt < 0) {
        cout << "Get Device Error" << endl;
        return;
    }
    for (int i = 0; i < cnt; ++i) {
        libusb_device_descriptor desc;
        libusb_get_device_descriptor(devs[i], &desc);
        if (isCompatibleDevice(desc)) {
            switch (int status = libusb_open(devs[i], &currentDevice)) {
                case 0:
                    break;
                case LIBUSB_ERROR_NO_MEM:
                    cout << "LIBUSB_ERROR_NO_MEM" << endl;
                    continue;
                case LIBUSB_ERROR_ACCESS:
                    cout << "LIBUSB_ERROR_ACCESS" << endl;
                    continue;
                case LIBUSB_ERROR_NO_DEVICE:
                    cout << "LIBUSB_ERROR_NO_DEVICE" << endl;
                    continue;
                default:
                    cout << "Status: " << status << endl;
                    continue;
            }
            if(libusb_kernel_driver_active(currentDevice, 2) == 1 && libusb_detach_kernel_driver(currentDevice, 2) != 0) {
                libusb_close(currentDevice);
                continue;
            }
            devices.insert(std::pair<int, libusb_device_handle*>(libusb_get_device_address(devs[i]), currentDevice));
        }
    }
    libusb_free_device_list(devs, 1);
    if (devices.size() == 0){
        cout << "No suitable device found. " << endl;
        return;
    }
    currentDevice = devices.begin()->second;
}

bool Mouse::isCompatibleDevice(libusb_device_descriptor &desc) {
    if(desc.idVendor != A4TECH_VID) return false;
    for(size_t i = 0; i < COMPATIBLE_PIDS_SIZE; ++i)
        if(desc.idProduct == COMPATIBLE_PIDS[i]) return true;
    return false;
}

Mouse::~Mouse() {
    for (auto& dev : devices) libusb_close(dev.second);
    if (context != nullptr) libusb_exit(context);
}

int Mouse::setBackLightLevel(uint8_t level) {
    uint8_t data[72] = { A4TECH_MAGIC, BACKLIGHT_OPCODE, 0x00, 0x00, BACKLIGHT_WRITE, 0x00, 0x00, 0x00, level, 0x00 };
    if (level < 0 || level > 3) return -1;
    if (writeToMouse(data, sizeof(data)) < 0) return -2;
    return 0;
}

int Mouse::writeToMouse(uint8_t data[], size_t size) {
    int res = libusb_control_transfer(currentDevice, 0x21, 9, 0x0307, 2, data, size, 10000);
    switch (res){
        case LIBUSB_ERROR_TIMEOUT:
            cout << "LIBUSB_ERROR_TIMEOUT" << endl;
            return -1;
        case LIBUSB_ERROR_PIPE:
            cout << "LIBUSB_ERROR_PIPE" << endl;
            return -1;
        case LIBUSB_ERROR_NO_DEVICE:
            cout << "LIBUSB_ERROR_NO_DEVICE" << endl;
            return -1;
        case LIBUSB_ERROR_BUSY:
            cout << "LIBUSB_ERROR_BUSY" << endl;
            return -1;
        case LIBUSB_ERROR_INVALID_PARAM:
            cout << "LIBUSB_ERROR_INVALID_PARAM" << endl;
        default:
            return 0;
    }
}

int Mouse::readFromMouse(uint8_t *request, size_t requestSize, uint8_t *response, size_t responseSize) {
    if (writeToMouse(request, requestSize) < 0) return -1;
    int res =  libusb_control_transfer(currentDevice, 0xa1, 1, 0x0307, 2, response, responseSize, 10000);
    if (res < 0) {
        cout << "Unnable to receive data" << endl;
        return -2;
    }
	return 0;
}

uint8_t Mouse::getBackLightLevel() {
    uint8_t request[72] = { A4TECH_MAGIC, BACKLIGHT_OPCODE, 0x00, 0x00, BACKLIGHT_READ, 0x00 };
    uint8_t response[72];
    readFromMouse(request, sizeof(request), response, sizeof(response));
    return response[8];
}

void Mouse::listDevices() {
    std::cout << "Available devices: " << endl;
    for (auto&devHand : devices) {
        libusb_device* device = libusb_get_device(devHand.second);
        libusb_device_descriptor desc;
        libusb_get_device_descriptor(device, &desc);
        std::cout << devHand.first << ": " << /*name*/get_name(desc.idProduct) << endl;
    }
}

bool Mouse::selectDevice(int address) {
    if (devices.count(address) == 0) return false;
    currentDevice = devices.at(address);
    return true;
}
