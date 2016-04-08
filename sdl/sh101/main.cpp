#include <SDL/SDL.h>
#include <cmath>

#include "sh101.h"
#include "controller.h"
#include "view.h"

SH101 * sh101 = NULL;

void
audioCB(void * data, Uint8 * stream, int length)
{
  if (sh101 == NULL) return;
  sh101->output(stream, length);
}

int
main(int argc, char * argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    printf("main init error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Surface * screen = SDL_SetVideoMode(800, 480, 32, SDL_HWSURFACE);
  if (screen == NULL) {
    printf("video init error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_AudioSpec * audioSpecDesired = new SDL_AudioSpec;
  audioSpecDesired->freq = 44100;
  audioSpecDesired->format = AUDIO_S16SYS;
  audioSpecDesired->channels = 1;
  audioSpecDesired->samples = 1024;
  audioSpecDesired->callback = audioCB;
  audioSpecDesired->userdata = NULL;
  SDL_AudioSpec * audioSpecObtained = new SDL_AudioSpec;
  if (SDL_OpenAudio(audioSpecDesired, audioSpecObtained) == -1) {
    printf("audio init error: %s\n", SDL_GetError());
    return 1;
  }
  delete audioSpecDesired;
  SDL_PauseAudio(0);
    
  sh101 = new SH101;
  sh101->sampleFrequency = audioSpecObtained->freq;
  sh101->maxVolume = SDL_MIX_MAXVOLUME;

  View * view = new View(sh101);
  Controller * controller = new Controller(sh101, view);

  while (controller->running) {
    controller->processEvents();
    SDL_Delay(1000 / 30);
  }

  delete controller;
  delete view;
  delete sh101;
  SDL_Quit();

  return 0;
}
