/*------------------------------------------------------------------
   Corona Virus Monitoring dengan ESP32
   Oleh: Feryan Romadhon

   Sumber data dari https://api.kawalcorona.com/indonesia/provinsi/
   - Sesuaikan SSID dan Password WiFi dengan WiFi anda
   - Sesuaikan index Provinsi dengan kebutuhan

   Library yang dibutuhkan:
   - WiFi.h
   - HTTPClient.h
   - ArduinoJson.h
  ------------------------------------------------------------------*/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// sesuaikan dengan WiFi anda
const char* WiFiSSID = "wifi-anda";
const char* WiFiPass = "password-wifi-anda";

const char* url = "https://api.kawalcorona.com/indonesia/provinsi/";
const int indexProvinsi = 4;  // 4 = Jawa Tengah

/* Inisialisasi */
void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.println("ESP32 Covid19 Monitoring");
  Serial.println();
  Serial.print("Menghubungkan ke WiFi ");

  WiFi.begin(WiFiSSID, WiFiPass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Terhubung!");
  Serial.println("IP: " + WiFi.localIP().toString());
  Serial.println();
}

/* Program Utama */
void loop() {
  DynamicJsonDocument doc(7000);
  HTTPClient http;

  http.begin(url);
  int httpCode = http.GET();  // ambil data dari server

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // Berhasil
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println();

      deserializeJson(doc, payload);  // Deserialize JSON dari server
      Serial.println("+===========================================+");
      Serial.println("| " + String(doc[indexProvinsi]["attributes"]["Provinsi"].as<char*>()));
      Serial.println("+-------------------------------------------+");
      Serial.println("| Kasus Positif  : " + String(doc[indexProvinsi]["attributes"]["Kasus_Posi"].as<int>()));
      Serial.println("| Kasus Sembuh   : " + String(doc[indexProvinsi]["attributes"]["Kasus_Semb"].as<int>()));
      Serial.println("| Kasus Meninggal: " + String(doc[indexProvinsi]["attributes"]["Kasus_Meni"].as<int>()));
      Serial.println("+-------------------------------------------+");
      Serial.println("| api.kawalcorona.com/indonesia/provinsi/");
      Serial.println("+===========================================+");
    }
  }
  else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  delay(10000);   // delay 10 detik
}
