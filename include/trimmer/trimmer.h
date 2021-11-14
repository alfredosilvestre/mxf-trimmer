#ifndef TRIMMER_H
#define TRIMMER_H

#include <QThread>

extern "C"
{
    #include <libavformat/avformat.h>
}

class Trimmer : public QThread
{
    Q_OBJECT

    public:
        explicit Trimmer(QObject *parent = 0);
        ~Trimmer();
        bool initTrimmer(const QString& inputFile, int inPTS, int outPTS, const QString& outputFile, const char* timecode);

    private:
        void run();
        void cleanup();

    private:
        AVFormatContext* inputContext;
        AVStream* inputVideoStream;
        AVFormatContext* outputContext;

        int inPTS;
        int outPTS;

    signals:
        void trimProgress(const int pts);
};

#endif // TRIMMER_H
