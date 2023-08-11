#ifndef _det8317device_hpp
#define _det8317device_hpp

#include "AfinDevice.hpp"
#include "DeviceConfig.hpp"
#include "DeviceCalibr.hpp"

#define STATE_ANS_SIZE 50
#define ANALOG_INPUT A0
#define FILTER_SIZE 3

class Det8317Device : AfinDevice {
    public:
        Det8317Device() : 
        AfinDevice() {

            _deviceConfig = new DeviceConfig(fileSystem, "config/device.json");
            _deviceCalibr = new DeviceCalibr(fileSystem, "config/calibr.json");
            DBG_OUTPUT.println(_deviceCalibr->serialized());

            connect();              
        }

        void loop() {
            AfinDevice::loop();
        }

    private:
        DeviceConfig *_deviceConfig;
        DeviceCalibr *_deviceCalibr;
        StaticJsonDocument<STATE_ANS_SIZE> _jsonState;
        float _filter[FILTER_SIZE];
        int _calibr = 0;

        void connect() {
            AfinDevice::connect();

            server.on("/state", HTTP_GET, [&]{handleState();}); 
            server.on("/calibr", HTTP_GET, [&]{_calibr = !_calibr; server.send(200, TEXT_PLAIN, "");}); 
        }

        void handleState() {
            DBG_OUTPUT.println(String("handleState"));

            if(server.arg("freq") != "") {
                _deviceConfig->setFreq(server.arg("freq"));
                server.send(200, TEXT_PLAIN, "");
                _deviceConfig->save();
                return;
            }

            _jsonState["time"] = millis();
            _jsonState["analog"] = _calibr ? filter((float)analogRead(ANALOG_INPUT)) : _deviceCalibr->get(_deviceConfig->freq().toFloat(), filter((float)analogRead(ANALOG_INPUT)));
            _jsonState["freq"] = _deviceConfig->freq().toFloat();
            String strState;
            serializeJson(_jsonState, strState);

            DBG_OUTPUT.println(strState);
            server.send(200, TEXT_JSON, strState);
            return;
        }

        float filter(float value) {
            for(int i = 1; i < FILTER_SIZE; i++)
                _filter[i - 1] = _filter[i]; 
            _filter[FILTER_SIZE - 1] = value;

            float min_delta = 0xffff;
            float retval = value;
            float delta = 0xffff;
            for(int i = FILTER_SIZE - 1; i > 0; i--) {
                for(int j = i - 1; j >= 0; j--) {
                    delta = abs(_filter[i] - _filter[j]);
                    if(delta < min_delta) {
                        min_delta = delta;
                        retval = (_filter[i] + _filter[j])/2;
                    }
                }
            }

            return retval;
        }

};

#endif _det8317device_hpp
