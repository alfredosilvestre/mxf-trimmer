#include "trimmergui.h"
#include "ui_trimmergui.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

TrimmerGUI::TrimmerGUI(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TrimmerGUI)
{
    ui->setupUi(this);
    player = new Player(this);
    trimmer = new Trimmer(this);
    baseStylesheet = this->styleSheet();
    currentFilename = "";
    clipInTimecode = "--:--:--:--";
    clipOutTimecode = "--:--:--:--";
    aspectRatio = "PAL";
    debugName = "{Trimmer} ";

    this->ui->labelSpeed->setVisible(false);
    this->ui->sliderSeek->setVisible(false);
    this->ui->progressBar->setVisible(false);

    this->initSignals();
    player->togglePreviewVideo(ui->preview);
    QTimer::singleShot(100, this, SLOT(loadBars()));
}

TrimmerGUI::~TrimmerGUI()
{
    this->disableSignals();

    delete trimmer;
    delete player;
    delete ui;
}

const bool TrimmerGUI::isStopped() const
{
    return !ui->progressBar->isVisible();
}

void TrimmerGUI::initSignals()
{
    this->connect(ui->pushButtonAspectRatio, SIGNAL(clicked()), this, SLOT(changeAspectRatio()));
    this->connect(ui->pushButtonOpenFile, SIGNAL(clicked()), this, SLOT(loadFileSlot()));
    this->connect(ui->pushButtonPlayPause, SIGNAL(clicked()), this, SLOT(playPauseSlot()));
    this->connect(ui->pushButtonNextFrame, SIGNAL(clicked()), this, SLOT(nextFrameSlot()));
    this->connect(ui->pushButtonPreviousFrame, SIGNAL(clicked()), this, SLOT(previousFrameSlot()));
    this->connect(ui->pushButtonForward, SIGNAL(clicked()), this, SLOT(forwardSlot()));
    this->connect(ui->pushButtonReverse, SIGNAL(clicked()), this, SLOT(reverseSlot()));
    this->connect(ui->sliderSeek, SIGNAL(valueChanged(int)), this, SLOT(seekSlot(int)));

    this->connect(ui->pushButtonMarkIn, SIGNAL(clicked()), this, SLOT(markInSlot()));
    this->connect(ui->pushButtonMarkOut, SIGNAL(clicked()), this, SLOT(markOutSlot()));
    this->connect(ui->pushButtonMarkClip, SIGNAL(clicked()), this, SLOT(markClipSlot()));
    this->connect(ui->pushButtonClearMarks, SIGNAL(clicked()), this, SLOT(clearMarksSlot()));
    this->connect(ui->pushButtonGoToIn, SIGNAL(clicked()), this, SLOT(gotoInSlot()));
    this->connect(ui->pushButtonGoToOut, SIGNAL(clicked()), this, SLOT(gotoOutSlot()));
    this->connect(ui->pushButtonExtract, SIGNAL(clicked()), this, SLOT(extractSlot()));

    this->connect(ui->lineEditClipIn, SIGNAL(selectionChanged()), this, SLOT(selectionChangedClipIn()));
    this->connect(ui->lineEditClipIn, SIGNAL(textEdited(QString)), this, SLOT(textEditedClipIn(QString)));
    this->connect(ui->lineEditClipIn, SIGNAL(editingFinished()), this, SLOT(editingFinishedClipIn()));
    this->connect(ui->lineEditClipOut, SIGNAL(selectionChanged()), this, SLOT(selectionChangedClipOut()));
    this->connect(ui->lineEditClipOut, SIGNAL(textEdited(QString)), this, SLOT(textEditedClipOut(QString)));
    this->connect(ui->lineEditClipOut, SIGNAL(editingFinished()), this, SLOT(editingFinishedClipOut()));

    this->connect(ui->preview, SIGNAL(setValues(QByteArray)), ui->audioMeter, SLOT(setValues(QByteArray)));
}

