#ifndef _devicecalibr_hpp
#define _devicecalibr_hpp

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "AfinConfig.hpp"

#define POW_LEVEL_SIZE 7

class DeviceCalibr : public AfinConfig {
    public:
        DeviceCalibr(FS* fileSystem, String fileName) :
        AfinConfig(fileSystem, fileName) {
        }

        float get(float freq, float value) {
            // Serial.println("Calibr");
            if(_freq != freq)
            {
                _freq = freq;

                int idx = 0;
                for(int i = 0; i < _config["freq"].size(); i++)
                    if(freq >= _config["freq"][i])
                        idx = i;
                for(int i = 0; i < POW_LEVEL_SIZE; i++) {
                    String dBm = "0dBm";
                    switch (i)
                    {
                    case 0:
                        dBm = "0dBm";
                        break;
                    case 1:
                        dBm = "-10dBm";
                        break;
                    case 2:
                        dBm = "-20dBm";
                        break;
                    case 3:
                        dBm = "-30dBm";
                        break;
                    case 4:
                        dBm = "-40dBm";
                        break;
                    case 5:
                        dBm = "-50dBm";
                        break;
                    case 6:
                        dBm = "-60dBm";
                        break;
                    default:
                        break;
                    }
                    _inter[i] = inter(dBm, idx, freq);
                    // Serial.print(i); Serial.print(" inter "); Serial.println(_inter[i]);
                }
            }

            return point(value);
        }

        float inter(String dBm, int idx, float freq) {
            if(idx == 0)
                return _config[dBm][0];
            if(idx >= _config[dBm].size() - 1)
                return _config[dBm][_config[dBm].size() - 1];

            float p1 = _config[dBm][idx];
            float p2 = _config[dBm][idx + 1];
            float f1 = _config["freq"][idx];
            float f2 = _config["freq"][idx + 1];

            float k = (p2 - p1)/(f2 - f1);
            float intr = (p1 + k*(freq - f1));

            // Serial.println(dBm);
            // Serial.print("inter = "); Serial.println(intr);

            return intr;
        }

        float point(float value) {
            float koef = 1.;

            int idx = 0;
            for(int i = 0; i < POW_LEVEL_SIZE; i++) {
                if(value >= _inter[i]) {
                    idx = i;
                }
                if(i == (POW_LEVEL_SIZE - 1) && value >= _inter[i]) {
                    koef = 10/(_inter[POW_LEVEL_SIZE - 1] - _inter[POW_LEVEL_SIZE - 2]);
                    return (-1)*(60 + (value - _inter[POW_LEVEL_SIZE - 1])*koef);
                }
                    
            }

            // Serial.print("value = "); Serial.println(value);
            // Serial.print("idx = "); Serial.println(idx);
            
            koef = 10/(_inter[idx + 1] - _inter[idx]);
            return (-1)*(10*idx + (value - _inter[idx])*koef);
        }

        void reset() {
            for(int i = 0; i < _config["coefs"].size(); i++)
                _config["coefs"][i] = 1;
        }

    private:
        float _freq = 0;
        float _inter[POW_LEVEL_SIZE];
};

#endif _devicecalibr_hpp
