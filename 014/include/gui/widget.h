#ifndef WYOOS__GUI__WIDGET_H
#define WYOOS__GUI__WIDGET_H

#include "common/types.h"
#include "common/graphicscontext.h"
#include "drivers/keyboard.h"

namespace wyoos
{
    namespace gui
    {
        class Widget : public wyoos::drivers::KeyboardEventHandler {
        public:
            Widget(Widget* parent, wyoos::common::int32_t x, wyoos::common::int32_t y,
                wyoos::common::int32_t w, wyoos::common::int32_t h, wyoos::common::uint8_t r,
                wyoos::common::uint8_t g, wyoos::common::uint8_t b);
            ~Widget();

            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(wyoos::common::int32_t &x, wyoos::common::int32_t &y);
            virtual bool ContainsCoordinate(wyoos::common::int32_t x, wyoos::common::int32_t y);

            virtual void Draw(wyoos::common::GraphicsContext* gc);
            virtual void OnMouseDown(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t button);
            virtual void OnMouseUp(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t button);
            virtual void OnMouseMove(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::int32_t nx, wyoos::common::int32_t ny);

        protected:
            Widget* parent;
            wyoos::common::int32_t x, y, w, h;
            wyoos::common::uint8_t r, g, b;
            bool Focusable;
        };

        class CompositWidget : public Widget{
        public:
            CompositWidget(Widget* parent, wyoos::common::int32_t x, wyoos::common::int32_t y,
                wyoos::common::int32_t w, wyoos::common::int32_t h, wyoos::common::uint8_t r,
                wyoos::common::uint8_t g, wyoos::common::uint8_t b);

            ~CompositWidget();

            virtual void GetFocus(Widget* widget);
            virtual bool AddChild(Widget* child);

            virtual void Draw(wyoos::common::GraphicsContext* gc) override;
            virtual void OnMouseDown(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t button) override;
            virtual void OnMouseUp(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t button) override;
            virtual void OnMouseMove(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::int32_t nx, wyoos::common::int32_t ny) override;

            virtual void OnKeyDown(wyoos::common::int8_t x);
            virtual void OnKeyUp(wyoos::common::int8_t x);
        private:
            Widget* children[100];
            wyoos::common::int32_t numChildren;
            Widget* focusedChild;
        };
    } 
    
}

#endif