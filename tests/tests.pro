QT += testlib core
CONFIG += testcase c++17 console
CONFIG -= app_bundle

SOURCES += \
    tst_gamecore.cpp \
    ../snake.cpp

HEADERS += \
    ../snake.h
