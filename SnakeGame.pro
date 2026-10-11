QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# 这个东西就是项目清单 SOURCES：告诉 qmake 哪些 .cpp 要分别编译。  HEADERS：登记 .h 头文件，方便 qmake 依赖分析、Qt Creator 项目树展示等
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    gamewindow.cpp\
    snake.cpp \
    food.cpp \
    gameboard.cpp

HEADERS += \
    gamewindow.h \
    snake.h \
    food.h \
    gameboard.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore
