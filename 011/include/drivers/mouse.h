#ifndef __WYOOS__DRIVERS__MOUSE_H
#define __WYOOS__DRIVERS__MOUSE_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace wyoos
{
    namespace drivers
    {
        class MouseEventHandler {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(wyoos::common::int8_t);
            virtual void OnMouseUp(wyoos::common::int8_t);
            virtual void OnMouseMove(wyoos::common::int8_t x, wyoos::common::int8_t y);
        private:
            wyoos::common::int8_t x, y;
        };

        class MouseDriver : public wyoos::hardwarecommunication::InterruptHandler, public Driver{
        public:
            MouseDriver(wyoos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint32_t esp);
            virtual void Activate();

        private:
            wyoos::hardwarecommunication::Port8Bit dataport;
            wyoos::hardwarecommunication::Port8Bit commandport;

            wyoos::common::uint8_t buffer[3];
            wyoos::common::uint8_t offset;
            wyoos::common::uint8_t buttons;

            MouseEventHandler* handler;
        };
    }
}

#endif