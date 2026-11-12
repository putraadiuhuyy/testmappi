#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "wifi_manager_helper.h"

// ========================================
// KONFIGURASI MQTT
// ========================================
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

const char* mqtt_client_id = "ESP32_FirmwareDayone_13";

const char* mqtt_topic =
    "smk2/sija/firmware_dayone/telemetry";

// ========================================
// OBJEK WIFI DAN MQTT
// ========================================
WiFiClient espClient;
PubSubClient client(espClient);

// ========================================
// TIMER
// ========================================
unsigned long lastMsg = 0;
const unsigned long interval = 5000;

// ========================================
// MQTT CONNECTION
// ========================================
void reconnect() {
  while (!client.connected()) {

    Serial.println();
    Serial.println("Menghubungkan ke MQTT Broker...");

    if (client.connect(mqtt_client_id)) {

      Serial.println("MQTT berhasil terhubung!");

    } else {

      Serial.print("MQTT gagal terhubung. State: ");
      Serial.println(client.state());

      Serial.println("Mencoba kembali dalam 5 detik...");
      delay(5000);
    }
  }
}

// ========================================
// MEMBUAT DAN MENGIRIM JSON
// ========================================
void publishSensorData() {

  // Membuat JSON Document
  JsonDocument doc;

  // Informasi perangkat
  doc["device_id"] = "firmware_dayone";

  // Timestamp sementara
  doc["timestamp"] = "2026-11-09T04:15:30Z";

  // Array sensor
  JsonArray sensors = doc["sensors"].to<JsonArray>();

  // ----------------------------------------
  // SENSOR 1: WIFI RSSI
  // ----------------------------------------
  JsonObject s1 = sensors.add<JsonObject>();
  s1["id"] = "kekuatan_wifi";
  s1["value"] = WiFi.RSSI();
  s1["satuan"] = "dBm";

  // ----------------------------------------
  // SENSOR 2: pH
  // ----------------------------------------
  JsonObject s2 = sensors.add<JsonObject>();
  s2["id"] = "001pH";
  s2["value"] = 7.20;
  s2["satuan"] = "pH";

  // ----------------------------------------
  // SENSOR 3: EC
  // ----------------------------------------
  JsonObject s3 = sensors.add<JsonObject>();
  s3["id"] = "002EC";
  s3["value"] = 1.45;
  s3["satuan"] = "mS/cm";

  // ----------------------------------------
  // SENSOR 4: BH1750
  // ----------------------------------------
  JsonObject s4 = sensors.add<JsonObject>();
  s4["id"] = "003bh1750";
  s4["value"] = 450.50;
  s4["satuan"] = "lux";

  // ----------------------------------------
  // SENSOR 5: AHT20 TEMPERATURE
  // ----------------------------------------
  JsonObject s5 = sensors.add<JsonObject>();
  s5["id"] = "004aht20_temp";
  s5["value"] = 28.50;
  s5["satuan"] = "°C";

  // ----------------------------------------
  // SENSOR 6: AHT20 HUMIDITY
  // ----------------------------------------
  JsonObject s6 = sensors.add<JsonObject>();
  s6["id"] = "004aht20_hum";
  s6["value"] = 65.20;
  s6["satuan"] = "%";

  // ----------------------------------------
  // SENSOR 7: SENSOR AIR
  // ----------------------------------------
  JsonObject s7 = sensors.add<JsonObject>();
  s7["id"] = "005sensorair";
  s7["value"] = 26.80;
  s7["satuan"] = "°C";

  // ========================================
  // SERIALIZE JSON
  // ========================================
  char jsonBuffer[1024];

  size_t jsonLength = serializeJson(
    doc,
    jsonBuffer,
    sizeof(jsonBuffer)
  );

  if (jsonLength == 0) {
    Serial.println("Gagal membuat JSON!");
    return;
  }

  // ========================================
  // PUBLISH MQTT
  // ========================================
  Serial.println();
  Serial.println("================================");
  Serial.println("         MQTT PUBLISH");
  Serial.println("================================");

  Serial.print("Topic   : ");
  Serial.println(mqtt_topic);

  Serial.print("Payload : ");
  Serial.println(jsonBuffer);

  Serial.print("Ukuran  : ");
  Serial.print(jsonLength);
  Serial.println(" bytes");

  bool success = client.publish(
    mqtt_topic,
    jsonBuffer
  );

  if (success) {
    Serial.println("STATUS  : PUBLISH BERHASIL");
  } else {
    Serial.println("STATUS  : PUBLISH GAGAL");
    Serial.println("Periksa koneksi atau ukuran buffer MQTT.");
  }

  Serial.println("================================");
}

// ========================================
// SETUP
// ========================================
void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("   ESP32 MQTT FIRMWARE DAYONE");
  Serial.println("================================");

  // Koneksi WiFi menggunakan WiFiManager
  setup_wifi();

  // Konfigurasi MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Memperbesar buffer MQTT
  client.setBufferSize(1024);

  Serial.println();
  Serial.println("Konfigurasi MQTT setup selesai.");
}

// ========================================
// LOOP
// ========================================
void loop() {

  // Cek koneksi WiFi
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi terputus! Membuka kembali WiFiManager portal...");
    setup_wifi();
  }

  // Cek koneksi MQTT
  if (!client.connected()) {
    reconnect();
  }

  // Menjaga koneksi MQTT
  client.loop();

  // Kirim data setiap 5 detik
  unsigned long now = millis();

  if (now - lastMsg >= interval) {

    lastMsg = now;

    publishSensorData();
  }
}
