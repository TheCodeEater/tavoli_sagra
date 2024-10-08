#ifndef BLESCANNER_H
#define BLESCANNER_H
#include "esphome.h"
#include <string>
#include <map>
#include <ArduinoJson.h>
#include <algorithm>

namespace jcc{ //jack custom components namespace
    /**
     * Represents a ble device
     * Note: is and must stay POD
     */
    class bleDevice{
        int8_t rssi{};
        uint16_t timestamp{};
    };
/**
 * Class to represent a BLE device scanner. Detects nearby ble devices and allows to perform some operations.
 * Designed to read RSSI.
 */
    class bleScanner: public esphome::Component{
        using map_device=std::map<std::string,int8_t>;
        using pair_type=map_device::value_type;
        using RSSI=int8_t;
        static const uint8_t max_devices=128;

        public:
        /**
         * Create empty device association
         */
            bleScanner(): max_rssi{-2000} { //set to low value so that no device will ever have a lesser rssi
                
            }

        /**
         * Wrapper around set devices which interprets ble tracker output
         */
        void processDevice(std::string& dev){
            StaticJsonDocument<96> doc{}; //create json object
            deserializeJson(doc,dev); //parse
            setDevice(doc["rssi"]); //save rssi value
        }
        /**
         * Add a new device RSSI.
         * If has a maximum value greater than the current one, it becomes the new nearbiest device.
         */
        void setDevice(RSSI value){
            if(value>max_rssi){
                max_rssi=value;
            }
        }
        /**
         * Empty device list
         */
        void clearDevices(){
            //set memory to 0. POD is essential to make this work
            //memset(m_devices,0,max_devices*sizeof(bleDevice));
            max_rssi=-200;
        }
        /**
         * Check if there are devices with rssi greater than the thd
         */
        bool checkNearbyDevices(int8_t threshold){
            return max_rssi>threshold;
            
        }

        private:
             //map_device m_nearby_devices; /// Map MAC to rssi of nearby devices
             //map_device m_nearby_devices_lastonline; /// Map MAC to last detected state
             int16_t max_rssi{};
            

    };
}
#endif