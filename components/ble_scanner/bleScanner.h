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
        public:
        /**
         * Create empty device association
         */
            bleScanner(): m_nearby_devices{}, m_nearby_devices_lastonline{}, m_last_timestamp{}{

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
            m_nearby_devices[mac]=rssi; //match devices and rssi
            m_nearby_devices_lastonline[mac]=time; //match device and last update
            m_last_timestamp=time; //save last timestamp
            //ESP_LOGD("set device",mac.c_str());
        }
        /**
         * Check if there are devices with rssi greater than the thd
         */
        bool checkNearbyDevices(int8_t threshold){
            /*for(auto const& device : m_nearby_devices){//iterate over each detected device
                if(device.second>threshold /*&& device.first=="4C:D0:19:38:D7:F7"){ //if the rssi is greater than the minimu, then turn on lights
                    found_nearby=true; //flag as found, do not halt the loop. see below
                    //ESP_LOGD("Nearby","yes");
                }
                //remove devices with low rssi that are likely to be unreachable soon. this
                //mitigates ram overflow
                //if(device.second<-70){
                //    m_nearby_devices.erase(device.first);
                //}
            }*/
           //for(auto const& device: m_nearby_devices){
            //ESP_LOGD("RSSI","%d",(int)device.second);
           //}
            auto max=std::max_element(m_nearby_devices.begin(),m_nearby_devices.end(),[](pair_type const& a, pair_type const& b){
                return a.second<b.second; //compare map hold values
            });
            /*std::for_each(m_nearby_devices_lastonline.begin(),m_nearby_devices_lastonline.end(),[this](pair_type const& device){
                //check last update timestamp
                if(abs(device.second-m_last_timestamp)>10){//if a device has not been updated for 10+ seconds
                    m_nearby_devices.erase(device.first);
                    m_nearby_devices_lastonline.erase(device.first);
                }
            });*/

            return max->second>threshold;
        }

        private:
             map_device m_nearby_devices; /// Map MAC to rssi of nearby devices
             map_device m_nearby_devices_lastonline; /// Map MAC to last detected state
             uint16_t m_last_timestamp;
            

    };
}
#endif