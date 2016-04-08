#ifndef BLAGGER_H
#define BLAGGER_H

#include "tinygamelib.h"

class Blagger
{
public:
    Blagger();

private:
    void setLevel(int level);
    TinyGameLibrary tgl;
};

#endif