#include "wifi_manager_helper.h"

void setup_wifi() {
  Serial.println();
  Serial.println("================================");
  Serial.println("       WIFIMANAGER PORTAL");
  Serial.println("================================");

  WiFiManager wifiManager;

  // Uncomment baris di bawah ini jika ingin mereset setting WiFi yang tersimpan sebelumnya saat testing
  // wifiManager.resetSettings();

  // Membuka Access Point dengan nama "ESP32_Config_Portal" (password: 12345678)
  // Jika ESP32 belum terhubung ke WiFi atau gagal, dia akan membuat AP konfigurasi sendiri
  if (!wifiManager.autoConnect("ESP32_Config_Portal", "12345678")) {
    Serial.println("Gagal terhubung dan waktu habis (Timeout). Restart ESP32...");
    delay(3000);
    ESP.restart();
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung via WiFiManager!");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
