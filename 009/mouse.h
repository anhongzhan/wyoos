#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

class MouseDriver : public InterruptHandler, public Driver{
public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();

private:
    Port8Bit dataport;
    Port8Bit commandport;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    int8_t x, y;
};

#endif