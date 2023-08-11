#ifndef _clk5351device_hpp
#define _clk5351device_hpp

#include <si5351.h>
#include <Wire.h>

#include "AfinDevice.hpp"
#include "DeviceConfig.hpp"

class Clk5351Device : AfinDevice {
    public:
        Clk5351Device() : 
        AfinDevice() {

            _deviceConfig = new DeviceConfig(fileSystem, "config/device.json");

            _si5351 = new Si5351();


            _si5351->init(SI5351_CRYSTAL_LOAD_8PF, 0, _deviceConfig->calibr().toInt());

            for(int i = 0; i < OUTPUTS_SIZE; i++) {
                setFreq(i, _deviceConfig->freq(i));
                setPhase(i, _deviceConfig->phase(i));
                setEnable(i, _deviceConfig->enable(i));
                setInvert(i, _deviceConfig->invert(i));
                setIdrive(i, _deviceConfig->idrive(i));
            }

            connect();              
        }

        void loop() {
            AfinDevice::loop();
        }

    private:
        Si5351 *_si5351 = nullptr;
        DeviceConfig *_deviceConfig;

        void connect() {
            AfinDevice::connect();

            server.on("/state", HTTP_GET, [&]{handleState();}); 
            server.on("/calibr", HTTP_GET, [&]{handleCalibr();});
        }

        void handleState() {
            DBG_OUTPUT.println(String("handleState"));
            if(server.arg("output") != "") {
                int chan = server.arg("output").toInt();

                if(server.arg("freq") != "")
                    setFreq(chan, server.arg("freq"));

                if(server.arg("phase") != "")
                    setPhase(chan, server.arg("phase"));

                if(server.arg("enable") != "")
                    setEnable(chan, server.arg("enable"));

                if(server.arg("invert") != "")
                    setInvert(chan, server.arg("invert"));

                if(server.arg("idrive") != "")
                    setIdrive(chan, server.arg("idrive"));

                _deviceConfig->save();
            }
            else {
                server.send(200, TEXT_JSON, _deviceConfig->serialized());
                return;
            }
            server.send(200, TEXT_PLAIN, "");
            return;
        }

        void handleCalibr() {
            DBG_OUTPUT.println(String("handleCalibr"));
            if(server.arg("value") != "") {
                DBG_OUTPUT.println(server.arg("value"));
                _si5351->set_correction(server.arg("value").toInt(), SI5351_PLL_INPUT_XO);
                _deviceConfig->setCalibr(server.arg("value"));
                _deviceConfig->save();
            }
            else {
                server.send(200, TEXT_PLAIN, _deviceConfig->calibr());
                return;
            }
            server.send(200, TEXT_PLAIN, "");   
            return;
        }

        void setFreq(int chan, double freq) {
            DBG_OUTPUT.println(String("setFreq: ") + String(freq));

            if(freq >= 215000000)
                freq = 215000000;
            if(freq >= 0) {
                if(_deviceConfig->freq(chan).toDouble() >= 150000000 && freq < 150000000) {
                     _si5351->set_freq(11000000000ULL, si5351_clock(chan));
                     DBG_OUTPUT.println("setFreq 110");
                }
                _si5351->set_freq((uint64_t) (freq*100), si5351_clock(chan));
                _deviceConfig->setFreq(chan, String(freq));

            }
        }

        void setFreq(int chan, String freq) {
            setFreq(chan, freq.toDouble());
        }

        void setPhase(int chan, int phase) {
            DBG_OUTPUT.println(String("setPhase: ") + String(phase));

            if(phase >= 0 && phase <= 100) {
                _si5351->set_phase(si5351_clock(chan), phase);
                _deviceConfig->setPhase(chan, String(phase));
            }
        }

        void setPhase(int chan, String phase) {
            setPhase(chan, phase.toInt());
        }

        void setEnable(int chan, int enable) {
            DBG_OUTPUT.println(String("setEnable: ") + String(enable));

            if(enable >= 0 && enable <= 1) {
                _si5351->output_enable(si5351_clock(chan), enable);
                _deviceConfig->setEnable(chan, String(enable));
            }
        }

        void setEnable(int chan, String enable) {
            setEnable(chan, enable.toInt());
        }

        void setInvert(int chan, int invert) {
            DBG_OUTPUT.println(String("setInvert: ") + String(invert));

            if(invert >= 0 && invert <= 1) {
                _si5351->set_clock_invert(si5351_clock(chan), invert);
                _deviceConfig->setInvert(chan, String(invert));
            }
        }

        void setInvert(int chan, String invert) {
            setInvert(chan, invert.toInt());
        }

        void setIdrive(int chan, int idrive) {
            DBG_OUTPUT.println(String("setIdrive: ") + String(idrive));

            if(idrive >= 0 && idrive <= 8) {

                int current = 0;
                switch (idrive)
                {
                case 4:
                    current = 1;
                    break;
                case 6:
                    current = 2;
                    break;
                case 8:
                    current = 3;
                    break;
                default:
                    current = 0;
                    break;
                }

                _si5351->drive_strength(si5351_clock(chan), si5351_drive(current));
                _deviceConfig->setIdrive(chan, String(idrive));
            }
        }

        void setIdrive(int chan, String idrive) {
            setIdrive(chan, idrive.toInt());
        }
};

#endif _clk5351device_hpp
