#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSplashScreen>
#include <QTextStream>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
}

#ifdef PORTAUDIO
#include <portaudio.h>
#endif

QTextStream stream;
MainWindow* form;

void LogHandler(QtMsgType type, const char* msg)
{
    QString message(msg);
    if(message.contains("QGLShader::link: \"No errors"))
        return;

    message = message.replace("\"", "").trimmed();
    QString date = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss.zzz]");

    switch(type)
    {
        case QtDebugMsg:
            stream << date << " Info: " << message << "\n";
            break;
        case QtCriticalMsg:
            stream << date << " Critical: " << message << "\n";
            break;
        case QtWarningMsg:
            stream << date << " Warning: " << message << "\n";
            break;
        case QtFatalMsg:
            stream << date <<  " Fatal: " << message << "\n";
            break;
        default:
            break;
    }

    stream.flush();
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QSplashScreen splash(QPixmap(":/images/splash.jpg"));
    splash.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    // Initialize libraries
    av_register_all();
    avfilter_register_all();
#ifdef PORTAUDIO
    Pa_Initialize();
#endif

    QString path = QApplication::applicationDirPath() + "/logs/";
    QDir().mkpath(path);
    path += "mxftrimmer " + QDateTime::currentDateTime().toString("yyyy-MM-dd HHmmss") + ".log";

    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    stream.setDevice(&file);
    qInstallMsgHandler(LogHandler);

    qDebug() << "Application started";

    form = new MainWindow();
    splash.finish(form);
    form->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    form->show();

    int ret_val = app.exec();
    file.close();

    // Deinitialize libraries
#ifdef PORTAUDIO
    Pa_Terminate();
#endif

    qDebug() << "Application finished";

    return ret_val;
}
