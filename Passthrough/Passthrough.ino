#include <SoftwareSerial.h>

SoftwareSerial swSer(D2, D3, true, 256);

void setup() {
  Serial.begin(38400);
  swSer.begin(38400);
}

void loop() {
  while( Serial.available() )
    swSer.write( Serial.read() );

  while (swSer.available() > 0)
    Serial.write(swSer.read());
}
