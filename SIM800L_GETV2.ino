/*
 *  ESP32 SIM800 HTTP GET TEST | V 1.0_2020-AUG-18
 */

#define SIM800L_RX     15
#define SIM800L_TX     16
#define SIM800L_PWRKEY 4
#define SIM800L_RST    5
#define SIM800L_POWER  23

String apn = "econet.net";                    // APN
String apn_u = "";                     // APN-Username
String apn_p = "";                     // APN-Password
String base_url = "http://api.thingspeak.com/update?api_key=526KMH6YFV06NRWN";
String latitude;
String longitude;

void setup()
{
  pinMode(SIM800L_POWER, OUTPUT);
  digitalWrite(SIM800L_POWER, HIGH);

  Serial.begin(115200);
  Serial.println("ESP32+SIM800L AT CMD Test");
  Serial2.begin(9600, SERIAL_8N1, 15, 16);
  delay(15000);
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  delay(2000);
  gsm_config_gprs();
}


void loop() {
  generateRandomCoordinates();
  gsm_http_get(latitude, longitude);
  delay(3000);
}

void gsm_http_get(String lat, String lon) {
  Serial.println(" --- Start GPRS & HTTP --- ");
  String url = base_url + "&field1=" + lat + "&field2=" + lon;

  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  gsm_send_serial("AT+HTTPACTION=0");
  gsm_send_serial("AT+HTTPREAD");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
}

void gsm_config_gprs() {
  Serial.println(" --- CONFIG GPRS --- ");
  gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
  if (apn_u != "") {
    gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
  }
  if (apn_p != "") {
    gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
  }
}

void gsm_send_serial(String command) {
  Serial.println("Send ->: " + command);
  Serial2.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (Serial2.available()) {
      Serial.write(Serial2.read());
    }
  }
  Serial.println();
}

void generateRandomCoordinates() {
  // Generate random latitude and longitude values
  float lat = random(-90, 90) + random(0, 10000) / 10000.0;
  float lon = random(-180, 180) + random(0, 10000) / 10000.0;

  // Convert float values to string with desired precision
  latitude = String(lat, 5);
  longitude = String(lon, 5);
}
