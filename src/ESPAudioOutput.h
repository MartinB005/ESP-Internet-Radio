#include <Arduino.h>
#include <AudioOutput.h>

#define BUFF_LEN 6000

class ESPAudioOutput : public AudioOutput {
public:

ESPAudioOutput(long sampleRate) {
    SetGain(1.0);
}

~ESPAudioOutput() {
    stop();
}

bool SetRate(int hz) {
    this->hertz = hz;
    return true;
}

bool SetBitsPerSample(int bits) {
    if ((bits != 16) && (bits != 8)) {
        return false;
    }
    this->bps = bits;
    return true;
}

bool SetChannels(int channels) {
    if ((channels < 1) || (channels > 2)) {
        return false;
    }
    this->channels = channels;
    return true;
}

bool SetOutputModeMono(bool mono) {

    return true;
}

bool begin() {
    Serial.println("begin");
   //SetRate(44100); // Default
    return true;
}

int16_t* getBufferPtr() {
    return &buffer[write_index++ % BUFF_LEN];
}

void incrementPtr(int val) {
    write_index += val;
}

bool bufferAvailable() {
    return write_index - read_index < BUFF_LEN;
}

bool ConsumeSample(int16_t sample[2]) {
   // Serial.print("sample: ");
   // Serial.println(sample[0]);
   // uint8_t mappedValue = (uint8_t)(((sample[0] + 32768) / 65535.0) * 255);
    buffer[write_index++ % BUFF_LEN] = sample[0];

  /*  while(micros() - lastMicros < 10);
    lastMicros = micros();
//if (counter++ % 4 == 0) {
    digitalWrite(D0, val & 0b00001);
    digitalWrite(D1, val & 0b00010);
    digitalWrite(D2, val & 0b00100);
    digitalWrite(D3, val & 0b01000);
    digitalWrite(D4, val & 0b10000);*/
//}

    return true;
}

uint8_t read() {
    if (read_index > write_index) {
        return 0;
    }
    
    uint8_t val = (uint32_t)(buffer[++read_index % BUFF_LEN] + 32768) * 256 / 65535;
    //if (val >= 36) return 0;
   //  if (read_index > 100000) Serial.println("!!!");
   // Serial.println((uint32_t)(buffer[++read_index % BUFF_LEN] + 32768) * 64 / 65535);
    return  val;
}

void flush() {
    
}

bool stop() {
    return true;
}



private:
    int16_t buffer[BUFF_LEN];
    int write_index = 0;
    int read_index = -1;
};