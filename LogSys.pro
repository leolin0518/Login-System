#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T12:41:38
#
#-------------------------------------------------

QT       += core sql gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogSys
TEMPLATE = app


SOURCES += main.cpp\
        loginsystem.cpp \
    qdb.cpp

HEADERS  += loginsystem.h \
    qdb.h

FORMS    += loginsystem.ui

DISTFILES += \
    db.s3db.sql

RESOURCES += \
    elements.qrc
