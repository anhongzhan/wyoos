#include "common/types.h"
#include "gdt.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/pci.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/driver.h"
#include "drivers/vga.h"
#include "gui/desktop.h"
#include "gui/widget.h"
#include "gui/window.h"


using namespace wyoos;
using namespace wyoos::common;
using namespace wyoos::drivers;
using namespace wyoos::hardwarecommunication;
using namespace wyoos::gui;

#define GRAPHICMODE

void printf(const char* str){
    static uint16_t *VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;
    for(int32_t i=0;str[i];i++){
        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;
        
        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }

        if(x >= 80){
            y++;
            x = 0;
            if(y >= 25){
                for(y = 0;y < 25; y++){
                    for(x = 0;x < 80; x++){
                        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                    }
                }
            }
        }
    }
}

void printfHex(uint8_t key){
    char* foo = (char*)"00";
    const char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0f];
    foo[1] = hex[key & 0x0f];
    printf((const char*)foo);    
}

class PrintKeyBoardEventHandler : public KeyboardEventHandler {
public:
    void OnKeyDown(char c){
        char* foo = (char*)" ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler {
    int8_t x, y;
public:
    MouseToConsole()
        : x(40), y(12){

        }

    void OnActive() {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[y*80+x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                            ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                            ((VideoMemory[y * 80 + x] & 0x00ff));
    }

    void OnMouseMove(int8_t nx, int8_t ny){
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[y*80+x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                            ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                            ((VideoMemory[y * 80 + x] & 0x00ff));

        x += nx;
        if(x < 0) x = 0;
        else if(x >= 80) x = 79;

        y += ny;
        if(y < 0) y = 0;
        else if(y >= 25) y = 24;

        VideoMemory[y*80+x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                            ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                            ((VideoMemory[y * 80 + x] & 0x00ff));
    }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors(){
    for(constructor* i = &start_ctors; i != &end_ctors; i++){
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt, 0x20);

#ifdef GRAPHICMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xa8);
#endif

    DriverManager drvManager;
#ifdef GRAPHICMODE
    KeyBoardDriver keyBoard(&interrupts, &desktop);
#else
    PrintKeyBoardEventHandler kbhandler;
    KeyBoardDriver keyBoard(&interrupts, &kbhandler);
#endif
    drvManager.AddDriver(&keyBoard);

#ifdef GRAPHICMODE
    MouseDriver mouse(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
#endif
    drvManager.AddDriver(&mouse);

    PerippheraComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

#ifdef GRAPHICMODE
    VideoGraphicsArray vga;
    vga.SetMode(320, 200, 8);
    Window w1(&desktop, 10, 10, 20, 20, 0xa8, 0x00, 0x00);
    desktop.AddChild(&w1);
    Window w2(&desktop, 40, 15, 30, 30, 0x00, 0xa8, 0x00);
    desktop.AddChild(&w2);
#endif
    drvManager.ActivateAll();

    interrupts.Activate();
    //vga.FillRectangle();
    while(1){
#ifdef GRAPHICMODE
    desktop.Draw(&vga);
#endif        
    }
}