#include <Arduino.h>
#include <AudioOutput.h>

#define BUFF_LEN 6000
#define VOLUME 0.14

class ESPAudioOutput : public AudioOutput {
public:

ESPAudioOutput() {
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
    buffer[write_index++ % BUFF_LEN] = sample[0];

    return true;
}

uint8_t read() {
    if (read_index > write_index) {
        return 0;
    }
    
    uint8_t val = (uint32_t)(buffer[++read_index % BUFF_LEN] + 32768) * 512 / 65535 * VOLUME;
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