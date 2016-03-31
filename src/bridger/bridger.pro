TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../QtService
INCLUDEPATH += ../common

QT += network

#CONFIG -= qt
QTPLUGIN += qcncodecs

CharacterSet = 1
DEFINES += UNICODE
#DEFINES += __STDC_CONSTANT_MACROS
#DEFINES += __STDC_FORMAT_MACROS

QMAKE_LFLAGS += /MACHINE:X86

QMAKE_CXXFLAGS_DEBUG += /MDd
QMAKE_CXXFLAGS_RELEASE += /MD
QMAKE_CXXFLAGS_RELEASE += /Zi

#QMAKE_CFLAGS_RELEASE    = -O2 -Mt
#QMAKE_CFLAGS_DEBUG      = -Zi -Mtd

QMAKE_LFLAGS_RELEASE += /INCREMENTAL:NO
QMAKE_LFLAGS_RELEASE += /OPT:NOREF

#QMAKE_LFLAGS += /NODEFAULTLIB:LIBCMTD.lib
#QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:LIBCMT.lib

CONFIG(debug, debug|release) {
        QMAKE_LIBDIR += ../../src/QtService
}
else {
        QMAKE_LIBDIR += ../../src/QtService
}

CONFIG(debug, debug|release) {
	DESTDIR = ../../bin/debug
}
else {
	DESTDIR = ../../bin/release
}

CONFIG(debug, debug|release) {
        LIBS += QtSolutions_Service-headd.lib
}
else {
        LIBS += QtSolutions_Service-head.lib
}

#LIBS += avcodec.lib

# header files
HEADERS += ../common/xlogger.h
HEADERS += HttpBridger.h
HEADERS += HttpService.h
HEADERS += config.h
HEADERS += template.h
HEADERS += configer.h

# source files
SOURCES += main.cpp
SOURCES += ../common/xlogger.cpp
SOURCES += HttpBridger.cpp
SOURCES += HttpService.cpp
SOURCES += configer.cpp