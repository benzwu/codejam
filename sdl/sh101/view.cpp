#include "view.h"

#include <SDL/SDL.h>

#include "sh101.h"
#include "components/bender.h"
#include "components/button.h"
#include "components/component.h"
#include "components/handle.h"
#include "components/knob.h"
#include "components/knobswitch.h"
#include "components/light.h"
#include "components/pianokey.h"
#include "components/slider.h"
#include "components/sliderswitch.h"

View::View(SH101 * sh101) : sh101(sh101)
{
  // load sprites
  SDL_Surface * sprites = SDL_LoadBMP("images/gray.bmp");
  if (sprites == NULL) {
    printf("bmp load error: %s\n", SDL_GetError());
    exit(1);
  }
  this->background = this->getSprite(0, 0, 800, 480, sprites);
  for (int i = 0; i < 21; i++) this->knob[i] = this->getSprite(i * 29, 481, 29, 29, sprites);
  for (int i = 0; i < 3; i++) this->sliders[i] = this->getSprite(i * 10, 511, 10, 29, sprites);
  for (int i = 0; i < 2; i++) this->button[i] = this->getSprite(31, i * 12 + 514, 20, 12, sprites);
  for (int i = 0; i < 2; i++) this->light[i] = this->getSprite(52, i * 5 + 521, 9, 5, sprites);
  this->handle = this->getSprite(62, 522, 8, 8, sprites);
  this->bender = this->getSprite(62, 522, 8, 8, sprites);
  for (int i = 0; i < 6; i++) {
    this->pianoKeys[i][0] = this->getSprite(2 * i * 35, 541, 35, 205, sprites);
    this->pianoKeys[i][1] = this->getSprite((2 * i + 1) * 35, 541, 35, 205, sprites);
  }
  this->pianoKeys[6][0] = this->getSprite(421, 541, 20, 127, sprites);
  this->pianoKeys[6][1] = this->getSprite(441, 541, 20, 127, sprites);
  SDL_FreeSurface(sprites);

  // paint background
  SDL_Surface * screen = SDL_GetVideoSurface();
  SDL_BlitSurface(this->background, NULL, screen, NULL);
  SDL_Flip(screen);

  // make sliders, knobs, buttons etc.
  this->components.push_back(new Knob(&this->sh101->tune, this->knob, this->background, 41, 112));
  this->components.push_back(new Slider(&this->sh101->lfoRate, this->sliders[GREEN], this->background, 93, 86, 80));
  //this->components.push_back(new KnobSwitch(&this->sh101->lfoMode, this->knob, this->background, 126, 112));
  //this->components.push_back(new Slider(&this->sh101->oscMod, this->sliders[ORANGE], this->background, 186, 86, 80));
  this->components.push_back(new KnobSwitch(&this->sh101->oscRange, this->knob, this->background, 217, 112));
  this->components.push_back(new Slider(&this->sh101->pwmValue, this->sliders[ORANGE], this->background, 263, 86, 80));
  this->components.push_back(new SliderSwitch(&this->sh101->pwmMode, this->sliders[YELLOW], this->background, 290, 86, 80, 3));
  this->components.push_back(new Slider(&this->sh101->pwmVolume, this->sliders[GREEN], this->background, 335, 86, 80));
  this->components.push_back(new Slider(&this->sh101->sawVolume, this->sliders[GREEN], this->background, 357, 86, 80));
  this->components.push_back(new Slider(&this->sh101->subVolume, this->sliders[GREEN], this->background, 379, 86, 80));
  this->components.push_back(new SliderSwitch(&this->sh101->subMode, this->sliders[YELLOW], this->background, 406, 86, 80, 3));
  this->components.push_back(new Slider(&this->sh101->noiseVolume, this->sliders[GREEN], this->background, 452, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->freqFilter, this->sliders[ORANGE], this->background, 487, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->resonanceFilter, this->sliders[ORANGE], this->background, 509, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->envFilter, this->sliders[ORANGE], this->background, 540, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->modFilter, this->sliders[ORANGE], this->background, 562, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->keybFilter, this->sliders[ORANGE], this->background, 584, 86, 80));
  //this->components.push_back(new SliderSwitch(&this->sh101->ampMode, this->sliders[YELLOW], this->background, 629, 86, 80, 2));
  //this->components.push_back(new SliderSwitch(&this->sh101->envMode, this->sliders[YELLOW], this->background, 670, 86, 80, 3));
  //this->components.push_back(new Slider(&this->sh101->attackEnv, this->sliders[ORANGE], this->background, 695, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->decayEnv, this->sliders[ORANGE], this->background, 717, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->sustainEnv, this->sliders[ORANGE], this->background, 739, 86, 80));
  //this->components.push_back(new Slider(&this->sh101->releaseEnv, this->sliders[ORANGE], this->background, 761, 86, 80));
  this->components.push_back(new Knob(&this->sh101->volume, this->knob, this->background, 16, 267));
  //this->components.push_back(new Knob(&this->sh101->portamento, this->knob, this->background, 84, 267));
  //this->components.push_back(new Slider(&this->sh101->oscBender, this->sliders[YELLOW], this->background, 12, 317, 63));
  //this->components.push_back(new Slider(&this->sh101->filterBender, this->sliders[YELLOW], this->background, 33, 317, 63));
  //this->components.push_back(new Slider(&this->sh101->modBender, this->sliders[YELLOW], this->background, 55, 317, 63));
  this->components.push_back(new Light(&this->sh101->sequencerMode, this->light, this->background, 145, 185, SH101::SEQUENCER_LOAD));
  this->components.push_back(new Button(&this->sh101->sequencerMode, this->button, this->background, 139, 194, SH101::SEQUENCER_LOAD));
  this->components.push_back(new Light(&this->sh101->sequencerMode, this->light, this->background, 168, 185, SH101::SEQUENCER_PLAY));
  this->components.push_back(new Button(&this->sh101->sequencerMode, this->button, this->background, 163, 194, SH101::SEQUENCER_PLAY));
  //this->components.push_back(new Light(&this->sh101->arpeggioMode, this->light, this->background, 200, 185, SH101::ARPEGGIO_DOWN));
  //this->components.push_back(new Button(&this->sh101->arpeggioMode, this->button, this->background, 195, 194, SH101::ARPEGGIO_DOWN));
  //this->components.push_back(new Light(&this->sh101->arpeggioMode, this->light, this->background, 223, 185, SH101::ARPEGGIO_UP_AND_DOWN));
  //this->components.push_back(new Button(&this->sh101->arpeggioMode, this->button, this->background, 218, 194, SH101::ARPEGGIO_UP_AND_DOWN));
  //this->components.push_back(new Light(&this->sh101->arpeggioMode, this->light, this->background, 246, 185, SH101::ARPEGGIO_UP));
  //this->components.push_back(new Button(&this->sh101->arpeggioMode, this->button, this->background, 241, 194, SH101::ARPEGGIO_UP));
  //this->components.push_back(new Light(&this->sh101->hold, this->light, this->background, 281, 185));
  //this->components.push_back(new Button(&this->sh101->hold, this->button, this->background, 276, 194));
  //this->components.push_back(new Light(&this->sh101->keyTranspose, this->light, this->background, 316, 185));
  //this->components.push_back(new Button(&this->sh101->keyTranspose, this->button, this->background, 311, 194));
  //this->components.push_back(new Handle(&this->sh101->portamentoMode, this->handle, this->background, 83, 327));
  //this->components.push_back(new Handle(&this->sh101->transposeMode, this->handle, this->background, 83, 363));
  //this->components.push_back(new Bender(&this->sh101->bender, this->bender, this->background, 31, 418));
  this->components.push_back(new PianoKey(this->sh101->keyboard[0], PianoKey::C, this->pianoKeys[PianoKey::C], this->background, 128, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[1], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 150, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[2], PianoKey::G, this->pianoKeys[PianoKey::G], this->background, 163, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[3], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 188, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[4], PianoKey::A, this->pianoKeys[PianoKey::A], this->background, 198, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[5], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 226, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[6], PianoKey::E, this->pianoKeys[PianoKey::E], this->background, 233, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[7], PianoKey::C, this->pianoKeys[PianoKey::C], this->background, 268, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[8], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 290, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[9], PianoKey::D, this->pianoKeys[PianoKey::D], this->background, 303, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[10], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 331, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[11], PianoKey::E, this->pianoKeys[PianoKey::E], this->background, 338, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[12], PianoKey::C, this->pianoKeys[PianoKey::C], this->background, 373, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[13], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 395, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[14], PianoKey::G, this->pianoKeys[PianoKey::G], this->background, 408, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[15], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 433, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[16], PianoKey::A, this->pianoKeys[PianoKey::A], this->background, 443, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[17], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 471, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[18], PianoKey::E, this->pianoKeys[PianoKey::E], this->background, 478, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[19], PianoKey::C, this->pianoKeys[PianoKey::C], this->background, 513, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[20], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 535, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[21], PianoKey::D, this->pianoKeys[PianoKey::D], this->background, 548, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[22], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 576, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[23], PianoKey::E, this->pianoKeys[PianoKey::E], this->background, 583, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[24], PianoKey::C, this->pianoKeys[PianoKey::C], this->background, 618, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[25], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 640, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[26], PianoKey::G, this->pianoKeys[PianoKey::G], this->background, 653, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[27], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 678, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[28], PianoKey::A, this->pianoKeys[PianoKey::A], this->background, 688, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[29], PianoKey::SHARP, this->pianoKeys[PianoKey::SHARP], this->background, 716, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[30], PianoKey::E, this->pianoKeys[PianoKey::E], this->background, 723, 243));
  this->components.push_back(new PianoKey(this->sh101->keyboard[31], PianoKey::B, this->pianoKeys[PianoKey::B], this->background, 758, 243));
  this->components.push_back(new Light(&this->sh101->lfoRateOnOff, this->light, this->background, 112, 78));
}

