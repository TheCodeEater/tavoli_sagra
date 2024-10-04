#include "esphome.h"

namespace jcc{ //jack custom components namespace
/**
 * Class to represent a BLE device scanner. Detects nearby ble devices and allows to perform some operations.
 * Designed to read RSSI.
 */
    class bleScanner: public esphome::Component{
        public:
        /**
         * 
         */
            bleScanner();

        private:

    };
}