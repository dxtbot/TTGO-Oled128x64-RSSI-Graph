////////////////////////////////////////////////////////////////
// TTGO Signal Strength Meter                ////
// made by dexter
////////////////////////////////////////////////////////////////
#include "images.h"
#include <SPI.h>
#include <LoRa.h>
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  23
#define CONFIG_DIO0 26
#define LORA_PERIOD 433

#include <Wire.h>
#include "SSD1306Wire.h"
#define OLED_CLASS_OBJ  SSD1306Wire
#define OLED_ADDRESS    0x3C
#define OLED_SDA    21
#define OLED_SCL    22
#define OLED_RST    -1
#define LED1    25
//array of frequencies valid for the application to change
long int frequencies[3] = {433910000, 433910000, 433910000};
//controls the current frequency index in the array
int indexFreq = 0;
int incomingPacketSize;
int rssi = 0;
OLED_CLASS_OBJ tft(OLED_ADDRESS, OLED_SDA, OLED_SCL);
int cpo1 = 0;
int cpo2 = 0;
void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  LoRa.setGain(6);
  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
  LoRa.begin(frequencies[indexFreq]);
  LoRa.receive();
  tft.init();
  tft.setTextAlignment(TEXT_ALIGN_CENTER);
  tft.flipScreenVertically();
  tft.clear();
  tft.drawString(57, 20, "!   DXT GRAPH   !");
  tft.drawString(60, 40, "V1.01");
  tft.display();
  delay(2000);
  digitalWrite(LED1, HIGH);
  tft.clear();
  tft.drawXbm(10, 1, robo_width, robo_height, robo_bits);
  tft.display();
  delay(3000);
  digitalWrite(LED1, LOW);
  tft.setColor(WHITE);
  tft.setFont(ArialMT_Plain_10);
  tft.clear();
}


void loop() {
  tft.clear();
  rssi = LoRa.rssi();
  incomingPacketSize = map(rssi, -115, -30, 0, 100);
  tft.drawString(57, 3, " !   SIGNAL   !");
  tft.drawXbm(0, 22, antena_width, antena_height, antena);
  int progress = (incomingPacketSize ) % 100;
  tft.drawProgressBar(0, 53, 120, 10, progress);
  tft.drawString(62, 30, "{ # 433 Mhz # }");
  tft.drawString(60, 51, "RSSI");
  if (progress < 30) {
    digitalWrite(LED1, LOW);
    tft.drawXbm(100, 24, thumbdown_width, thumbdown_height, thumbdown);
  }
  if (progress > 30) {
    digitalWrite(LED1, HIGH);
    tft.drawXbm(100, 24, thumb_width, thumb_height, thumb);
  }
  tft.drawCircle(10, 30, progress / 8);
  tft.drawCircle(10, 30, progress / 6);
  tft.drawCircle(10, 30, progress / 4);
  tft.drawCircle(10, 30, progress / 2);
  tft.display();
  delay(50);
  Serial.println(rssi);
}


