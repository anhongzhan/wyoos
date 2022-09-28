#ifndef __WYOOS__DRIVERS__VGA_H
#define __WYOOS__DRIVERS__VGA_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace wyoos
{
    namespace drivers
    {
        class VideoGraphicsArray
        {
        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            bool SupportsModel(wyoos::common::uint32_t width, wyoos::common::uint32_t height, wyoos::common::uint32_t colordepth);
            bool SetMode(wyoos::common::uint32_t width, wyoos::common::uint32_t height, wyoos::common::uint32_t colordepth);
            void PutPixel(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t r, wyoos::common::uint8_t g, wyoos::common::uint8_t b);
            void PutPixel(wyoos::common::int32_t x, wyoos::common::int32_t y, wyoos::common::uint8_t colorIndex);
        protected:
            hardwarecommunication::Port8Bit miscPort;
            hardwarecommunication::Port8Bit crtcIndexPort;
            hardwarecommunication::Port8Bit crtcDataPort;
            hardwarecommunication::Port8Bit sequencerIndexPort;
            hardwarecommunication::Port8Bit sequencerDataPort;
            hardwarecommunication::Port8Bit graphicsControllerIndexPort;
            hardwarecommunication::Port8Bit graphicsControllerDataPort;
            hardwarecommunication::Port8Bit attributeControllerIndexPort;
            hardwarecommunication::Port8Bit attributeControllerReadPort;
            hardwarecommunication::Port8Bit attributeControllerWritePort;
            hardwarecommunication::Port8Bit attributeControllerResetPort;

            virtual void WriteRegisters(wyoos::common::uint8_t* registers);
            virtual wyoos::common::uint8_t GetColorIndex(wyoos::common::uint8_t r, wyoos::common::uint8_t g, wyoos::common::uint8_t b);
            virtual wyoos::common::uint8_t* GetFrameBufferSegment();

            virtual void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h,   common::uint8_t r, common::uint8_t g, common::uint8_t b);
        };
    }
}

#endif