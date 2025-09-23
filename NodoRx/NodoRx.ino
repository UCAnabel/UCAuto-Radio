#include <RadioLib.h>
#include "LoRaBoards.h"
#include "utilities.h"
#include "SharedTypes.h"
#include <TinyGPS++.h>
#include <math.h>

SX1276 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_DIO1_PIN);
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// Cálculo de distancia 3D entre coordenadas incluyendo altitud
float distanceMeters(float lat1, float lon1, float alt1, float lat2, float lon2, float alt2) {
  const float R = 6371000.0; // Radio de la Tierra en metros
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  float meanLat = radians((lat1 + lat2) / 2.0);

  float dx = dLon * cos(meanLat) * R;
  float dy = dLat * R;
  float dz = alt2 - alt1;

  return sqrt(dx * dx + dy * dy + dz * dz);
}

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
    radio.startReceive();
  } else {
    Serial.println("Radio init failed!");
    while (true);
  }
}

void loop() {
  // Leer datos del GPS local
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  GPSPacket packet;
  int state = radio.receive((uint8_t *)&packet, sizeof(packet));

  if (state == RADIOLIB_ERR_NONE) {
    float rssi = radio.getRSSI();
    float snr  = radio.getSNR();

    if (gps.location.isValid() && gps.altitude.isValid()) {
      float distancia = distanceMeters(
        packet.latitude, packet.longitude, packet.altitude,
        gps.location.lat(), gps.location.lng(), gps.altitude.meters()
      );

      Serial.println("----------s- Paquete recibido -----------");
      Serial.printf("TX: lat=%.6f lon=%.6f alt=%.2f\n", packet.latitude, packet.longitude, packet.altitude);
      Serial.printf("RX: lat=%.6f lon=%.6f alt=%.2f\n", gps.location.lat(), gps.location.lng(), gps.altitude.meters());
      Serial.printf("Distancia: %.2f metros\n", distancia);
      Serial.printf("RSSI: %.0f dBm | SNR: %.2f dB\n", rssi, snr);
      Serial.println("----------------------------------------");
    } else {
      Serial.println("GPS local no disponible o incompleto.");
    }

    radio.startReceive();  // Esperar siguiente paquete
  } else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.printf("Error de recepción: %d\n", state);
  }

  delay(500);
}
