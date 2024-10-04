#include "esphome.h"
#include <BLEScan.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertisedDevice.h>

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