#include <signal.h>
#include <stdio.h>
#include <wiringPi.h>

volatile int run = 1;
void intHandler(int i) { run = 0; }

int v1_frequency = 0x0;
int v2_frequency = 0x0;
int v3_frequency = 0x0;
int v1_waveform = 3;
int v2_waveform = 3;
int v3_waveform = 3;
int v1_pulsewidth = 0x0;
int v2_pulsewidth = 0x0;
int v3_pulsewidth = 0x0;
int v1_attack = 0x0;
int v2_attack = 0x0;
int v3_attack = 0x0;
int v1_decay = 0x0;
int v2_decay = 0x0;
int v3_decay = 0x0;
int v1_sustain = 0xf;
int v2_sustain = 0x0;
int v3_sustain = 0x0;
int v1_release = 0x0;
int v2_release = 0x0;
int v3_release = 0x0;
int filtertype = 3;
int cutoff = 0x7ff;
int resonance = 0;
int volume = 0xf;

void status()
{
    const char *waveforms[] = { "rnd", "|_", "/|", "/\\" };
    const char *filters[] = { "OFF", "HP", "BP", "LP" };

    printf("VOICE #1\t\tVOICE #2\t\tVOICE #3\n");
    printf("Frequency: %04x\t\t", v1_frequency);
    printf("Frequency: %04x\t\t", v2_frequency);
    printf("Frequency: %04x\n", v3_frequency);
    printf("Waveform:  %s\t\t", waveforms[v1_waveform]);
    printf("Waveform:  %s\t\t", waveforms[v2_waveform]);
    printf("Waveform:  %s\n", waveforms[v3_waveform]);
    printf("Pulse W:   %03x\t\t", v1_pulsewidth);
    printf("Pulse W:   %03x\t\t", v2_pulsewidth);
    printf("Pulse W:   %03x\n", v3_pulsewidth);
    printf("Env/ADSR:  %02x %02x %02x %02x\t", v1_attack, v1_decay, v1_sustain, v1_release);
    printf("Env/ADSR:  %02x %02x %02x %02x\t", v2_attack, v2_decay, v2_sustain, v2_release);
    printf("Env/ADSR:  %02x %02x %02x %02x\n", v3_attack, v3_decay, v3_sustain, v3_release);
    printf("\n");
    printf("FILTER\n");
    printf("Cutoff:    %02x\t\t", cutoff);
    printf("Resonance: %x\t\t", resonance);
    printf("Type:      %s\n", filters[filtertype]);
    printf("Volume:    %x\n", volume);
    
    // fill remaining 24 lines
    for (int i = 0; i < 14; i++,printf("\n"));
}

#define LedPin 0

void update()
{
    //digitalWrite(LedPin, LOW);
    //digitalWrite(LedPin, HIGH);
}

int main()
{
    if (wiringPiSetup() == -1) {
        printf("Failed setting up wiringPi.\n");
        return 1;
    }

    pinMode(LedPin, OUTPUT);

    signal(SIGINT, intHandler);

    while (run) {
        v1_frequency = rand() % 0xffff;
        status();
        update();
        delay(100);
    }

    return 0;
}