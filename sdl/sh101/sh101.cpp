#include "sh101.h"

#include <SDL/SDL.h>
#include <cmath>
#include <cstring>
#include "note.h"

SH101::SH101()
{
  this->sampleFrequency = 44100;
  this->fase = 0;
  this->maxVolume = 128;
  this->arpeggioUpDown = true;
  this->position = 0;
  this->lfoTimer = SDL_GetTicks();
  for (int i = 0; i < 32; i++) {
    this->keyboard[i] = new Note(i + 9, &this->hold);
  }

  this->sequencerData.push_back(SequencerStep(40, false, false));
  this->sequencerData.push_back(SequencerStep(42, false, false));
  this->sequencerData.push_back(SequencerStep(44, false, false));
  this->sequencerData.push_back(SequencerStep(45, false, false));
  saveSequencer();
  loadSequencer();

  this->tune = 0.5f;
  this->lfoRate = 0.1f;
  this->lfoMode = LFO_TRIANGLE;
  this->oscMod = 0.0f;
  this->oscRange = OSC_RANGE_SIXTEEN;
  this->pwmValue = 0.0f;
  this->pwmMode = PWM_MAN;
  this->pwmVolume = 0.0f;
  this->sawVolume = 0.0f;
  this->subVolume = 1.0f;
  this->subMode = SUB_ONE_DOWN;
  this->noiseVolume = 0.0f;
  this->freqFilter = 1.0f;
  this->resonanceFilter = 0.0f;
  this->envFilter = 0.0f;
  this->modFilter = 0.0f;
  this->keybFilter = 0.0f;
  this->ampMode = AMP_GATE;
  this->envMode = ENV_GATE;
  this->attackEnv = 0.0f;
  this->decayEnv = 0.0f;
  this->sustainEnv = 1.0f;
  this->releaseEnv = 0.0f;
  this->volume = 0.5f;
  this->portamento = 0.0f;
  this->oscBender = 0.0f;
  this->filterBender = 0.0f;
  this->modBender = 0.0f;
  this->portamentoMode = PORTAMENTO_OFF;
  this->transposeMode = TRANSPOSE_MID;
  this->bender = 0.5f;
  this->sequencerMode = SEQUENCER_OFF;
  this->arpeggioMode = ARPEGGIO_OFF;
  this->hold = 0;
  this->keyTranspose = 0;
}

SH101::~SH101()
{
  for (int i = 0; i < 32; i++) delete this->keyboard[i];
}

void SH101::arpeggiator()
{
  if (this->arpeggioMode == ARPEGGIO_UP_AND_DOWN && Note::numNotesOn > 1 && this->position >= Note::numNotesOn) {
    this->position = 1;
    this->arpeggioUpDown = !this->arpeggioUpDown;
  } else {
    this->position %= Note::numNotesOn;
  }

  int note = -1;
  int noteOnCounter = 0;
  for (int i = 0; i < 32; i++) {
    int j;
    switch (this->arpeggioMode) {
      case ARPEGGIO_UP:
        j = i;
        break;
      case ARPEGGIO_UP_AND_DOWN:
        j = this->arpeggioUpDown ? 31 - i : i;
        break;
      case ARPEGGIO_DOWN:
      case ARPEGGIO_OFF:
        j = 31 - i;
        break;
    }
    if (this->keyboard[j]->on & (Note::ON|(this->hold?Note::HOLD:0))) {
      if (this->arpeggioMode == ARPEGGIO_OFF) {
        note = this->keyboard[j]->pitch;
        break;
      }
      if (this->position == noteOnCounter) {
        note = this->keyboard[j]->pitch;
      }
      noteOnCounter++;
    }
  }

  this->note = note;
}

void SH101::loadSequencer()
{
  SDL_RWops * file = SDL_RWFromFile("sh101.seq", "rb");
  if (!file) return;

  int length = SDL_RWseek(file, 0, SEEK_END);
  SDL_RWseek(file, 0, SEEK_SET);

  char * content = new char[length];
  SDL_RWread(file, content, 1, length);

  if (strncmp(content, "SH101", 5) == 0) {
    this->sequencerData.clear();
    for (int i = 0; i < length - 5; i++) {
      this->sequencerData.push_back(SequencerStep(content[i + 5], false, false));
    }
  }

  delete[] content;

  SDL_RWclose(file);
}

