#ifndef PIANOKEY_H
#define PIANOKEY_H

#include "component.h"

class Note;

class PianoKey : public Component
{
public:
  enum Key { A, B, C, D, E, G, SHARP };
  PianoKey(Note * note, Key key, SDL_Surface * images[], SDL_Surface * background, int x, int y);
  void setValue(int x, int y, int mouseEvent);
  void render();
  SDL_Surface ** images;
  Note * note;
  Key key;
};

#endif
