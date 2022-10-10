#ifndef __WYOOS__DRIVERS__AMD_AM79C973_H
#define __WYOOS__DRIVERS__AMD_AM79C973_H

#include "common/types.h"
#include "drivers/driver.h"
#include "hardwarecommunication/pci.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"

namespace wyoos
{
    namespace drivers
    {
        class amd_am79c973 : public Driver, public hardwarecommunication::InterruptHandler {
        public:
            amd_am79c973(wyoos::hardwarecommunication::PerippheraComponentInterconnectDeviceDescriptor* dev,
                        wyoos::hardwarecommunication::InterruptManager* interrupts);
            ~amd_am79c973();

            void Activate();
            int Reset();
            common::uint32_t HandleInterrupt(common::uint32_t esp);

            void Send(common::uint8_t* buffer, int size);
            void Receive();
        
        private:
            struct InitializationBlock
            {
                common::uint16_t mode;
                unsigned reserved1 : 4;
                unsigned numSendBuffers : 4;
                unsigned reserved2 : 4;
                unsigned numRecvBuffers : 4;
                common::uint64_t physicalAddress : 48;
                common::uint16_t reserved3;
                common::uint64_t logicalAddress;
                common::uint32_t recvBufferDescAddress;
                common::uint32_t sendBufferDescAddress;
            } __attribute__((packed));
            
            
            struct BufferDescriptor
            {
                common::uint32_t address;
                common::uint32_t flags;
                common::uint32_t flags2;
                common::uint32_t avail;
            } __attribute__((packed));

            hardwarecommunication::Port16Bit MACAddress0Port;
            hardwarecommunication::Port16Bit MACAddress2Port;
            hardwarecommunication::Port16Bit MACAddress4Port;
            hardwarecommunication::Port16Bit registerDataPort;
            hardwarecommunication::Port16Bit registerAddressPort;
            hardwarecommunication::Port16Bit resetPort;
            hardwarecommunication::Port16Bit busControlRegisterDataPort;

            InitializationBlock initBlock;
            BufferDescriptor* sendBufferDesc;
            common::uint8_t sendBufferDescMemory[2048 + 15];

            common::uint8_t sendBuffers[2048 + 15][8];
            common::uint8_t currentSendBuffer;

            BufferDescriptor* recvBufferDesc;
            common::uint8_t recvBufferDescMemory[2048 + 15];
            common::uint8_t recvBuffers[2048 + 15][8];
            common::uint8_t currentRecvBuffer;  
        };
    }
}

#endif