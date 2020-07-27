
//Includes-------------------------------------------|

#include "WiFi.h"

#include <ArduinoJson.h>

#include <HTTPClient.h>

#include "U8g2lib.h"

//Definitions-----------------------------------------|

#define   FONT_ONE_HEIGHT               8                  // font one height in pixels
#define   FONT_TWO_HEIGHT               20                 // font two height in pixels

#define ONBOARD_LED  25

//Screen Dimensions and type

U8G2_SSD1306_128X64_NONAME_F_HW_I2C     u8g2(U8G2_R0, 16, 15, 4);


//Constants and Variables----------------------------|

char            chPassword[] = "oldman 1952";
char            chSSID[] = "Winterfell";

char            chBuffer[128];

//Setup----------------------------------------------|

void setup() {

  Serial.begin (115200);

  pinMode(ONBOARD_LED, OUTPUT);

  Serial.print("Connecting to Wifi");
  WiFi.begin (chSSID, chPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay (1250);
  }
  // Oled Screen Startup 0.96-inch 128*64 dot matrix OLED

  u8g2.begin();
  u8g2.setFont( u8g2_font_6x12_t_symbols);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);


  // Display Wifi Connecting to....

  u8g2.clearBuffer();
  
  u8g2.print("Connecting to Wifi.....");
  
  u8g2.sendBuffer();

  delay(2000);
  
  u8g2.clearBuffer();
  
  u8g2.print("Connected!");
  
  u8g2.sendBuffer();

  delay(2000);

  u8g2.clearBuffer();

  sprintf(chBuffer, "%s", "WiFi Stats:");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);

  // Display the ssid of the wifi router.

  sprintf(chBuffer, "SSID: %s", chSSID);
  u8g2.drawStr(0, FONT_ONE_HEIGHT * 3, chBuffer);

  
  // Display the IP.

  char  chIp[81];
  WiFi.localIP().toString().toCharArray(chIp, sizeof(chIp) - 1);
  sprintf(chBuffer, "IP  : %s", chIp);
  u8g2.drawStr(0, FONT_ONE_HEIGHT * 2, chBuffer);


  // Display the rssi.

  sprintf(chBuffer, "RSSI: %d", WiFi.RSSI());
  u8g2.drawStr(0, FONT_ONE_HEIGHT, chBuffer);


  u8g2.sendBuffer();

delay (5000);
}


void loop() {

  if ((WiFi.status() == WL_CONNECTED))

  {
    HTTPClient http;

    http.begin ("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=EUR%2CGBP%2CUSD%2C%20&include_24hr_change=true");
    int httpCode = http.GET();

    if (httpCode > 0) {

    }

    Serial.println ("\nStatuscode: " + String(httpCode));
    delay (100);




    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(6) + 80;
    DynamicJsonDocument doc(capacity);
    String payload = http.getString();
    const char* json = payload.c_str();


    deserializeJson(doc, json);

    JsonObject bitcoin = doc["bitcoin"];
    float bitcoin_eur = bitcoin["eur"]; // 8509.23
    float bitcoin_eur_24h_change = bitcoin["eur_24h_change"];
    float bitcoin_gbp = bitcoin["gbp"]; // 7748.63
    float bitcoin_gbp_24h_change = bitcoin["gbp_24h_change"];
    float bitcoin_usd = bitcoin["usd"]; // 9908.19
    float bitcoin_usd_24h_change= bitcoin["usd_24h_change"];



    Serial.println("-------------------------------");

    Serial.print("EUR: ");
    Serial.println(bitcoin_eur);
    Serial.print("EUR 24hr %: ");
    Serial.println(bitcoin_eur_24h_change);

    Serial.print("GBP: ");
    Serial.println(bitcoin_gbp);
    Serial.print("GBP 24hr %: ");
    Serial.println(bitcoin_gbp_24h_change);

    Serial.print("USD: ");
    Serial.println(bitcoin_usd);
    Serial.print("USD 24hr %: ");
    Serial.println(bitcoin_gbp_24h_change);

    Serial.println("-------------------------------");

    //Screen Outputs----------------------------|
    
    u8g2.clearBuffer();

    u8g2.setFont( u8g2_font_inb38_mf);

    u8g2.setCursor(0, 0);
    //u8g2.print("EURO: ");
    u8g2.print(bitcoin_eur,0);
    //u8g2.setCursor(0, 10);
    //u8g2.print("EURO: 24hr ");
    //u8g2.print(bitcoin_eur_24h_change);
    //u8g2.print(" %");
    

    u8g2.sendBuffer();


    http.end();



    delay(60000); // 60 seconds cycle to update.
  }
}
