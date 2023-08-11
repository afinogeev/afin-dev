#include "Dds9833Device.hpp"

Dds9833Device *device;

void setup(void) {
  device = new Dds9833Device();
}

void loop(void) {
  device->loop();
}
