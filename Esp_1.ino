#include <WiFi.h>
#include <esp_now.h>

uint8_t esp2_mac[] = {0x08, 0xA6, 0xF7, 0x46, 0xC4, 0xB4};
uint8_t esp3_mac[] = {0x3C, 0x8A, 0x1F, 0xAD, 0xC8, 0x8C};

String received = "";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (received != "") 
  {
    if(received.indexOf(',') != -1){
      int commaIndex = received.indexOf(',');
      if (commaIndex > 0) 
      {
        String esp2_ldr = received.substring(0, commaIndex);
        String esp3_ldr = received.substring(commaIndex + 1);
        Serial.println(esp2_ldr);
        Serial.println(esp3_ldr);
        received = "";
      }
    }
    else
    {
      Serial.println(received);
      received = "";
    }
  } 
  else
  {
    Serial.println("Not Received...");
  } 
  Serial.println("---------------------------------");

  delay(1400);
}

bool tryAddPeer(const uint8_t *mac) 
{
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

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  received = "";
  for (int i = 0; i < len; i++) received += (char)data[i];

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
           recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);
}
