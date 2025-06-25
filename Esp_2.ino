#include <WiFi.h>
#include <esp_now.h>

uint8_t esp1_mac[] = {0x88, 0x13, 0xBF, 0x0D, 0x3C, 0x78};
uint8_t esp3_mac[] = {0x3C, 0x8A, 0x1F, 0xAD, 0xC8, 0x8C};

String received = "";
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

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  Serial.println("-----------------------------");

  int ldr = analogRead(34);
  String own_ldr = String(ldr);

  esp_now_del_peer(esp1_mac);

  if (received != "") {
    if (tryAddPeer(esp1_mac)) 
    {
      String message = "ESP2 LDR: " + own_ldr + "," + received;
      esp_now_send(esp1_mac, (uint8_t *)message.c_str(), message.length());
      delay(100);
      if (success == false)
      {
        Serial.print("ESP2 LDR: ");
        Serial.println(own_ldr);
        Serial.println(received);

        received = "";
      }
      else
      {
        Serial.print("ESP2 LDR: ");
        Serial.println(own_ldr);
        Serial.println(received);
        Serial.println("----Sent to ESP1");
        
        received = "";
      }
    } 
  } 
  else 
  {
    if(tryAddPeer(esp1_mac))
    {
      String message = "ESP2 LDR: " + own_ldr;
      esp_now_send(esp1_mac, (uint8_t *)message.c_str(), message.length());
      delay(100);

      if (success == false)
      {
        Serial.print("ESP2 LDR: ");
        Serial.println(own_ldr);
      }
      else
      {
        Serial.print("ESP2 LDR: ");
        Serial.println(own_ldr);
        Serial.println("----Sent to ESP1");
      }
    } 
  }
  delay(1000);
}

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  received = "";
  for (int i = 0; i < len; i++) received += (char)data[i];

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
           recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);
}

void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  if (tx_info == nullptr) return;
  const uint8_t *mac_addr = tx_info->des_addr;

  success = (status == ESP_NOW_SEND_SUCCESS);
  
  if (memcmp(mac_addr, esp1_mac, 6) == 0) 
  {
    esp1_available = success;
  }
}

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
