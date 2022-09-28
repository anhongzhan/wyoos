#include "hardwarecommunication/pci.h"

using namespace wyoos::common;
using namespace wyoos::hardwarecommunication;
using namespace wyoos::drivers;

PerippheraComponentInterconnectDeviceDescriptor::PerippheraComponentInterconnectDeviceDescriptor() {}

PerippheraComponentInterconnectDeviceDescriptor::~PerippheraComponentInterconnectDeviceDescriptor() {}

PerippheraComponentInterconnectController::PerippheraComponentInterconnectController()
    : dataPort(0xcfc),
    commonPort(0xcf8){}

PerippheraComponentInterconnectController::~PerippheraComponentInterconnectController() {}

wyoos::common::uint32_t PerippheraComponentInterconnectController::Read(uint8_t bus, 
                             uint8_t device, 
                             uint8_t function,
                             uint8_t registeroffset)
{
    uint32_t id = 
        (1 << 31) |
        ((bus & 0xff) << 16) | 
        ((device & 0x1f) << 11) |
        ((function & 0x07) << 8) | 
        (registeroffset & 0xfc);
    commonPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8 * (registeroffset % 4));
}

void PerippheraComponentInterconnectController::Write(uint8_t bus, 
           uint8_t device, 
           uint8_t function,
           uint8_t registeroffset,
           uint32_t value)
{
     uint32_t id = 
        (1 << 31) |
        ((bus & 0xff) << 16) | 
        ((device & 0x1f) << 11) |
        ((function & 0x07) << 8) | 
        (registeroffset & 0xfc);
    commonPort.Write(id);
    dataPort.Write(value);
}

bool PerippheraComponentInterconnectController::DeviceHasFunction(uint8_t bus, uint8_t device){
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void printf(const char*);
void printfHex(uint8_t);

void PerippheraComponentInterconnectController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts){
    for (uint16_t bus = 0; bus < 256; bus++){
        for (uint8_t device = 0; device < 32; device++){
            int numFunction = DeviceHasFunction((uint8_t)bus, device) ? 8 : 1;
            for (uint8_t function = 0; function < numFunction; function++){
                PerippheraComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
                if(dev.vendor_id == 0 || dev.vendor_id == 0xffff) break;
                printf("PCI BUS ");
                printfHex(bus & 0xff);

                printf(", DEVICE ");
                printfHex(device);

                printf(", FUNCTION ");
                printfHex(function);

                printf(" = VENDOR ");
                printfHex((dev.vendor_id & 0xff00) >> 8);
                printfHex(dev.vendor_id & 0xff);

                printf(", DEVICE ");
                printfHex((dev.device_id & 0xff00) >> 8);
                printfHex(dev.device_id & 0xff);

                printf("\n");

                for(uint8_t barNum = 0; barNum < 6; barNum++){
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if (bar.address && (bar.type == InputOutput)){
                        dev.portBase = (uint32_t)bar.address;
                    }

                    Driver* driver = GetDriver(dev, interrupts);
                    if (driver != 0){
                        driverManager->AddDriver(driver);
                    }
                }
            }
        }
    }
}

Driver* PerippheraComponentInterconnectController::GetDriver(PerippheraComponentInterconnectDeviceDescriptor dev, InterruptManager* interrupts){
    switch (dev.vendor_id){
        case 0x1022: // AMD
            switch (dev.device_id)
            {
                case 0x2000:
                    printf("amd");
                    break;
            }
            break;
        case 0x8086: // intel
            break;
    }

    switch (dev.class_id)
    {
    case 0x03:
        switch (dev.subclass_id) {
            case 0x00: // VGA
                printf("VGA");
                break;
        }
        break;
    }

    return 0;
}

BaseAddressRegister PerippheraComponentInterconnectController::GetBaseAddressRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t bar){
    BaseAddressRegister result;
    
    uint32_t headertype = Read(bus, device, function, 0x0e) & 0x7e;
    int maxBARs = 6 - 4 * headertype;
    if (bar >= maxBARs) return result;

    uint32_t bar_value = Read(bus, device, function, 0x10 + 4 * bar);
    result.type = (bar_value & 0x01) ? InputOutput : MemoryMapping;

    if (result.type == MemoryMapping) {
        switch ((bar_value >> 1) & 0x3) {
            case 0: //32
            case 1: //20
            case 2: //64
            break;
        }
    } else {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
}

PerippheraComponentInterconnectDeviceDescriptor PerippheraComponentInterconnectController::GetDeviceDescriptor(uint8_t bus, uint8_t device, uint8_t function){
    PerippheraComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);
    result.revision = Read(bus, device, function, 0x08);

    result.interrupt = Read(bus, device, function, 0x3c);
    return result;
}