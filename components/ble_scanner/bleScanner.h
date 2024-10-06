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
        static const uint8_t max_devices=128;

        public:
        /**
         * Create empty device association
         */
            bleScanner(): m_devices{}, m_scanComplete{false} {

            }

        /**
         * Wrapper around process devices which 
         */
        void processDevice(std::string& dev){
            StaticJsonDocument<96> doc{};
            deserializeJson(doc,dev);
            const char* mac{doc["address"]};

            //check validity
            if(mac==nullptr){
                ESP_LOGE("Ble scanner","Cannot get mac address of given device!");
            }else{
                setDevice(std::string(mac),doc["rssi"],doc["timestamp"]);
            }
            
        }
        /**
         * Add device with mac and rssi
         */
        void setDevice(std::string mac, int8_t rssi, uint16_t time){

        }
        /**
         * Empty device list
         */
        void clearDevices(){
            //set memory to 0. POD is essential to make this work
            memset(m_devices,0,max_devices*sizeof(bleDevice));
        }
        /**
         * Check if there are devices with rssi greater than the thd
         */
        bool checkNearbyDevices(int8_t threshold){

            return true;
        }

        private:
             //map_device m_nearby_devices; /// Map MAC to rssi of nearby devices
             //map_device m_nearby_devices_lastonline; /// Map MAC to last detected state
             bleDevice m_devices[max_devices];
             bool m_scanComplete;
            

    };
}
#endif