void TrimmerGUI::disableSignals()
{
    this->disconnect(ui->pushButtonAspectRatio, SIGNAL(clicked()), this, SLOT(changeAspectRatio()));
    this->disconnect(ui->pushButtonOpenFile, SIGNAL(clicked()), this, SLOT(loadFileSlot()));
    this->disconnect(ui->pushButtonPlayPause, SIGNAL(clicked()), this, SLOT(playPauseSlot()));
    this->disconnect(ui->pushButtonNextFrame, SIGNAL(clicked()), this, SLOT(nextFrameSlot()));
    this->disconnect(ui->pushButtonPreviousFrame, SIGNAL(clicked()), this, SLOT(previousFrameSlot()));
    this->disconnect(ui->pushButtonForward, SIGNAL(clicked()), this, SLOT(forwardSlot()));
    this->disconnect(ui->pushButtonReverse, SIGNAL(clicked()), this, SLOT(reverseSlot()));
    this->disconnect(ui->sliderSeek, SIGNAL(valueChanged(int)), this, SLOT(seekSlot(int)));

    this->disconnect(ui->pushButtonMarkIn, SIGNAL(clicked()), this, SLOT(markInSlot()));
    this->disconnect(ui->pushButtonMarkOut, SIGNAL(clicked()), this, SLOT(markOutSlot()));
    this->disconnect(ui->pushButtonMarkClip, SIGNAL(clicked()), this, SLOT(markClipSlot()));
    this->disconnect(ui->pushButtonClearMarks, SIGNAL(clicked()), this, SLOT(clearMarksSlot()));
    this->disconnect(ui->pushButtonGoToIn, SIGNAL(clicked()), this, SLOT(gotoInSlot()));
    this->disconnect(ui->pushButtonGoToOut, SIGNAL(clicked()), this, SLOT(gotoOutSlot()));

    this->disconnect(ui->lineEditClipIn, SIGNAL(selectionChanged()), this, SLOT(selectionChangedClipIn()));
    this->disconnect(ui->lineEditClipIn, SIGNAL(textEdited(QString)), this, SLOT(textEditedClipIn(QString)));
    this->disconnect(ui->lineEditClipIn, SIGNAL(editingFinished()), this, SLOT(editingFinishedClipIn()));
    this->disconnect(ui->lineEditClipOut, SIGNAL(selectionChanged()), this, SLOT(selectionChangedClipOut()));
    this->disconnect(ui->lineEditClipOut, SIGNAL(textEdited(QString)), this, SLOT(textEditedClipOut(QString)));
    this->disconnect(ui->lineEditClipOut, SIGNAL(editingFinished()), this, SLOT(editingFinishedClipOut()));

    this->disconnect(ui->preview, SIGNAL(setValues(QByteArray)), ui->audioMeter, SLOT(setValues(QByteArray)));
}

// Player Slots

void TrimmerGUI::changeAspectRatio()
{
    if(!player->isPlaying())
    {
        ui->pushButtonAspectRatio->blockSignals(true);

        if(aspectRatio == "PAL")
        {
            aspectRatio = "PAL 16:9";
            ui->pushButtonAspectRatio->setStyleSheet("QPushButton { border: none; background: url(:/images/buttons/16x9.png); } QPushButton:pressed { background: url(:/images/buttons/16x9_pressed.png); }");
        }
        else
        {
            aspectRatio = "PAL";
            ui->pushButtonAspectRatio->setStyleSheet("QPushButton { border: none; background: url(:/images/buttons/4x3.png); } QPushButton:pressed { background: url(:/images/buttons/4x3_pressed.png); }");
        }

        player->changeFormat(aspectRatio);
        if(player->isLoaded())
            seekSlot(ui->sliderSeek->value());

        QTimer::singleShot(QApplication::doubleClickInterval(), this, SLOT(changeAspectRatioReactivate()));
    }
}

void TrimmerGUI::changeAspectRatioReactivate()
{
    ui->pushButtonAspectRatio->blockSignals(false);
}

