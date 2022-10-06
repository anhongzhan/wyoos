#ifndef __WYOOS__GUI_WINDOW_H
#define __WYOOS__GUI_WINDOW_H

#include "gui/widget.h"
#include "drivers/mouse.h"

namespace wyoos
{
    namespace gui
    {
        class Window : public CompositWidget {
        public:
            Window(Widget* parent, wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::int32_t w, wyoos::common::int32_t h,
                wyoos::common::uint8_t r, wyoos::common::uint8_t g, wyoos::common::uint8_t b);
            ~Window();

            void OnMouseDown(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t button);
            void OnMouseUp(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t button);
            void OnMouseMove(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::int32_t nx, wyoos::common::int32_t ny);
        private:
            bool Dragging;
        };
    }
}

#endif