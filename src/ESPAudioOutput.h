#include <Arduino.h>
#include <AudioOutput.h>


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

void setCurrentDutyVar(volatile uint16_t* currentDuty) {
    this->currentDuty = currentDuty;
}



bool ConsumeSample(int16_t sample[2]) {
   // Serial.print("sample: ");
   // Serial.println(sample[0]);
   // uint8_t mappedValue = (uint8_t)(((sample[0] + 32768) / 65535.0) * 255);
    uint8_t val = (uint32_t)(sample[0] + 32768) * 16 / 65535;

//if (counter++ % 4 == 0) {
    digitalWrite(D1, val & 0b0001);
    digitalWrite(D2, val & 0b0010);
    digitalWrite(D3, val & 0b0100);
    digitalWrite(D4, val & 0b1000);

//}

    return true;
}

void flush() {
    
}

bool stop() {
    return true;
}



private:
    int counter;
    volatile uint16_t *currentDuty;
};