void TrimmerGUI::loadFileSlot()
{
    if(!player->isPlaying())
    {
        QFileDialog fileDialog;
        fileDialog.setWindowTitle("Add Video File...");
        fileDialog.setFileMode(QFileDialog::ExistingFile);
        fileDialog.setFilter(tr("MXF Files (*.mxf);;All Files (*.*)"));

        QStringList fileNames;
        if(fileDialog.exec())
            fileNames = fileDialog.selectedFiles();

        QString path = "";
        if(!fileNames.empty())
            path = fileNames.first();

        if(path != "")
        {
            if(player->isLoaded())
            {
                player->dropMedia(true);
                if(playTimer.isActive())
                {
                    this->disconnect(&playTimer, SIGNAL(timeout()), this, SLOT(timeStep()));
                    playTimer.stop();
                }

                ui->labelClipName->setText("No Clip Loaded");
                currentFilename = "";
                ui->labelClipDuration->setText("00:00:00:00");
                ui->labelClipTimecode->setText("00:00:00:00");
                clearMarksSlot();

                this->ui->labelSpeed->setVisible(false);
                this->ui->labelSpeed->setText("1x");
                this->ui->sliderSeek->setVisible(false);
                this->ui->sliderSeek->setValue(0);

                QTimer::singleShot(100, this, SLOT(loadBars()));
            }

            QFileInfo fileInfo(path);
            QString name = fileInfo.baseName();

            int64_t duration_ms = player->loadMedia(path);
            if(duration_ms != -1)
            {
                currentFilename = path;
                this->ui->sliderSeek->setVisible(true);
                this->ui->sliderSeek->setMaximum(duration_ms);
                ui->labelClipName->setText(name);

                qDebug() << debugName + "Loaded clip: " + name;

                if(!playTimer.isActive())
                {
                    this->connect(&playTimer, SIGNAL(timeout()), this, SLOT(timeStep()));
                    timeStep();
                }
            }
            else
            {
                QString msg("Error loading file \"" + name + "\"");
                QMessageBox::critical((QWidget*)this->parent(), "Error", msg);
                qCritical() << debugName + msg;
            }
        }
    }
}

void TrimmerGUI::playPauseSlot()
{
    if(player->isLoaded())
    {
        if(player->isPlaying())
        {
            qDebug() << debugName + "Pause pressed";

            player->pause(true);
            ui->pushButtonPlayPause->setStyleSheet("QPushButton { border: none; background: url(:/images/buttons/play.png); } QPushButton:pressed { background: url(:/images/buttons/play_pressed.png); }");
            this->ui->labelSpeed->setVisible(false);
            this->ui->labelSpeed->setText("1x");

            if(playTimer.isActive())
                playTimer.stop();
        }
        else
        {
            qDebug() << debugName + "Play pressed";
            player->take();
            ui->pushButtonPlayPause->setStyleSheet("QPushButton { border: none; background: url(:/images/buttons/pause.png); } QPushButton:pressed { background: url(:/images/buttons/pause_pressed.png); }");
            this->ui->labelSpeed->setVisible(true);

            if(!playTimer.isActive())
                playTimer.start(10);
        }
    }
}

void TrimmerGUI::nextFrameSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        qDebug() << debugName + "Next frame pressed";
        player->nextFrame();
        timeStep();
    }
}

void TrimmerGUI::previousFrameSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        qDebug() << debugName + "Previous frame pressed";
        player->previousFrame();
        timeStep();
    }
}

void TrimmerGUI::forwardSlot()
{
    if(player->isLoaded() && player->isPlaying())
    {
        qDebug() << debugName + "Forward pressed";
        this->ui->labelSpeed->setText(QString::number(player->forward()) + "x");
    }
}

void TrimmerGUI::reverseSlot()
{
    if(player->isLoaded() && player->isPlaying())
    {
        qDebug() << debugName + "Reverse pressed";
        this->ui->labelSpeed->setText(QString::number(player->reverse()) + "x");
    }
}

void TrimmerGUI::seekSlot(const int value)
{
    if(player->isLoaded() && !player->isPlaying())
    {
        qDebug() << debugName + "Seek value changed";
        player->seek(value);

        ui->sliderSeek->blockSignals(true);
        timeStep();
        ui->sliderSeek->blockSignals(false);
    }
}

// Trimmer Slots

void TrimmerGUI::calculateTrimDuration()
{
    if(clipInTimecode != "--:--:--:--" && clipOutTimecode != "--:--:--:--")
    {
        QTime durationIn = timecodeToQTime(clipInTimecode);
        QTime durationOut = timecodeToQTime(clipOutTimecode);

        int msecs = durationIn.msecsTo(durationOut);

        if((durationIn > durationOut) || (durationOut < durationIn))
        {
            clipInTimecode = clipOutTimecode = ui->labelClipTimecode->text();
            ui->lineEditClipIn->setText(clipInTimecode);
            ui->lineEditClipOut->setText(clipOutTimecode);
            msecs = 0;
        }

        ui->labelTrimDuration->setText(mSecsToTimecode(msecs));
    }
}

void TrimmerGUI::markInSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        clipInTimecode = ui->labelClipTimecode->text();
        ui->lineEditClipIn->setText(clipInTimecode);
        calculateTrimDuration();

        qDebug() << debugName + "Mark In pressed: " + clipInTimecode;
    }
}