View::~View()
{
  SDL_FreeSurface(this->background);
  for (int i = 0; i < 21; i++) SDL_FreeSurface(this->knob[i]);
  for (int i = 0; i < 3; i++) SDL_FreeSurface(this->sliders[i]);
  for (int i = 0; i < 2; i++) SDL_FreeSurface(this->button[i]);
  for (int i = 0; i < 2; i++) SDL_FreeSurface(this->light[i]);
  SDL_FreeSurface(this->handle);
  SDL_FreeSurface(this->bender);
  for (int i = 0; i < 7; i++) {
    SDL_FreeSurface(this->pianoKeys[i][0]);
    SDL_FreeSurface(this->pianoKeys[i][1]);
  }
  for (unsigned int i = 0; i < this->components.size(); i++) {
    delete this->components[i];
  }
}

int
View::pick(int x, int y) const
{
  for (unsigned int i = 0; i < this->components.size(); i++) {
    if (this->components.at(i)->intersects(x, y)) return i;
  }
  return -1;
}

SDL_Surface *
View::getSprite(int x, int y, int width, int height, SDL_Surface * sprites)
{
  SDL_Rect spriteRect = {x, y, width, height};
  SDL_Surface * sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
  SDL_BlitSurface(sprites, &spriteRect, sprite, NULL);
  SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, 0xff00ff);
  return sprite;
}
