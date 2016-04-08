#ifndef SH101_H
#define SH101_H

#include <vector>

class Note;

class SH101
{
public:
  SH101();
  ~SH101();
  void output(unsigned char * stream, int length);
  void setNoteOn(int key, bool on);

  Note * keyboard[32];
  int sampleFrequency;
  int maxVolume;
  int lfoRateOnOff;

  enum LfoMode { LFO_TRIANGLE, LFO_PULSE, LFO_RANDOM, LFO_NOISE };
  enum OscRange { OSC_RANGE_TWO, OSC_RANGE_FOUR, OSC_RANGE_EIGHT, OSC_RANGE_SIXTEEN };
  enum PwmMode { PWM_LFO, PWM_MAN, PWM_ENV };
  enum SubMode { SUB_ONE_DOWN, SUB_TWO_DOWN, SUB_TWO_DOWN_PWM };
  enum AmpMode { AMP_GATE, AMP_ENV };
  enum EnvMode { ENV_GATE_AND_TRIG, ENV_GATE, ENV_LFO };
  enum PortamentoMode { PORTAMENTO_AUTO, PORTAMENTO_OFF, PORTAMENTO_ON };
  enum TransposeMode { TRANSPOSE_LOW, TRANSPOSE_MID, TRANSPOSE_HIGH };
  enum SequencerMode { SEQUENCER_OFF, SEQUENCER_PLAY, SEQUENCER_LOAD };
  enum ArpeggioMode { ARPEGGIO_OFF, ARPEGGIO_UP, ARPEGGIO_DOWN, ARPEGGIO_UP_AND_DOWN };

  float tune;
  float lfoRate;
  int lfoMode;
  float oscMod;
  int oscRange;
  float pwmValue;
  int pwmMode;
  float pwmVolume;
  float sawVolume;
  float subVolume;
  int subMode;
  float noiseVolume;
  float freqFilter;
  float resonanceFilter;
  float envFilter;
  float modFilter;
  float keybFilter;
  int ampMode;
  int envMode;
  float attackEnv;
  float decayEnv;
  float sustainEnv;
  float releaseEnv;
  float volume;
  float portamento;
  float oscBender;
  float filterBender;
  float modBender;
  int portamentoMode;
  int transposeMode;
  float bender;
  int sequencerMode;
  int arpeggioMode;
  int hold;
  int keyTranspose;

private:
  struct SequencerStep {
    SequencerStep(int pitch, bool legato, bool rest)
      : pitch(pitch), legato(legato), rest(rest) {}
    int pitch;
    bool legato;
    bool rest;
  };

  void sequencer();
  void loadSequencer();
  void saveSequencer();
  void arpeggiator();
  void pitch();
  void lfo();
  void env();

  int pwm();
  int saw();
  int sub();
  int noise();

  int fase;
  int bytesPerPeriode;

  int note;
  int lfoTimer;
  float lfoValue;

  int position;
  bool arpeggioUpDown;
  std::vector<SequencerStep> sequencerData;
};

#endif
