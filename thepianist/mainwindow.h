#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class MidiPlayer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void loadXML();
    bool eventFilter(QObject* object, QEvent* event) override;
    Ui::MainWindow* ui;
    QMap<QString, QPixmap> photos;
    MidiPlayer* player;
    bool isPlaying;

private slots:
    void playButtonPressed();
    void playMusic();
    void stopMusic();
    void setPiece() const;
    void setDevice(QAction* menuItem);
    void setInstrument(QAction* menuItem) const;
    void nextPiece();
};

#endif // MAINWINDOW_H
