#include "note.h"

int Note::numNotesOn = 0;

Note::Note(int pitch, const int * hold) : pitch(pitch), hold(hold), on(OFF) {}

void Note::setOn(bool on) {
  this->on = on ? ON : *this->hold ? HOLD : OFF;
  Note::numNotesOn += on ? 1 : -1;
}
