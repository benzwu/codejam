#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDomDocument>
#include <QKeyEvent>

#include "midiplayer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new MidiPlayer();

    connect(ui->songList, SIGNAL(itemSelectionChanged()), this, SLOT(setPiece()));
    connect(player, SIGNAL(noteOn(int)), ui->pianoRoll, SLOT(noteOn(int)));
    connect(player, SIGNAL(noteOff(int)), ui->pianoRoll, SLOT(noteOff(int)));
    connect(player, SIGNAL(done()), this, SLOT(nextPiece()));
    connect(ui->songList, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(playMusic()));
    connect(ui->menuFile->actions()[0], SIGNAL(triggered()), this, SLOT(close()));

    setFixedSize(size());

    ui->songList->installEventFilter(this);

    QAction* menuItemPiano = ui->menuInstrument->addAction("Piano");
    menuItemPiano->setData(0);
    menuItemPiano->setCheckable(true);
    QAction* menuItemHarpsichord = ui->menuInstrument->addAction("Harpsichord");
    menuItemHarpsichord->setData(6);
    menuItemHarpsichord->setCheckable(true);
    QAction* menuItemGlockenspiel = ui->menuInstrument->addAction("Glockenspiel");
    menuItemGlockenspiel->setData(9);
    menuItemGlockenspiel->setCheckable(true);
    QAction* menuItemOrgan = ui->menuInstrument->addAction("Church Organ");
    menuItemOrgan->setData(19);
    menuItemOrgan->setCheckable(true);
    QAction* menuItemPizzicato = ui->menuInstrument->addAction("Pizzicato");
    menuItemPizzicato->setData(45);
    menuItemPizzicato->setCheckable(true);
    connect(ui->menuInstrument, SIGNAL(triggered(QAction*)), this, SLOT(setInstrument(QAction*)));
    setInstrument(ui->menuInstrument->actions().at(0));

    for (QString deviceId : player->devices().keys())
    {
        QString deviceName = player->devices()[deviceId];
        QAction* menuItem = ui->menuMidiOut->addAction(deviceId + " - " + deviceName);
        menuItem->setData(deviceId);
        menuItem->setCheckable(true);
    }
    connect(ui->menuMidiOut, SIGNAL(triggered(QAction*)), this, SLOT(setDevice(QAction*)));
    setDevice(ui->menuMidiOut->actions().at(0));

    connect(ui->playButton, SIGNAL(pressed()), this, SLOT(playButtonPressed()));

    loadXML();

    isPlaying = false;

    ui->songList->setFocus();
}

MainWindow::~MainWindow()
{
    player->stop();
    delete player;
    delete ui;
}

void MainWindow::loadXML()
{
    QFile file("data/database.xml");
    file.open(QIODevice::ReadOnly);

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    QDomNodeList pieces = doc.elementsByTagName("piece");
    int numPieces = pieces.size();

    ui->songList->setRowCount(numPieces);
    ui->songList->setColumnCount(5);
    ui->songList->hideColumn(2);
    ui->songList->hideColumn(3);
    ui->songList->hideColumn(4);

    for (int i = 0; i < numPieces; i++) {
        QDomElement element = pieces.item(i).toElement();
        QString name = element.attribute("name");
        QString componist = element.attribute("componist");
        QString midiFile = element.attribute("midifile");
        QString photo = element.attribute("photo");
        QString text = element.text();

        text = text.replace(QRegExp("\n"), " ").replace(QRegExp("  +"), " ").trimmed();

        ui->songList->setItem(i, 0, new QTableWidgetItem(name));
        ui->songList->setItem(i, 1, new QTableWidgetItem(componist));
        ui->songList->setItem(i, 2, new QTableWidgetItem(midiFile));
        ui->songList->setItem(i, 3, new QTableWidgetItem(photo));
        ui->songList->setItem(i, 4, new QTableWidgetItem(text));

        if (!photos.contains(photo))
            photos[photo] = QPixmap("data/" + photo);
    }

    ui->songList->setColumnWidth(0, 600);
    ui->songList->selectRow(0);
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            playMusic();
        }
    }

    return QObject::eventFilter(object, event);
}

void MainWindow::playMusic()
{
    QModelIndexList selection = ui->songList->selectionModel()->selectedIndexes();
    if (selection.count() == 0) return;

    int row = selection.first().row();
    QString midiFile = "data/" + ui->songList->item(row, 2)->text();

    stopMusic();
    player->loadFile(midiFile);
    player->start();

    ui->playButton->setText("Stop");
    isPlaying = true;
}

void MainWindow::stopMusic()
{
    ui->pianoRoll->noteOffAll();
    player->stop();

    ui->playButton->setText("Play");
    isPlaying = false;
}

void MainWindow::setPiece() const
{
    QModelIndexList selection = ui->songList->selectionModel()->selectedIndexes();
    if (selection.count() == 0) return;

    int row = selection.first().row();
    QString photo = ui->songList->item(row, 3)->text();
    QString text = ui->songList->item(row, 4)->text();

    ui->composerImage->setPixmap(photos[photo]);
    ui->composerText->setText(text);
}

void MainWindow::setDevice(QAction* menuItem)
{
    QString deviceId = menuItem->data().toString();
    stopMusic();
    player->setDevice(deviceId);
    setInstrument(ui->menuInstrument->actions().at(0));
    playMusic();

    for (QAction* it : ui->menuMidiOut->actions())
        it->setChecked(false);
    menuItem->setChecked(true);
}

void MainWindow::setInstrument(QAction* menuItem) const
{
    int instrumentId = menuItem->data().toInt();
    player->setInstrument(instrumentId);

    for (QAction* it : ui->menuInstrument->actions())
        it->setChecked(false);
    menuItem->setChecked(true);
}

void MainWindow::playButtonPressed()
{
    if (isPlaying)
        stopMusic();
    else
        playMusic();
}

void MainWindow::nextPiece()
{
    int row = ui->songList->selectionModel()->selectedRows().first().row();
    if (row == ui->songList->rowCount() - 1)
        row = 0;
    else
        row++;
    ui->songList->selectRow(row);
    playMusic();
}
