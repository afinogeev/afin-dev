#include "Clk5351Device.hpp"

Clk5351Device *device;

void setup(void) {
  device = new Clk5351Device();
}

void loop(void) {
  device->loop();
}
