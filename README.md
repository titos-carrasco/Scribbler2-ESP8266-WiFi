# Scribbler2-ESP8266-WiFi
Trying to add Wifi and GPIO to the Scribbler2 robot using and ESP8266 (NodeMCU)

# WiFi - Access Point
* RS232 connections from [S2 Hardware Schematic](https://www.parallax.com/downloads/s2-hardware-schematic) (Back PCB)
* NodeMCU programming using **Arduino IDE**
* Simple code using **SoftwareSerial**
* SoftwareSerial need to be modified. It doesn't use inverse logic in **write()**
* Bridge WiFi<->Serial with **socat**: `$ socat -d -d pty,raw,echo=0 tcp4:192.168.145.1:1500`
* Tested with [Scribbler2-Python](https://github.com/titos-carrasco/Scribbler2-Python)
* [Calico](https://bitbucket.org/ipre/calico) shows error on port **/dev/pts/XX** created by **socat**  

![](images/image3.jpg)


## Connections

![](images/Diagrama_2.jpg)


# Passthrough only
* Tested with [Calico](https://bitbucket.org/ipre/calico) and [Scribbler2-Python](https://github.com/titos-carrasco/Scribbler2-Python)

![](images/image2.jpg)

![](images/image1.jpg)


## Connections

![](images/Diagrama_1.jpg)


## Arduino IDE simple code for NodeMCU

```C++
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
```

### Modified SoftwareSerial.cpp

```C++
size_t SoftwareSerial::write(uint8_t b) {
   if (!m_txValid) return 0;

   if (m_invert) b = ~b;
   // Disable interrupts in order to get a clean transmit
   cli();
   if (m_txEnableValid) digitalWrite(m_txEnablePin, HIGH);
   unsigned long wait = m_bitTime;
   digitalWrite(m_txPin,!m_invert ? HIGH : LOW);
   unsigned long start = ESP.getCycleCount();
    // Start bit;
   digitalWrite(m_txPin, !m_invert ? LOW : HIGH );
   WAIT;
   for (int i = 0; i < 8; i++) {
     digitalWrite(m_txPin, (b & 1) ? HIGH : LOW);
     WAIT;
     b >>= 1;
   }
   // Stop bit
   digitalWrite(m_txPin, !m_invert ? HIGH : LOW);
   WAIT;
   digitalWrite(m_txPin, !m_invert);
   if (m_txEnableValid) digitalWrite(m_txEnablePin, LOW);
   sei();
   return 1;
}
```





