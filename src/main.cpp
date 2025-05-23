#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <AudioFileSourceBuffer.h>
#include <AudioFileSourceICYStream.h>
#include <AudioGeneratorMP3.h>
#include <ESPAudioOutput.h>
#include <AudioOutputSPDIF.h>
#include <Ticker.h>

#include "radio_stations.h"

#define ADC_RESOLUTION 9

// WiFi credentials
const char* ssid = "DDAM3M";
const char* password = "matus987";

volatile uint16_t pwmCounter = 0;
volatile uint16_t currentDuty = 512;

//RELAX RADIO
//const char* streamURL = "http://icecast7.play.cz/relax128.mp3";

//ROCK RADIO
//const char* streamURL = "http://ice.radia.cz/rockzabava128.mp3";

//EXPRESS SK
//const char* streamURL = "http://stream.bauermedia.sk/expres-lo.mp3"; 

//NETWORK
//const char* streamURL = "http://192.168.0.106:8000/mystream";

//HOTSPOT
const char* streamURL = "http://10.42.0.1:8000/stream.mp3";

AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPStream *file;
AudioFileSourceBuffer *buff;
ESPAudioOutput *out;

uint8_t dac_pins[] = { 1, D8, D7, D6, D5, D4, D3, D2, D1};
static int currentStation = 0;

Ticker button;
bool next = false;

void IRAM_ATTR onTimerISR() {
  uint8_t val = out->read();
  uint16_t high = 0;
  uint16_t low = 0;

  if (val != 0) {
    
    for (int i = 0; i < ADC_RESOLUTION; i++) {
      if (val & (1 << i)) high |= (1 << dac_pins[i]);
      else low |= (1 << dac_pins[i]);
    }

    GPOS = high;
    GPOC = low;
  }
}

void startListening(const char* streamURL) {
  //noInterrupts();
  if (mp3 != NULL) {
    mp3->stop();
    delete mp3;
    delete file;
    delete buff;
    delete out;
   // mp3 = NULL;
  //  delete buff;
   // delete out;
  }
  delay(1000);
  Serial.println("next station");
  Serial.println(streamURL);

  file = new AudioFileSourceICYStream(streamURL);

  // Commented out for performance issues with high rate MP3 stream
  //file->RegisterMetadataCB(MDCallback, (void*)"ICY");

  buff = new AudioFileSourceBuffer(file, 4096);
  out = new ESPAudioOutput();
  mp3 = new AudioGeneratorMP3();
  

  bool success = mp3->begin(buff, out);
  Serial.println(success);
}


void checkButton() {
//  Serial.println("check");
 if (analogRead(A0) > 1000 && !next) {
    currentStation = (currentStation + 1) % RADIO_COUNT;
    next = true;
    mp3->pause();
   // startListening(stations[currentStation]);
 }
}

void setup() {
  Serial.begin(115200);
  button.attach(0.1, checkButton);

  pinMode(A0, INPUT);
 
  for (uint8_t pin : dac_pins) {
    pinMode(pin, OUTPUT);
  }


  /*int pin = 3;
  pinMode(pin, OUTPUT);


  for (int i = 0; i < 20; i++) {
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    delay(500);
  }*/



  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");


  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(" connected!");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup streaming and audio pipeline
  file = new AudioFileSourceICYStream(stations[currentStation]);

  // Commented out for performance issues with high rate MP3 stream
  //file->RegisterMetadataCB(MDCallback, (void*)"ICY");

  buff = new AudioFileSourceBuffer(file, 4096);
  out = new ESPAudioOutput();
  mp3 = new AudioGeneratorMP3();

  mp3->begin(buff, out);
 // analogWriteFreq(30000);

  noInterrupts();

  // Timer1 config: 80MHz / 1 = 80MHz, so 2.5µs tick = 25% of 40kHz PWM period
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);  // Use prescaler of 1
  timer1_write(1815*2);

  interrupts();

}

void loop() {

  if (mp3 != NULL && mp3->isRunning()) {
    mp3->loop();

  } else if (next) {
    Serial.println("MP3 next");
    Serial.println(currentStation);
    startListening(stations[currentStation]);
    next = false;
  } else {
    Serial.println("MP3 stopped");
    delay(1000);
  }
}




//#include <my_data.h>
/*
const int pwmPin = D1;             // PWM output pin (GPIO5)
const int resolution = 255;        // 8-bit resolution
const int tableSize = 256;         // Sine table resolution
const float waveFreq = 100.0;       // Sine wave frequency in Hz
const int sampleRate = 8000;       // Samples per second
const int amplitude = 127;         // Peak amplitude
const int offset = 128;            // Midpoint to get 0-255 range

uint8_t sineTable[tableSize];
unsigned long lastMicros = 0;
int idx = 0;

void setup() {
  Serial.begin(9600);
  analogWriteFreq(20000);
  pinMode(D1, OUTPUT);
  analogWrite(D1, 50);

  for (int i = 0; i < tableSize; i++) {
    float angle = 2 * PI * i / tableSize;
    sineTable[i] = offset + amplitude * sin(angle);
  }
}

void loop() {
  unsigned long now = micros();
  unsigned long interval = 1000000UL / sampleRate;  // microseconds

  if (now - lastMicros >= interval) {
    lastMicros = now;

    uint8_t val = (uint8_t)(((data[idx] + 10.0f) / 20.0f) * 255.0f);
    analogWrite(pwmPin, val);

    idx = (idx + 1) % tableSize;  // Loop around the table
  }
}*/