void SH101::saveSequencer()
{
  SDL_RWops * file = SDL_RWFromFile("sh101.seq", "wb");
  if (!file) return;

  const char * id = "SH101";
  SDL_RWwrite(file, id, 1, 5);

  const char sequence[5] = {40, 41, 42, 43, 44};
  SDL_RWwrite(file, sequence, 1, 5);

  SDL_RWclose(file);
}

void SH101::sequencer()
{
  this->arpeggioMode = ARPEGGIO_OFF;

  this->position %= this->sequencerData.size();

  this->note = this->sequencerData[this->position].pitch;
}

void SH101::pitch()
{
  float tune = 2 * this->tune - 1;
  float frequency = 440 * powf(powf(2, 1 / 12.0f), this->note - 49 + tune);
  this->bytesPerPeriode = int(this->sampleFrequency / frequency);
}

void SH101::lfo()
{
  int now = SDL_GetTicks();
  int lfoRateMs = int(1000 / (this->lfoRate * 29.9 + 0.1));

  this->lfoValue = float(now - this->lfoTimer) / lfoRateMs;
  this->lfoRateOnOff = this->lfoValue < 0.5f;

  if (now >= this->lfoTimer + lfoRateMs) {
    this->position++;
    this->lfoTimer = now;
  }
}

void SH101::output(unsigned char * stream, int length)
{
  this->note = NULL;
  this->lfo();

  if (this->sequencerMode == SEQUENCER_PLAY) {
    this->sequencer();
  } else {
    if (!Note::numNotesOn) return;
    this->arpeggiator();
  }
 
  if (this->note == NULL) return;

  switch (this->oscRange) {
    case OSC_RANGE_TWO:
      this->note += 36;
      break;
    case OSC_RANGE_FOUR:
      this->note += 24;
      break;
    case OSC_RANGE_EIGHT:
      this->note += 12;
      break;
  }

  this->pitch();

  for (int i = 0; i < length; i++) {
    int output = 0;
    output += pwm();
    output += saw();
    output += sub();
    output += noise();
    //output += env();
    output = int(output * this->volume);

    stream[i] = output;

    this->fase++;
  }
}

int SH101::pwm()
{
  int volume = int(this->pwmVolume * this->maxVolume);
  int fase = this->fase % this->bytesPerPeriode;
  int output = 0;
  float lfoValue = this->lfoValue < 0.5f ? 2 * this->lfoValue : 2 * (1 - this->lfoValue);

  switch (this->pwmMode) {
    case PWM_MAN:
      output = fase < (1 - this->pwmValue * 0.9f) * this->bytesPerPeriode / 2 ? volume : 0;
      break;
    case PWM_LFO:
      output = fase < (1 - this->pwmValue * 0.9f * lfoValue) * this->bytesPerPeriode / 2 ? volume : 0;
      break;
    case PWM_ENV:
      break;
  }
  return output;
}

int SH101::saw()
{
  int volume = int(this->sawVolume * this->maxVolume);
  int output = (this->fase % this->bytesPerPeriode) * volume / this->bytesPerPeriode;
  return output;
}

int SH101::sub()
{
  int volume = int(this->subVolume * this->maxVolume);
  int fase = this->fase;
  float pwmValue = 0.0f;

  switch (subMode) {
      case SUB_ONE_DOWN:
          fase /= 2;
          break;
      case SUB_TWO_DOWN:
          fase /= 4;
          break;
      case SUB_TWO_DOWN_PWM:
          fase /= 4;
          pwmValue = 0.6f;
          break;
  }
  fase %= this->bytesPerPeriode;

  int output = fase < ((1 - pwmValue * 0.9f) * this->bytesPerPeriode / 2) ? volume : 0;

  return output;
}

int SH101::noise()
{
  int volume = int(this->noiseVolume * this->maxVolume);
  int output = rand() * volume / RAND_MAX;
  return output;
}

void SH101::env()
{
}

void SH101::setNoteOn(int key, bool on)
{
  this->keyboard[key]->setOn(on);
  //this->on = on ? ON : *this->hold ? HOLD : OFF;
  //Note::numNotesOn += on ? 1 : -1;
}
