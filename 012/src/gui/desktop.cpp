#include "gui/desktop.h"

using namespace wyoos;
using namespace wyoos::common;
using namespace wyoos::gui;

Desktop::Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b) 
    : CompositWidget(0, 0, 0, w, h, r, g, b),
      MouseEventHandler() 
{
    mouseX = w / 2;
    mouseY = h / 2;
}

void Desktop::Draw(GraphicsContext* gc) {
    CompositWidget::Draw(gc);

    for (uint32_t i = 0; i < 4; i++) {
        gc->PutPixel(mouseX - 1, mouseY, 0xff, 0xff, 0xff);
        gc->PutPixel(mouseX + i, mouseY, 0xff, 0xff, 0xff);
        gc->PutPixel(mouseX, mouseY - i, 0xff, 0xff, 0xff);
        gc->PutPixel(mouseX, mouseY + i, 0xff, 0xff, 0xff);
    }
}

void Desktop::OnMouseDown(uint8_t button) {
    CompositWidget::OnMouseDown(mouseX, mouseY, button);
}

void Desktop::OnMouseUp(uint8_t button) {
    CompositWidget::OnMouseUp(mouseX, mouseY, button);
}

void Desktop::OnMouseMove(int32_t x, int32_t y) {
    x /= 4;
    y /= 4;

    int32_t newMouseX = mouseX + x;
    if(newMouseX < 0) newMouseX = 0;
    if(newMouseX >= w) newMouseX = w -1;

    int32_t newMouseY = mouseY + y;
    if(newMouseY < 0) newMouseY = 0;
    if(newMouseY >= h) newMouseY = h - 1;

    CompositWidget::OnMouseMove(mouseX, mouseY, newMouseX, newMouseY);
    mouseX = newMouseX, mouseY = newMouseY;
}