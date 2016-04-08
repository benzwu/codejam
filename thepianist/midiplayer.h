#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <QThread>
#include <QMidiOut.h>
#include <QMidiFile.h>

class MidiPlayer : public QThread
{
    Q_OBJECT
public:
    enum Instrument
    {
        PIANO = 0,
        HARPSICHORD = 6,
        GLOCKENSPIEL = 9,
        CHURCH_ORGAN = 19,
        PIZZICATO = 45
    };


    MidiPlayer();
    void loadFile(const QString & filename) const;
    void stop();
    static QMap<QString, QString> devices();

signals:
    void noteOn(int);
    void noteOff(int);
    void done();

public slots:
    void setDevice(const QString & deviceId);
    void setInstrument(int instrumentId);

private:
    QMidiFile* midiFile;
    QMidiOut* midiOut;
    QString deviceId;
    int instrument;

protected:
    void run() override;
};

#endif // MIDIPLAYER_H
