#ifndef BLESCANNER_H
#define BLESCANNER_H
#include "esphome.h"
#include <string>
#include <map>

namespace jcc{ //jack custom components namespace
/**
 * Class to represent a BLE device scanner. Detects nearby ble devices and allows to perform some operations.
 * Designed to read RSSI.
 */
    class bleScanner: public esphome::Component{
        public:
        /**
         * Create empty device association
         */
            bleScanner(): m_nearby_devices{}{

            }

        void setDevice(std::string& mac, int8_t rssi){
            m_nearby_devices[mac]=rssi; //match devices and rssi
        }

        bool checkNearbyDevices(int8_t threshold){
            bool found_nearby=false;
            for(auto const& device : m_nearby_devices){//iterate over each detected device
                if(device.second>threshold){ //if the rssi is greater than the minimu, then turn on lights
                    found_nearby=true; //flag as found, do not halt the loop. see below
                }
                //remove devices with low rssi that are likely to be unreachable soon. this
                //mitigates ram overflow
                if(device.second<-70){
                    m_nearby_devices.erase(device.first);
                }
            }
            return found_nearby; //report
        }

        private:
            std::map<std::string,int8_t> m_nearby_devices; /// Map MAC to rssi of nearby devices

    };
}
#endif