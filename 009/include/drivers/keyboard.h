#ifndef __WYOOS__DRIVERS__KEYBOARD_H
#define __WYOOS__DRIVERS__KEYBOARD_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace wyoos
{
    namespace drivers
    {
        class KeyboardEventHandler {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyBoardDriver : public wyoos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            KeyBoardDriver(wyoos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyBoardDriver();
            virtual wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint32_t esp);
            virtual void Activate();

        private:
            wyoos::hardwarecommunication::Port8Bit dataport;
            wyoos::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler* handler;
        };
    }
}

#endif