#include "midiplayer.h"

#include <QElapsedTimer>

MidiPlayer::MidiPlayer()
{
    instrument = PIANO;
    midiFile = new QMidiFile();
    midiOut = new QMidiOut();
}

void MidiPlayer::loadFile(const QString & filename) const
{
    midiFile->load(filename);
}

void MidiPlayer::setDevice(const QString & deviceId)
{
    this->deviceId = deviceId;
}

void MidiPlayer::setInstrument(int instrumentId)
{
    this->instrument = instrumentId;
    midiOut->setInstrument(0, instrument);
}

void MidiPlayer::run()
{
    midiOut->connect(deviceId);
    midiOut->setInstrument(0, instrument);

    QElapsedTimer t;
    t.start();
    QList<QMidiEvent*> events = midiFile->events();
    for (QMidiEvent* e : events) {
        if (e->type() != QMidiEvent::Meta) {
            qint64 event_time = midiFile->timeFromTick(e->tick()) * 1000;

            qint32 waitTime = event_time - t.elapsed();
            if (waitTime > 0) {
                msleep(waitTime);
            }
            if (e->type() != QMidiEvent::SysEx) {
                qint32 message = e->message();
                midiOut->sendMsg(message);

                if (e->type() == QMidiEvent::NoteOn)
                    emit noteOn(e->note());
                if (e->type() == QMidiEvent::NoteOff)
                    emit noteOff(e->note());
            }
        }
    }

    midiOut->disconnect();
    emit done();
}

void MidiPlayer::stop()
{
    terminate();
    midiOut->stopAll();
    midiOut->disconnect();
}

QMap<QString, QString> MidiPlayer::devices()
{
    return QMidiOut::devices();
}
