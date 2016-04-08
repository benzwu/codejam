#include "pianoroll.h"

#include <QPainter>

PianoRoll::PianoRoll(QWidget* parent) : QLabel(parent)
{
    keysPixmap = QPixmap(":/images/keys.png");
}

void PianoRoll::noteOn(int pitch)
{
    keysDown.insert(pitch);
    update();
}

void PianoRoll::noteOff(int pitch)
{
    keysDown.remove(pitch);
    update();
}

void PianoRoll::noteOffAll()
{
    keysDown.clear();
    update();
}

void PianoRoll::paintNoteOn()
{
    QPainter painter(this);
    painter.drawTiledPixmap(rect(), keysPixmap);
    foreach (int key, keysDown) {
        key -= 12;
        int x = keysPixmap.width() * (key / 12) + 7 * (key % 12) + 5;
        if (key % 12 > 4)
            x += 7;
        int y = 16;
        if (key % 12 == 1 || key % 12 == 3 || key % 12 == 6 || key % 12 == 8 || key % 12 == 10)
            y -= 11;
        painter.fillRect(x, y, 6, 6, Qt::red);
    }
}

void PianoRoll::paintEvent(QPaintEvent*)
{
    paintNoteOn();
}
