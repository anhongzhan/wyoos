#include "interrupts.h"

void printf(const char*);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager){
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler(){
    if(interruptManager->handlers[interruptNumber] == this){
        interruptManager->handlers[interruptNumber] = 0;
    }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp){
    return esp;
}


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType){
        const uint8_t IDT_DESC_PRESENT = 0x80;

        interruptDescriptorTable[interruptNumber].handleAddressLowBits = ((uint32_t)handler) & 0xffff;
        interruptDescriptorTable[interruptNumber].handleAddressHighBits = ((uint32_t)handler >> 16) & 0xffff;
        interruptDescriptorTable[interruptNumber].gdt_codeSegementSelector = codeSegmentSelectorOffset;
        interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
        interruptDescriptorTable[interruptNumber].reserved = 0; 
    }


InterruptManager::InterruptManager(GlobalDescriptorTable* gdt, uint16_t hardwareInterruptOffset)
    :picMasterCommand(0x20),
    picMasterData(0x21),
    picSlaveCommand(0xA0),
    picSlaveData(0xA1)
{
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint16_t codeSegemnt = gdt->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xe;
    for(uint16_t i=0; i<256; i++){
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, codeSegemnt, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    }

    SetInterruptDescriptorTableEntry(0x00, codeSegemnt, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, codeSegemnt, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, codeSegemnt, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, codeSegemnt, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, codeSegemnt, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, codeSegemnt, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, codeSegemnt, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, codeSegemnt, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, codeSegemnt, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, codeSegemnt, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, codeSegemnt, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, codeSegemnt, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, codeSegemnt, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, codeSegemnt, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, codeSegemnt, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, codeSegemnt, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, codeSegemnt, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, codeSegemnt, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, codeSegemnt, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, codeSegemnt, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, codeSegemnt, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, codeSegemnt, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, codeSegemnt, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, codeSegemnt, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, codeSegemnt, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, codeSegemnt, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, codeSegemnt, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, codeSegemnt, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, codeSegemnt, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, codeSegemnt, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, codeSegemnt, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, codeSegemnt, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, codeSegemnt, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, codeSegemnt, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, codeSegemnt, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, codeSegemnt, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x31, codeSegemnt, &HandleInterruptRequest0x31, 0, IDT_INTERRUPT_GATE);

    //ICW1
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    //ICW2
    picMasterData.Write(hardwareInterruptOffset);
    picSlaveData.Write(hardwareInterruptOffset + 0x8);

    //ICW3
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    //ICW4
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    //OCW1
    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    InterruptorDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {}

void InterruptManager::Activate() {
    if(ActiveInterruptManager != 0){
        ActiveInterruptManager->Deactivate();
    }
    ActiveInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate() {
    if(ActiveInterruptManager == this){
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::HandleInterrupt(uint8_t InterruptNumber, uint32_t esp){
    if(ActiveInterruptManager != 0){
        return ActiveInterruptManager->DoHandleInterrupt(InterruptNumber, esp);
    }
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t InterruptNumber, uint32_t esp){
    if(handlers[InterruptNumber] != 0){
        esp = handlers[InterruptNumber]->HandleInterrupt(esp);
    }else if(InterruptNumber != hardwareInterruptOffset){
        char* foo = (char*)"UNHANDLED INTERRUPT 0X00";
        const char* hex = "0123456789ABCDEF";
        foo[22] = hex[(InterruptNumber >> 4) & 0x0f];
        foo[23] = hex[InterruptNumber & 0x0f];
        printf((const char*)foo);
    }

    //结束外部中断，由于前7个中断是主芯片控制，后面8个中断是slave芯片控制，所以要判断向哪个芯片发送0x20(PIC_EOI)
    if(hardwareInterruptOffset <= InterruptNumber && InterruptNumber < hardwareInterruptOffset + 16){
        picMasterCommand.Write(0x20);
        if(hardwareInterruptOffset + 8 <= InterruptNumber){
            picSlaveCommand.Write(0x20);
        }
    }
    return esp;
}

uint16_t InterruptManager::getHardwareInterruptOffset(){
    return this->hardwareInterruptOffset;
}