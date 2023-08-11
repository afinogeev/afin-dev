#include "Det8317Device.hpp"

Det8317Device *device;

void setup(void) {
  device = new Det8317Device();
}

void loop(void) {
  device->loop();
}
