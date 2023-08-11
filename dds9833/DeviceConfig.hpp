#ifndef _deviceconfig_hpp
#define _deviceconfig_hpp

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "AfinConfig.hpp"

class DeviceConfig : public AfinConfig{
    public:
        DeviceConfig(FS* fileSystem, String fileName) :
        AfinConfig(fileSystem, fileName){
        }

        String mode(){
            return _config["mode"];
        }

        void setMode(String value){
            if(value != _config["mode"]){
                _config["mode"] = value;
                // save();
            }
        }

        String freq(){
            return _config["freq"];
        }

        void setFreq(String value){
            if(value != _config["freq"]){
                _config["freq"] = value;
                // save();
            }
        }

        String phase(){
            return _config["phase"];
        }

        void setPhase(String value){
            if(value != _config["phase"]){
                _config["phase"] = value;
                // save();
            }
        }

        String gain(){
            return _config["gain"];
        }

        void setGain(String value){
            if(value != _config["gain"]){
                _config["gain"] = value;
                // save();
            }
        }

        void reset(){
            _config["mode"] = "sin";
            _config["freq"] = "1000";
            _config["phase"] = "0";
            _config["gain"] = "100";
        }
};

#endif _deviceconfig_hpp