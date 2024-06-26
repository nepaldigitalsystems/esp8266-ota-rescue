#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

extern "C" {
#include <user_interface.h>
}

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void wificonfig_wifiOn() {
  wifi_fpm_do_wakeup();
  wifi_fpm_close();
  delay(100);
  }

#define FPM_SLEEP_MAX_TIME 0xFFFFFFF


void wificonfig_wifiOff() {
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_set_sleep_type(MODEM_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(FPM_SLEEP_MAX_TIME);
  delay(100);
  }


void setup(void){
  wificonfig_wifiOff();
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  wificonfig_wifiOn();
  boolean result = WiFi.softAP("esp8266", ""); // "" => no password
  Serial.println(result == true ? "AP setup OK" : "AP setup failed");

  IPAddress myIP = WiFi.softAPIP();  
  Serial.print("Access Point IP address: ");Serial.println(myIP);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  Serial.print("HTTPUpdateServer ready! Open http://");Serial.print(myIP);Serial.println("/update in your browser");  
  }


void loop(void){
  httpServer.handleClient();
  }
