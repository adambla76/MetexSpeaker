/*

           METEX SPEAKER v1.25

         (c) AdamBla76@gmail.com
     
       wersja ESP32 dla METEX M-3860D


       biblioteki potrzebne do kompilacji kodu: 

          https://github.com/schreibfaul1/ESP32-audioI2S
          https://github.com/tzapu/WiFiManager


*/

#include "Arduino.h"
#include "Audio.h"
#include <WiFiManager.h>

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26
#define RXD2          16      
#define TXD2          17      

Audio audio;

long lastms = 0;
String _mode;
String _value;
String _unit;
long _time;
long _lastcmd = 0;
long _lasttime = 0;
bool speech_done = false;

void setup() {
    Serial.begin(115200);
    Serial2.begin(1200, SERIAL_7N2, RXD2, TXD2);
    Serial.println();
    Serial.println("Metex Speaker");
    Serial.println();
   
    WiFi.mode(WIFI_STA);
    WiFiManager wm;
    //wm.resetSettings();

    bool res;
    res = wm.autoConnect("MetexSpeaker"); 
    
    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        Serial.println("connected... ok");
    }
 
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(18); // 0...21
    speech_done = false;
    audio.connecttospeech("Witam Cię Andrzeju - jestem gotowy do pracy. Czekam na połączenie z przyrządu Metex","pl");
    _lasttime = millis();
    _time = millis();
   
}

String DecodeUnits(String _unit) {
   String sss;

   if (_unit == "mV") { sss = " miliwolta"; }
   else if (_unit == "V") { sss = " wolta"; }
   else if (_unit == "mA") { sss = " miliampera"; }
   else if (_unit == "A") { sss = " ampera"; }
   else if (_unit == "Ohm") { sss = " ohma"; }
   else if (_unit == "KOhm") { sss = " kilo-ohma"; }
   else if (_unit == "MOhm") { sss = " mega-ohma"; }
   else if (_unit == "Hz") { sss = " herca"; }
   else if (_unit == "KHz") { sss = " kiloherca"; }
   else if (_unit == "MHz") { sss = " megaherca"; }
   else if (_unit == "pF") { sss = " pikofarada"; }
   else if (_unit == "nF") { sss = " nanofarada"; }
   else if (_unit == "uF") { sss = " mikrofarada"; }
   else if (_unit == "mH") { sss = " milihenra"; }
   else if (_unit == "uH") { sss = " mikrohenra"; }
   else if (_unit == "C") { sss = " stopni celcjusza"; }

   return sss;
   
}

void SayBabe() {
   char buf[200];
   String sss,ccc;
   if (_mode == "AC") { ccc=" AC "; } else { ccc=""; }; 
   if (_value == "0,00" ) { _value = "0"; };
   if (_unit == "mV") { sss = "napięcie " + ccc + _value + DecodeUnits(_unit); }
   else if (_unit == "V") { sss = "napięcie " + ccc + _value + DecodeUnits(_unit); }
   else if (_unit == "mA") { sss = "prąd " + ccc + _value + DecodeUnits(_unit); }
   else if (_unit == "A") { sss = "prąd " + ccc + _value + DecodeUnits(_unit); }
   else if (_mode == "OH") { sss = "rezystancja " + _value + DecodeUnits(_unit); }
   else if (_mode == "FR") { sss = "częstotliwość " + _value + DecodeUnits(_unit); }
   else if (_mode == "CA") { sss = "pojemność " + _value + DecodeUnits(_unit); }
   else if (_mode == "TM") { sss = "temperatura " + _value + DecodeUnits(_unit); }
   else if (_mode == "HR") { sss = "indukcyjność " + _value + DecodeUnits(_unit); }
   else { sss = "cisza"; };
   
   if (sss!="cisza") {
    sss.toCharArray(buf,sss.length()+1);
    speech_done = false;
    audio.connecttospeech(buf,"pl");
   } 
   
}

void checkMetex() {
  byte i;
  char inBuf[32];
  
  if (millis()-_lastcmd > 3000) {
       _lastcmd = millis();
       Serial2.print("D\r"); 
       Serial.println("send D");
  }

  i=Serial2.available();
  if (i>=28) {
      Serial2.read(inBuf,28);
      Serial2.flush();
      for(int x=0;x<28;x++) {
        if(inBuf[x]==13) {
          _time  = millis();
          _lasttime = _time;
          _mode  = String(inBuf[x+1])  + String(inBuf[x+2]);  
          _value = String(inBuf[x+4]) + String(inBuf[x+5])  + String(inBuf[x+6])  + String(inBuf[x+7]) + String(inBuf[x+8]) + String(inBuf[x+9]);    
          _unit  = String(inBuf[x+10]) + String(inBuf[x+11]) + String(inBuf[x+12]) + String(inBuf[x+13]);
          _mode.trim();
          _value.trim();
          _unit.trim();
          float v = _value.toFloat(); 
          _value = String(v);
          _value.replace(".",",");
          _unit.trim();
          Serial.println(String(_time) + ": " + _mode + " " + _value + " " + _unit);
          break; 
        }
      }
  }
}


void audio_eof_speech(const char*)
{
    speech_done=true;  
}

void loop()
{
    audio.loop();
    checkMetex();

    if (millis()-lastms > 10000 and speech_done) {
       lastms = millis();
       if (millis()-_lasttime < 15000) {
         SayBabe();
       } else {
         speech_done = false;
         audio.connecttospeech("Brak połączenia z metexem","pl");
       }     
    }

}
