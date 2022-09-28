#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/interrupts.h"
#include "drivers/driver.h"

namespace wyoos
{
    namespace hardwarecommunication
    {
        enum BaseAddressRegisterType {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister {
        public:
            bool prefetchable;
            wyoos::common::uint8_t* address;
            wyoos::common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PerippheraComponentInterconnectDeviceDescriptor {
        public:
            PerippheraComponentInterconnectDeviceDescriptor();
            ~PerippheraComponentInterconnectDeviceDescriptor();

            wyoos::common::uint32_t portBase;
            wyoos::common::uint32_t interrupt;

            wyoos::common::uint8_t bus;
            wyoos::common::uint8_t device;
            wyoos::common::uint8_t function;
            
            wyoos::common::uint16_t device_id;
            wyoos::common::uint16_t vendor_id;

            wyoos::common::uint8_t class_id;
            wyoos::common::uint8_t subclass_id;
            wyoos::common::uint8_t interface_id;
            wyoos::common::uint8_t revision;
        };

        class PerippheraComponentInterconnectController {
        public:
            PerippheraComponentInterconnectController();
            ~PerippheraComponentInterconnectController();

            wyoos::common::uint32_t Read(wyoos::common::uint8_t bus, 
                                        wyoos::common::uint8_t device, 
                                        wyoos::common::uint8_t function,
                                        wyoos::common::uint8_t registeroffset);

            void Write(wyoos::common::uint8_t bus, 
                                        wyoos::common::uint8_t device, 
                                        wyoos::common::uint8_t function,
                                        wyoos::common::uint8_t registeroffset,
                                        wyoos::common::uint32_t value);

            bool DeviceHasFunction(wyoos::common::uint8_t bus, wyoos::common::uint8_t device);

            void SelectDrivers(wyoos::drivers::DriverManager* driverManager, wyoos::hardwarecommunication::InterruptManager* interrupts);

            wyoos::drivers::Driver* GetDriver(PerippheraComponentInterconnectDeviceDescriptor dev, wyoos::hardwarecommunication::InterruptManager* interrupts);

            PerippheraComponentInterconnectDeviceDescriptor GetDeviceDescriptor(wyoos::common::uint8_t bus, 
                                                                                wyoos::common::uint8_t device, 
                                                                                wyoos::common::uint8_t function);

            BaseAddressRegister GetBaseAddressRegister(wyoos::common::uint8_t bus, 
                                                        wyoos::common::uint8_t device, 
                                                        wyoos::common::uint8_t function,
                                                        wyoos::common::uint8_t bar);
        private:
            Port32Bit dataPort;
            Port32Bit commonPort;
        };
    }
}



#endif