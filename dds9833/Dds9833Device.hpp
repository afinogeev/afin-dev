#ifndef _dds9833device_hpp
#define _dds9833device_hpp

#include <MD_AD9833.h>
#include <MCP41xxx.h>
#include <SPI.h>

#include "AfinDevice.hpp"
#include "DeviceConfig.hpp"

#define FSYNCH  15
#define CS      16

class Dds9833Device : AfinDevice {
    public:
        Dds9833Device() : 
        AfinDevice() {
            pinMode(FSYNCH,OUTPUT);
            pinMode(CS,OUTPUT);

            _deviceConfig = new DeviceConfig(fileSystem, "config/device.json");

            _dds = new MD_AD9833(FSYNCH);
            _dds->begin();

            _dds->setActiveFrequency(MD_AD9833::CHAN_0);
            _dds->setActivePhase(MD_AD9833::CHAN_0);
            setMode(_deviceConfig->mode());
            setFreq(_deviceConfig->freq());
            setPhase(_deviceConfig->phase());

            _pot = new MCP41xxx(CS);
            _pot->begin();
            setGain(_deviceConfig->gain());

            connect();              
        }

        void loop() {
            AfinDevice::loop();
        }

    private:
        MD_AD9833 *_dds = nullptr;
        MCP41xxx *_pot = nullptr;
        DeviceConfig *_deviceConfig;

        void connect() {
            AfinDevice::connect();

            server.on("/state", HTTP_GET, [&]{handleState();}); 
        }

        void handleState() {
            DBG_OUTPUT.println(String("handleState"));
            if(server.args()) {
                if(server.arg("mode") != "")
                    setMode(server.arg("mode"));

                if(server.arg("freq") != "")
                    setFreq(server.arg("freq"));

                if(server.arg("phase") != "")
                    setPhase(server.arg("phase"));

                if(server.arg("gain") != "")
                    setGain(server.arg("gain"));

                _deviceConfig->save();
            }
            else {
                server.send(200, TEXT_JSON, _deviceConfig->serialized());
                return;
            }
            server.send(200, TEXT_PLAIN, "");
            return;
        }

        void setMode(String mode) {
            DBG_OUTPUT.println(String("setMode: ") + mode);

            if(mode == "sin") {
                _dds->setMode(MD_AD9833::MODE_SINE);
                _deviceConfig->setMode(mode);
            }
            else if(mode == "triangle") {
                _dds->setMode(MD_AD9833::MODE_TRIANGLE);
                _deviceConfig->setMode(mode);
            }
            else if(mode == "square1") {
                _dds->setMode(MD_AD9833::MODE_SQUARE1);
                _deviceConfig->setMode(mode);
            }
            else if(mode == "square2") {
                _dds->setMode(MD_AD9833::MODE_SQUARE2);
                _deviceConfig->setMode(mode);
            }
            else if(mode == "off") {
                _dds->setMode(MD_AD9833::MODE_OFF);
                _deviceConfig->setMode(mode);
            }
            return;
        }

        void setFreq(float freq) {
            DBG_OUTPUT.println(String("setFreq: ") + String(freq));

            if(freq >= 0 && freq < 25000000 ) {
                _deviceConfig->setFreq(String(freq));
                _dds->setFrequency(MD_AD9833::CHAN_0, freq);
            }
        }

        void setFreq(String freq) {
            setFreq(freq.toFloat());
        }

        void setPhase(float phase) {
            DBG_OUTPUT.println(String("setPhase: ") + String(phase));

            if(phase >= 0. && phase <= 360.) {
                _deviceConfig->setPhase(String(phase));
                _dds->setPhase(MD_AD9833::CHAN_0, phase*10);
            }
        }

        void setPhase(String phase) {
            setPhase(phase.toFloat());
        }

        void setGain(float gain) {
            DBG_OUTPUT.println(String("setGain: ") + String(gain));

            if(gain < 0)
                gain = 0;
            if(gain > 100)
                gain = 100.;

            _deviceConfig->setGain(String(gain));
            _pot->analogWrite((int) 255*gain/100);
        }

        void setGain(String gain) {
            setGain(gain.toFloat());
        }

};

#endif _dds9833device_hpp
