#include <WiFi.h>
#include <esp_now.h>

// MAC Addresses of ESP1 and ESP2
uint8_t esp1_mac[] = {0x88, 0x13, 0xBF, 0x0D, 0x3C, 0x78};
uint8_t esp2_mac[] = {0x08, 0xA6, 0xF7, 0x46, 0xC4, 0xB4};

bool esp2_available = false;
bool esp1_available = false;
bool success = false;

void setup() {
  Serial.begin(115200);
  pinMode(34, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  int ldr = analogRead(34);
  String message = "ESP3 LDR: " + String(ldr);
  bool sent = false;

  Serial.println("-----------------------------");

  // Always clear and re-add peers to refresh network view
  esp_now_del_peer(esp1_mac);
  esp_now_del_peer(esp2_mac);

  if (tryAddPeer(esp2_mac)) {
    esp_now_send(esp2_mac, (uint8_t *)message.c_str(), message.length());
    delay(100);
    if (success == false) {
      if (tryAddPeer(esp1_mac)) {
        esp_now_send(esp1_mac, (uint8_t *)message.c_str(), message.length());
        delay(100);
        if (success == false) {
          Serial.println("No ESP available. " + message);
        } else {
          Serial.print(message);
          Serial.println("  ---Sent to ESP1");
          sent = true;
        }
      }
    } else {
      Serial.print(message);
      Serial.println("  ---Sent to ESP2");
      sent = true;
    }
  }

  delay(1000);
}

// Adds peer if not already in the table
bool tryAddPeer(const uint8_t *mac) {
  if (esp_now_is_peer_exist(mac)) return true;

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, mac, 6);
  peer.channel = 0;
  peer.encrypt = false;

  if (esp_now_add_peer(&peer) == ESP_OK) {
    return true;
  } else {
    return false;
  }
}

// Callback when data is sent
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  if (tx_info == nullptr) return;
  const uint8_t *mac_addr = tx_info->des_addr;

  success = (status == ESP_NOW_SEND_SUCCESS);

  if (memcmp(mac_addr, esp2_mac, 6) == 0) {
    esp2_available = success;
  } else if (memcmp(mac_addr, esp1_mac, 6) == 0) {
    esp1_available = success;
  }
}

// Helper to print MAC address
void printMac(const uint8_t *mac) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(macStr);
}