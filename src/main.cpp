#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- KONFIGURASI WIFI & MQTT ---
const char* ssid = "ghataghati";         // Ubah dengan SSID WiFi Anda
const char* password = "55555555"; // Ubah dengan Password WiFi Anda

const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
// Gunakan client_id yang unik (misal ditambah nomor absen/nama Anda)
const char* mqtt_client_id = "ESP32_FirmwareDayone_13"; 
const char* mqtt_topic = "smk2/sija/firmware_dayone/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long interval = 5000; // Kirim data setiap 5 detik

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi terhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop sampai terhubung kembali ke MQTT broker
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT Broker...");
    // Coba hubungkan
    if (client.connect(mqtt_client_id)) {
      Serial.println("Berhasil terhubung!");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;

    // --- MEMBUAT DOKUMEN JSON BERDASARKAN DATA DUMMY ---
    JsonDocument doc; // Menggunakan ArduinoJson v7

    doc["device_id"] = "firmware_dayone";
    doc["timestamp"] = "2026-11-09T04:15:30Z"; // (Opsional: bisa disesuaikan dengan millis/NTP time)

    JsonArray sensors = doc["to_sensors"].to<JsonArray>(); // Sesuai struktur data Anda
    // Menggunakan key "sensors" langsung:
    // (Jika ingin persis seperti struktur array, kita buat manual di bawah)

    // Cara mapping objek JSON array "sensors":
    JsonObject s1 = sensors.add<JsonObject>();
    s1["id"] = "kekuatan_wiFi";
    s1["value"] = (float)WiFi.RSSI(); // Nilai dinamis kekuatan sinyal WiFi aktual (dBm) atau set dummy float
    s1["satuan"] = "dBm";

    JsonObject s2 = sensors.add<JsonObject>();
    s2["id"] = "001pH";
    s2["value"] = 7.20;
    s2["satuan"] = "pH";

    JsonObject s3 = sensors.add<JsonObject>();
    s3["id"] = "002EC";
    s3["value"] = 1.45;
    s3["satuan"] = "mS/cm";

    JsonObject s4 = sensors.add<JsonObject>();
    s4["id"] = "003bh1750";
    s4["value"] = 450.50;
    s4["satuan"] = "lux";

    JsonObject s5 = sensors.add<JsonObject>();
    s5["id"] = "004aht20_temp";
    s5["value"] = 28.50;
    s5["satuan"] = "°C";

    JsonObject s6 = sensors.add<JsonObject>();
    s6["id"] = "004aht20_hum";
    s6["value"] = 65.20;
    s6["satuan"] = "%";

    JsonObject s7 = sensors.add<JsonObject>();
    s7["id"] = "005sensorair";
    s7["value"] = 26.80;
    s7["satuan"] = "°C";

    // Ubah JsonDocument menjadi string JSON
    char jsonBuffer[1024];
    serializeJson(doc, jsonBuffer);

    // Kirim payload via MQTT
    Serial.print("Mengirim payload ke topik [");
    Serial.print(mqtt_topic);
    Serial.println("]:");
    Serial.println(jsonBuffer);

    client.publish(mqtt_topic, jsonBuffer);
  }
}