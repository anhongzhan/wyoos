#ifndef __WYOOS__DRIVERS__DRIVER_H
#define __WYOOS__DRIVERS__DRIVER_H

namespace wyoos
{
    namespace drivers
    {
        class Driver{
        public:
            Driver();
            ~Driver();

            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };

        class DriverManager{
        public:
            DriverManager();
            ~DriverManager();

            void AddDriver(Driver*);
            void ActivateAll();

            Driver* drivers[256];
            int numDrivers;
        };
    }
}

#endif