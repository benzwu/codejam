#include "pianokey.h"

#include <SDL/SDL.h>

#include "../note.h"

PianoKey::PianoKey(Note * note, Key key, SDL_Surface * images[], SDL_Surface * background, int x, int y)
  : note(note), key(key), images(images), Component(x, y, 0, 0, images[0], background)
{
  switch (key) {
    case A:
      this->hitArea.push_back(point(x, y + 128));
      this->hitArea.push_back(point(x + 11, y + 128));
      this->hitArea.push_back(point(x + 11, y));
      this->hitArea.push_back(point(x + 28, y));
      this->hitArea.push_back(point(x + 28, y + 128));
      this->hitArea.push_back(point(x + 35, y + 128));
      this->hitArea.push_back(point(x + 35, y + 205));
      this->hitArea.push_back(point(x, y + 205));
      break;
    case B:
      this->hitArea.push_back(point(x, y));
      this->hitArea.push_back(point(x + 35, y));
      this->hitArea.push_back(point(x + 35, y + 205));
      this->hitArea.push_back(point(x, y + 205));
      break;
    case C:
      this->hitArea.push_back(point(x, y));
      this->hitArea.push_back(point(x + 21, y));
      this->hitArea.push_back(point(x + 21, y + 128));
      this->hitArea.push_back(point(x + 35, y + 128));
      this->hitArea.push_back(point(x + 35, y + 205));
      this->hitArea.push_back(point(x, y + 205));
      break;
    case D:
      this->hitArea.push_back(point(x, y + 128));
      this->hitArea.push_back(point(x + 8, y + 128));
      this->hitArea.push_back(point(x + 8, y));
      this->hitArea.push_back(point(x + 28, y));
      this->hitArea.push_back(point(x + 28, y + 128));
      this->hitArea.push_back(point(x + 35, y + 128));
      this->hitArea.push_back(point(x + 35, y + 205));
      this->hitArea.push_back(point(x, y + 205));
      break;
    case E:
      this->hitArea.push_back(point(x, y + 128));
      this->hitArea.push_back(point(x + 14, y + 128));
      this->hitArea.push_back(point(x + 14, y));
      this->hitArea.push_back(point(x + 35, y));
      this->hitArea.push_back(point(x + 35, y + 205));
      this->hitArea.push_back(point(x, y + 205));
      break;
    case G:
      this->hitArea.push_back(point(x, y + 128));
      this->hitArea.push_back(point(x + 8, y + 128));
      this->hitArea.push_back(point(x + 8, y));
      this->hitArea.push_back(point(x + 25, y));
      this->hitArea.push_back(point(x + 25, y + 128));
      this->hitArea.push_back(point(x + 35, y + 128));
      this->hitArea.push_back(point(x + 35, y + 205));
      this->hitArea.push_back(point(x, y + 205));
      break;
    case SHARP:
      this->hitArea.push_back(point(x, y));
      this->hitArea.push_back(point(x + 20, y));
      this->hitArea.push_back(point(x + 20, y + 127));
      this->hitArea.push_back(point(x, y + 127));
      break;
    default:
      this->hitArea.push_back(point(x, y));
      this->hitArea.push_back(point(x + 1, y));
      this->hitArea.push_back(point(x + 1, y + 1));
      this->hitArea.push_back(point(x, y + 1));
      break;
  }
  this->render();
}

void PianoKey::setValue(int x, int y, int mouseEvent)
{
  if (mouseEvent == SDL_MOUSEBUTTONDOWN) {
    this->note->setOn(true);
  }
  if (mouseEvent == SDL_MOUSEBUTTONUP) {
    this->note->setOn(false);
  }
  this->render();
}

void PianoKey::render()
{
  this->image = this->note->on & Note::ON ? this->images[1] : this->images[0];
  Component::render(false);
}
