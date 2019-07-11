#include <EncoderInterfaceMagnetic.h>


EncoderInterfaceMagnetic dev;

void setup()
{
  dev.setup();
  dev.startServer();
}

void loop()
{
  dev.update();
}
