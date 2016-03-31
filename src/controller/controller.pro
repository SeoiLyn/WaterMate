TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../QtService
INCLUDEPATH += ../common

RESOURCES     = controller.qrc

QT += network
QT += xml svg

#rc file
RC_FILE += controller.rc
FORMS   += controller.ui

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

QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

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
HEADERS += ControllTray.h
HEADERS += ../common/xlogger.h

# source files
SOURCES += ControllTray.cpp
SOURCES += main.cpp
SOURCES += ../common/xlogger.cpp