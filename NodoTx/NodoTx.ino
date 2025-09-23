#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <RadioLib.h>
#include "LoRaBoards.h"
#include "utilities.h"
#include "SharedTypes.h"


TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// Define radio, por ejemplo SX1276
SX1276 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_DIO1_PIN);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  setupBoards(true);

  if (radio.begin() == RADIOLIB_ERR_NONE) {
    Serial.println("Radio OK");
    radio.setFrequency(868.0);
    radio.setOutputPower(17);
    radio.setSpreadingFactor(10);
    radio.setBandwidth(125.0);
    radio.setCRC(true);
  } else {
    Serial.println("Radio init failed!");
    while (true);
  }
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    GPSPacket packet;
    packet.latitude = gps.location.lat();
    packet.longitude = gps.location.lng();
    packet.altitude = gps.altitude.meters();

    int state = radio.transmit((uint8_t *)&packet, sizeof(packet));
    Serial.printf("Sent: lat=%.6f lon=%.6f alt=%.2f\n",
                  packet.latitude, packet.longitude, packet.altitude);
    delay(1000);
  }
}
