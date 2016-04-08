#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QLabel>
#include <QSet>

class PianoRoll : public QLabel
{
    Q_OBJECT
public:
    explicit PianoRoll(QWidget* parent = nullptr);

signals:

public slots:
    void noteOn(int pitch);
    void noteOff(int pitch);
    void noteOffAll();

private:
    void paintNoteOn();
    QSet<int> keysDown;
    QPixmap keysPixmap;

protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // PIANOROLL_H
