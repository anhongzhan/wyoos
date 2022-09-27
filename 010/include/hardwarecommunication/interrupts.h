#ifndef __WYOOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __WYOOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "gdt.h"

namespace wyoos
{
    namespace hardwarecommunication
    {
        class InterruptManager;

        class InterruptHandler {
        protected:
            InterruptHandler(wyoos::common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();

            wyoos::common::uint8_t interruptNumber;
            InterruptManager* interruptManager;

        public:
            virtual wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint32_t esp);
        };

        class InterruptManager {
            friend class InterruptHandler;
        public:
            InterruptManager(GlobalDescriptorTable* gdt, wyoos::common::uint16_t hardwareInterruptOffset);
            ~InterruptManager();

            wyoos::common::uint16_t getHardwareInterruptOffset();

            void Activate();
            void Deactivate();

        protected:
            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];

            struct GateDescriptor {
                wyoos::common::uint16_t handleAddressLowBits;
                wyoos::common::uint16_t gdt_codeSegementSelector;
                wyoos::common::uint8_t reserved;
                wyoos::common::uint8_t access;
                wyoos::common::uint16_t handleAddressHighBits;
            }__attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptorDescriptorTablePointer{
                wyoos::common::uint16_t size;
                wyoos::common::uint32_t base;
            }__attribute__((packed));

            static void SetInterruptDescriptorTableEntry (
                wyoos::common::uint8_t interruptNumber,
                wyoos::common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                wyoos::common::uint8_t DescriptorPrivilegeLevel,
                wyoos::common::uint8_t DescriptorType
            );

            wyoos::common::uint16_t hardwareInterruptOffset;

            static void InterruptIgnore();

            static wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint8_t interruptNumber, wyoos::common::uint32_t esp);
            wyoos::common::uint32_t DoHandleInterrupt(wyoos::common::uint8_t interruptNumber, wyoos::common::uint32_t esp);

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;
        };
    }
}



#endif