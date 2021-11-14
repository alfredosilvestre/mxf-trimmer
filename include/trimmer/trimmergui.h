#ifndef TRIMMERGUI_H
#define TRIMMERGUI_H

#include "player.h"
#include "trimmer.h"

#include <QFrame>
#include <QTime>
#include <QTimer>

namespace Ui {
class TrimmerGUI;
}

class TrimmerGUI : public QFrame
{
    Q_OBJECT
    
    public:
        explicit TrimmerGUI(QWidget *parent = 0);
        ~TrimmerGUI();
        const bool isStopped() const;

    private:
        QString baseStylesheet;
        Ui::TrimmerGUI *ui;

        Player* player;
        Trimmer* trimmer;
        QString currentFilename;
        QString clipInTimecode;
        QString clipOutTimecode;
        QString aspectRatio;
        QString debugName;

        QTimer playTimer;

    private:
        void initSignals();
        void disableSignals();

    public slots:
        // Player Slots
        void changeAspectRatio();
        void changeAspectRatioReactivate();
        void loadFileSlot();
        void playPauseSlot();
        void nextFrameSlot();
        void previousFrameSlot();
        void forwardSlot();
        void reverseSlot();
        void seekSlot(const int value);

        // Trimmer Slots
        void calculateTrimDuration();
        void markInSlot();
        void markOutSlot();
        void markClipSlot();
        void clearMarksSlot();
        void gotoInSlot();
        void gotoOutSlot();
        void gotoStartSlot();
        void gotoEndSlot();
        void extractSlot();
        void trimProgressSlot(const int pts);

        void selectionChangedClipIn();
        void textEditedClipIn(const QString& text);
        void editingFinishedClipIn();
        void selectionChangedClipOut();
        void textEditedClipOut(const QString& text);
        void editingFinishedClipOut();

        // Helper slots
        void loadBars();
        const QString mSecsToTimecode(const int64_t msecs);
        const QTime timecodeToQTime(const QString& timecode);
        const QString timecodeMagic(const QString& input);
        void timeStep();
};

#endif // TRIMMERGUI_H
