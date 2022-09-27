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

            void SelectDrivers(wyoos::drivers::DriverManager* driverManager);

            PerippheraComponentInterconnectDeviceDescriptor GetDeviceDescriptor(wyoos::common::uint8_t bus, 
                                                                                wyoos::common::uint8_t device, 
                                                                                wyoos::common::uint8_t function);
        private:
            Port32Bit dataPort;
            Port32Bit commonPort;
        };
    }
}



#endif