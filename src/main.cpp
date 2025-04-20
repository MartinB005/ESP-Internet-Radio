#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <AudioFileSourceBuffer.h>
#include <AudioFileSourceICYStream.h>
#include <AudioGeneratorMP3.h>
#include <ESPAudioOutput.h>
#include <AudioOutputSPDIF.h>

// WiFi credentials
const char* ssid = "DDAM3M";
const char* password = "matus987";

volatile uint16_t pwmCounter = 0;
volatile uint16_t currentDuty = 512;

// Stream URL (use Icecast/MP3 stream)
const char* streamURL = "http://stream.bauermedia.sk/expres-lo.mp3";  // Example

AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPStream *file;
AudioFileSourceBuffer *buff;
ESPAudioOutput *out;


void IRAM_ATTR onTimerISR() {
 /* pwmCounter++;
  if (pwmCounter < currentDuty) {
    GPOS = (1 << D1); // Set pin HIGH
  } else {
    GPOC = (1 << D1); // Set pin LOW
  }
  if (pwmCounter >= 255) pwmCounter = 0;*/

  analogWrite(D1, currentDuty);
}


void setup() {
  Serial.begin(115200);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(" connected!");

  // Setup streaming and audio pipeline
  file = new AudioFileSourceICYStream(streamURL);

  // Commented out for performance issues with high rate MP3 stream
  //file->RegisterMetadataCB(MDCallback, (void*)"ICY");

  buff = new AudioFileSourceBuffer(file, 4096);
  out = new ESPAudioOutput(8000L);
  mp3 = new AudioGeneratorMP3();

  mp3->begin(buff, out);
 // analogWriteFreq(30000);

 /* noInterrupts();

  // Timer1 config: 80MHz / 1 = 80MHz, so 2.5µs tick = 25% of 40kHz PWM period
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  timer1_write(50); // 200 ticks = 2.5µs, for 400kHz PWM resolution

  interrupts();*/


  out->setCurrentDutyVar(&currentDuty);
}

void loop() {
  if (mp3->isRunning()) {
    mp3->loop();
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