void TrimmerGUI::markOutSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        clipOutTimecode = ui->labelClipTimecode->text();
        ui->lineEditClipOut->setText(clipOutTimecode);
        calculateTrimDuration();

        qDebug() << debugName + "Mark Out pressed: " + clipOutTimecode;
    }
}

void TrimmerGUI::markClipSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        int64_t startMiliseconds = player->getStartMillisecond();
        clipInTimecode = mSecsToTimecode(startMiliseconds);
        ui->lineEditClipIn->setText(clipInTimecode);
        clipOutTimecode = mSecsToTimecode(startMiliseconds + ui->sliderSeek->maximum());
        ui->lineEditClipOut->setText(clipOutTimecode);
        calculateTrimDuration();

        qDebug() << debugName + "Mark Clip pressed";
    }
}

void TrimmerGUI::clearMarksSlot()
{
    clipInTimecode = "--:--:--:--";
    ui->lineEditClipIn->setText(clipInTimecode);
    clipOutTimecode = "--:--:--:--";
    ui->lineEditClipOut->setText(clipOutTimecode);
    ui->labelTrimDuration->setText("00:00:00:00");

    qDebug() << debugName + "Clear Marks pressed";
}

void TrimmerGUI::gotoInSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        if(clipInTimecode != "--:--:--:--")
        {
            int pos = QTime().msecsTo(timecodeToQTime(clipInTimecode)) - player->getStartMillisecond();
            seekSlot(pos);

            qDebug() << debugName + "Goto In pressed: " + clipInTimecode;
        }
    }
}

void TrimmerGUI::gotoOutSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        if(clipOutTimecode != "--:--:--:--")
        {
            int pos = QTime().msecsTo(timecodeToQTime(clipOutTimecode)) - player->getStartMillisecond();
            seekSlot(pos);

            qDebug() << debugName + "Goto Out pressed: " + clipInTimecode;
        }
    }
}

void TrimmerGUI::gotoStartSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        seekSlot(0);
        qDebug() << debugName + "Goto Start pressed";
    }
}

void TrimmerGUI::gotoEndSlot()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        seekSlot(ui->sliderSeek->maximum());
        qDebug() << debugName + "Goto End pressed";
    }
}

void TrimmerGUI::extractSlot()
{
    if(player->isLoaded() && !player->isPlaying() && ui->labelTrimDuration->text() != "00:00:00:00" != 0 && !trimmer->isRunning())
    {
        QString path = QFileDialog::getSaveFileName((QWidget*)this->parent(), tr("Save file..."), "", tr("MXF File (*.mxf);;All Files (*.*)"));
        if(path != "")
        {
            int posIn = QTime().msecsTo(timecodeToQTime(clipInTimecode)) - player->getStartMillisecond();
            int posOut = QTime().msecsTo(timecodeToQTime(clipOutTimecode)) - player->getStartMillisecond();

            ui->progressBar->setMinimum(0);
            ui->progressBar->setMaximum(posOut);
            ui->progressBar->setVisible(true);
            ui->progressBar->setValue(0);
            this->connect(trimmer, SIGNAL(trimProgress(int)), this, SLOT(trimProgressSlot(int)));

            if(trimmer->initTrimmer(currentFilename, posIn, posOut, path, clipInTimecode.toStdString().c_str()))
            {
                trimmer->start();
                qDebug() << debugName + "Extract pressed: " + clipInTimecode + " - " + clipOutTimecode;
            }
        }
    }
}

void TrimmerGUI::trimProgressSlot(const int pts)
{
    this->ui->progressBar->setValue(pts);
    if(pts >= ui->progressBar->maximum())
    {
        ui->progressBar->setVisible(false);
        this->disconnect(trimmer, SIGNAL(trimProgress(int)), this, SLOT(trimProgressSlot(int)));

        QMessageBox::information((QWidget*)this->parent(), "Success", "Trim operation completed successfully");
    }
}

void TrimmerGUI::selectionChangedClipIn()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        ui->lineEditClipIn->setReadOnly(false);
        ui->lineEditClipIn->setText("");
    }
}

void TrimmerGUI::textEditedClipIn(const QString& text)
{
    QRegExp rx("(\\d|\\.|:)*");
    if(!rx.exactMatch(text))
    {
        QString str = text;
        str.chop(1);
        ui->lineEditClipIn->setText(str);
    }
}

