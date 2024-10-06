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
            bleScanner(): m_devices{}, m_last_timestamp{0},m_scanComplete{false} {
                m_devices.reserve(max_devices); //allocate initial memory
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
         * Add device rssi
         */
        void setDevice(RSSI value){
            m_devices.push_back(value);
        }
        /**
         * Empty device list
         */
        void clearDevices(){
            //set memory to 0. POD is essential to make this work
            //memset(m_devices,0,max_devices*sizeof(bleDevice));
            m_devices.clear();
        }
        /**
         * Check if there are devices with rssi greater than the thd
         */
        bool checkNearbyDevices(int8_t threshold){

            auto max_pos=std::max_element(m_devices.begin(),m_devices.end(),[](RSSI a, RSSI b){
                return a<b;
            });
            if(m_scanComplete){
                return *max_pos>threshold;
            }else{
                return true;
            }
            
        }

        private:
             //map_device m_nearby_devices; /// Map MAC to rssi of nearby devices
             //map_device m_nearby_devices_lastonline; /// Map MAC to last detected state
             std::vector<RSSI> m_devices;
             uint16_t m_last_timestamp;
             bool m_scanComplete;
            

    };
}
#endif