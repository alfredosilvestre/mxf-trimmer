TEMPLATE = app

TARGET = "MXF Trimmer"

CONFIG += qt

QT += opengl

# Add the headers to the include path
INCLUDEPATH += powervs-core/include/licensing powervs-core/include/player powervs-core/include/previewer/ include include/trimmer

# Required for some C99 defines
DEFINES += __STDC_CONSTANT_MACROS

# Define features to use
DEFINES += GUI PORTAUDIO

# Add the headers to the include path
INCLUDEPATH += powervs-core/ffmpeg/include powervs-core/portaudio/include powervs-core/log4cxx/include

# Add the path to static libraries
LIBS += -Lpowervs-core/ffmpeg/lib/ -Lpowervs-core/portaudio/lib/

# Add the path to shared libraries
LIBS += -Lpowervs-core/ffmpeg/shared/ -Lpowervs-core/portaudio/shared/

# Log4CXX is a special cookie :)
CONFIG(debug, debug|release){
    LIBS += -Lpowervs-core/log4cxx/Debug/
} else {
    LIBS += -Lpowervs-core/log4cxx/lib/ -Lpowervs-core/log4cxx/shared/
}

# Set list of required libraries
LIBS += -lavcodec-57 -lavformat-57 -lavutil-55 -lswscale-4 -lswresample-2 -lavfilter-6 -lportaudio_x86 -llog4cxx

SOURCES += \
    powervs-core/src/player/audiosamplearray.cpp \
    powervs-core/src/player/avdecodedframe.cpp \
    powervs-core/src/player/ffdecoder.cpp \
    powervs-core/src/player/audiothread.cpp \
    powervs-core/src/player/videothread.cpp \
    powervs-core/src/player/player.cpp \
    powervs-core/src/previewer/previewerrgb.cpp \
    powervs-core/src/previewer/qvumeter.cpp \
    src/trimmer/trimmergui.cpp \
    src/trimmer/trimmer.cpp \
    src/mainwindow.cpp \
    src/main.cpp

HEADERS += \
    powervs-core/include/player/audiosamplearray.h \
    powervs-core/include/player/avdecodedframe.h \
    powervs-core/include/player/ffdecoder.h \
    powervs-core/include/player/audiothread.h \
    powervs-core/include/player/videothread.h \
    powervs-core/include/player/player.h \
    powervs-core/include/previewer/previewerrgb.h \
    powervs-core/include/previewer/qvumeter.h \
    include/trimmer/trimmergui.h \
    include/trimmer/trimmer.h \
    include/mainwindow.h

FORMS += \
    forms/mainwindow.ui \
    forms/trimmergui.ui

RESOURCES += \
    resources.qrc

RC_FILE += \
    proj.qrc
