#ifndef __WYOOS__GUI_DESKTOP_H
#define __WYOOS__GUI_DESKTOP_H

#include "gui/widget.h"
#include "drivers/mouse.h"

namespace wyoos
{
    namespace gui
    {
        class Desktop : public CompositWidget, public wyoos::drivers::MouseEventHandler {
        public:
            Desktop(wyoos::common::int32_t w, wyoos::common::int32_t h, wyoos::common::uint8_t r, wyoos::common::uint8_t g, wyoos::common::uint8_t b);
            ~Desktop();

            void Draw(wyoos::common::GraphicsContext* gc) override;
            void OnMouseDown(wyoos::common::uint8_t button) override;
            void OnMouseUp(wyoos::common::uint8_t button) override;
            void OnMouseMove(wyoos::common::int32_t x, wyoos::common::int32_t y) override;

        private:
            wyoos::common::uint32_t mouseX;
            wyoos::common::uint32_t mouseY;
        };
    }
}



#endif