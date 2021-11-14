#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTrimShortcuts();

    initShortcuts();

    qDebug() << "Interface loaded";
}

MainWindow::~MainWindow()
{
    if(this->height() != 611)
        disableTrimShortcuts();

    disableShortcuts();

    delete ui;
}

void MainWindow::initShortcuts()
{
    spaceShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    jShortcut = new QShortcut(QKeySequence(Qt::Key_J), this);
    kShortcut = new QShortcut(QKeySequence(Qt::Key_K), this);
    lShortcut = new QShortcut(QKeySequence(Qt::Key_L), this);
    leftShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    rightShortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    homeShortcut = new QShortcut(QKeySequence(Qt::Key_Home), this);
    endShortcut = new QShortcut(QKeySequence(Qt::Key_End), this);

    this->connect(spaceShortcut, SIGNAL(activated()), ui->trimmer, SLOT(playPauseSlot()));
    this->connect(jShortcut, SIGNAL(activated()), ui->trimmer, SLOT(previousFrameSlot()));
    this->connect(kShortcut, SIGNAL(activated()), ui->trimmer, SLOT(playPauseSlot()));
    this->connect(lShortcut, SIGNAL(activated()), ui->trimmer, SLOT(nextFrameSlot()));
    this->connect(leftShortcut, SIGNAL(activated()), ui->trimmer, SLOT(previousFrameSlot()));
    this->connect(rightShortcut, SIGNAL(activated()), ui->trimmer, SLOT(nextFrameSlot()));
    this->connect(homeShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoStartSlot()));
    this->connect(endShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoEndSlot()));
}

void MainWindow::initTrimShortcuts()
{
    iShortcut = new QShortcut(QKeySequence(Qt::Key_I), this);
    oShortcut = new QShortcut(QKeySequence(Qt::Key_O), this);
    shiftiShortcut = new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_I), this);
    shiftoShortcut = new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_O), this);
    qShortcut = new QShortcut(QKeySequence(Qt::Key_Q), this);
    wShortcut = new QShortcut(QKeySequence(Qt::Key_W), this);

    this->connect(iShortcut, SIGNAL(activated()), ui->trimmer, SLOT(markInSlot()));
    this->connect(oShortcut, SIGNAL(activated()), ui->trimmer, SLOT(markOutSlot()));
    this->connect(shiftiShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoInSlot()));
    this->connect(shiftoShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoOutSlot()));
    this->connect(qShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoInSlot()));
    this->connect(wShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoOutSlot()));
}

void MainWindow::disableShortcuts()
{
    this->disconnect(spaceShortcut, SIGNAL(activated()), ui->trimmer, SLOT(playPauseSlot()));
    this->disconnect(jShortcut, SIGNAL(activated()), ui->trimmer, SLOT(previousFrameSlot()));
    this->disconnect(kShortcut, SIGNAL(activated()), ui->trimmer, SLOT(playPauseSlot()));
    this->disconnect(lShortcut, SIGNAL(activated()), ui->trimmer, SLOT(nextFrameSlot()));
    this->disconnect(leftShortcut, SIGNAL(activated()), ui->trimmer, SLOT(previousFrameSlot()));
    this->disconnect(rightShortcut, SIGNAL(activated()), ui->trimmer, SLOT(nextFrameSlot()));
    this->disconnect(homeShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoStartSlot()));
    this->disconnect(endShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoEndSlot()));

    delete spaceShortcut;
    delete jShortcut;
    delete kShortcut;
    delete lShortcut;
    delete leftShortcut;
    delete rightShortcut;
    delete homeShortcut;
    delete endShortcut;
}

void MainWindow::disableTrimShortcuts()
{
    this->disconnect(iShortcut, SIGNAL(activated()), ui->trimmer, SLOT(markInSlot()));
    this->disconnect(oShortcut, SIGNAL(activated()), ui->trimmer, SLOT(markOutSlot()));
    this->disconnect(shiftiShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoInSlot()));
    this->disconnect(shiftoShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoOutSlot()));
    this->disconnect(qShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoInSlot()));
    this->disconnect(wShortcut, SIGNAL(activated()), ui->trimmer, SLOT(gotoOutSlot()));

    delete iShortcut;
    delete oShortcut;
    delete shiftiShortcut;
    delete shiftoShortcut;
    delete qShortcut;
    delete wShortcut;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if(ui->trimmer->isStopped())
        event->accept();
    else event->ignore();
}