void TrimmerGUI::editingFinishedClipIn()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        QString str = timecodeMagic(ui->lineEditClipIn->text());
        if(str == "")
            ui->lineEditClipIn->setText(clipInTimecode);
        else
        {
            int64_t startMiliseconds = player->getStartMillisecond();
            int timecode = QTime().msecsTo(timecodeToQTime(str));

            if(startMiliseconds <= timecode && timecode <= startMiliseconds + ui->sliderSeek->maximum())
            {
                clipInTimecode = str;
                ui->lineEditClipIn->setText(clipInTimecode);
                calculateTrimDuration();
            }
            else ui->lineEditClipIn->setText(clipInTimecode);
        }

        ui->lineEditClipIn->setReadOnly(true);
    }
}

void TrimmerGUI::selectionChangedClipOut()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        ui->lineEditClipOut->setReadOnly(false);
        ui->lineEditClipOut->setText("");
    }
}

void TrimmerGUI::textEditedClipOut(const QString& text)
{
    QRegExp rx("(\\d|\\.|:)*");
    if(!rx.exactMatch(text))
    {
        QString str = text;
        str.chop(1);
        ui->lineEditClipIn->setText(str);
    }
}

void TrimmerGUI::editingFinishedClipOut()
{
    if(player->isLoaded() && !player->isPlaying())
    {
        QString str = timecodeMagic(ui->lineEditClipOut->text());
        if(str == "")
            ui->lineEditClipOut->setText(clipOutTimecode);
        else
        {
            int64_t startMiliseconds = player->getStartMillisecond();
            int timecode = QTime().msecsTo(timecodeToQTime(str));

            if(startMiliseconds <= timecode && timecode <= startMiliseconds + ui->sliderSeek->maximum())
            {
                clipOutTimecode = str;
                ui->lineEditClipOut->setText(clipOutTimecode);
                calculateTrimDuration();
            }
            else ui->lineEditClipOut->setText(clipOutTimecode);
        }

        ui->lineEditClipOut->setReadOnly(true);
    }
}

// Helper slots

void TrimmerGUI::loadBars()
{
    if(!player->isLoaded())
        ui->preview->loadBars();
}

const QString TrimmerGUI::mSecsToTimecode(const int64_t msecs)
{
    QTime newDuration = QTime().addMSecs(msecs);
    QStringList splitDuration = newDuration.toString("hh:mm:ss.zzz").split(".");
    int framesDuration = splitDuration.at(1).toInt() * 25 / 1000;
    return splitDuration[0] + ":" + QString::number(framesDuration).rightJustified(2, '0');
}

const QTime TrimmerGUI::timecodeToQTime(const QString& timecode)
{
    QStringList splitDuration = timecode.split(":");
    int framesDuration = splitDuration.at(3).toInt() * 1000 / 25;
    return QTime::fromString(splitDuration[0] + ":" + splitDuration[1] + ":" + splitDuration[2] + "." + QString::number(framesDuration).rightJustified(3, '0'), "hh:mm:ss.zzz");
}

const QString TrimmerGUI::timecodeMagic(const QString& input)
{
    QString str;

    QRegExp rx("^(\\d)*([.|:](\\d)*){1,3}$");
    if(rx.exactMatch(input))
    {
        QStringList split = input.split(QRegExp("(\\.|:)"));
        int len = split.length();
        int transport = 0;
        int position = 4;

        for(int i=len-1; i>=0; i--)
        {
            QString s = split[i];
            if(s == "")
                s = "00";

            int num = s.toInt() + transport;
            transport = 0;
            position--;

            if(position == 0) // Hours
            {
                if(num >= 24)
                {
                    transport += floorl(num/24);
                    num %= 24;
                }
            }
            else if(position == 1 || position == 2) // Minutes or seconds
            {
                if(num >= 60)
                {
                    transport += floorl(num/60);
                    num %= 60;
                }
            }
            else if(position == 3) // Frames
            {
                if(num >= 25)
                {
                    transport += floorl(num/25);
                    num %= 25;
                }
            }

            str = ':' + QString::number(num).rightJustified(2, '0') + str;
        }

        if(len < 4)
        {
            for(int i=len; i<4; i++)
                str = ":00" + str;
        }
        str = str.right(str.length()-1);
    }

    return str;
}

void TrimmerGUI::timeStep()
{
    if(!player->isEOF())
    {
        int64_t msecs = player->getCurrentPlayTime();
        this->ui->sliderSeek->setValue(msecs);

        ui->labelClipDuration->setText(mSecsToTimecode(msecs));
        ui->labelClipTimecode->setText(mSecsToTimecode(player->getCurrentTimeCode()));
    }
    else playPauseSlot();
}
