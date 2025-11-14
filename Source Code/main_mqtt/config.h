// Your WiFi credentials.
//char ssid[] = "kool";
//char pass[] = "120120120";

#define ssid      "ESP32"             //AP SSID
#define pass      ""                //AP Password

String  Essid = "";                 //EEPROM Network SSID
String  Epass = "";                 //EEPROM Network Password
String  Etoken = "";                //EEPROM Network token

String  sssid = "";                 //Read SSID From Web Page
String  passs = "";                 //Read Password From Web Page
String  token = "";                 //Read token From Web Page
