#ifndef NOTE_H
#define NOTE_H

class Note {
public:
  enum State { OFF=0, ON=1, HOLD=2 };

  Note(int pitch, const int * hold);
  void setOn(bool on);
  State on;
  int pitch;
  static int numNotesOn;

private:
  const int * hold;
};

#endif
