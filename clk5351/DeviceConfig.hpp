#ifndef _deviceconfig_hpp
#define _deviceconfig_hpp

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "AfinConfig.hpp"

#define OUTPUTS_SIZE 3

class DeviceConfig : public AfinConfig{
    public:
        DeviceConfig(FS* fileSystem, String fileName) :
        AfinConfig(fileSystem, fileName) {
        }

        String calibr() {
            return _config["calibr"];
        }

        void setCalibr(String value) {
            if(value != _config["calibr"])
                _config["calibr"] = value;
        }

        String freq(int chan) {
            if(chan < OUTPUTS_SIZE)
                return _config["output"][chan]["freq"];
            return "";
        }

        void setFreq(int chan, String value) {
            if(chan < OUTPUTS_SIZE)
                if(value != _config["output"][chan]["freq"])
                    _config["output"][chan]["freq"] = value;
        }

        String phase(int chan) {
            if(chan < OUTPUTS_SIZE)
                return _config["output"][chan]["phase"];
            return "";
        }

        void setPhase(int chan, String value) {
            if(chan < OUTPUTS_SIZE)
                if(value != _config["output"][chan]["phase"])
                    _config["output"][chan]["phase"] = value;
        }

        String enable(int chan) {
            if(chan < OUTPUTS_SIZE)
                return _config["output"][chan]["enable"];
            return "";
        }

        void setEnable(int chan, String value) {
            if(chan < OUTPUTS_SIZE)
                if(value != _config["output"][chan]["enable"])
                    _config["output"][chan]["enable"] = value;
        }

        String invert(int chan) {
            if(chan < OUTPUTS_SIZE)
                return _config["output"][chan]["invert"];
            return "";
        }

        void setInvert(int chan, String value) {
            if(chan < OUTPUTS_SIZE)
                if(value != _config["output"][chan]["invert"])
                    _config["output"][chan]["invert"] = value;
        }

        String idrive(int chan) {
            if(chan < OUTPUTS_SIZE)
                return _config["output"][chan]["idrive"];
            return "";
        }

        void setIdrive(int chan, String value) {
            if(chan < OUTPUTS_SIZE)
                if(value != _config["output"][chan]["idrive"])
                    _config["output"][chan]["idrive"] = value;
        }

        void reset() {
            for(int i = 0; i < OUTPUTS_SIZE; i++) {
                _config["output"][i]["freq"] = "100000000";
                _config["output"][i]["phase"] = "0";
                _config["output"][i]["enable"] = "1";
                _config["output"][i]["invert"] = "0";
                _config["output"][i]["idrive"] = "2";
            }
        }
};

#endif _deviceconfig_hpp