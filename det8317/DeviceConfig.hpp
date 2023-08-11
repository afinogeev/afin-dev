#ifndef _deviceconfig_hpp
#define _deviceconfig_hpp

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "AfinConfig.hpp"

class DeviceConfig : public AfinConfig {
    public:
        DeviceConfig(FS* fileSystem, String fileName) :
        AfinConfig(fileSystem, fileName) {
        }

        String freq() {
            return _config["freq"];
        }

        void setFreq(String value) {
            if(value != _config["freq"]){
                _config["freq"] = value;
                // save();
            }
        }
        void reset() {
            _config["freq"] = "5000";
        }
};

#endif _deviceconfig_hpp
