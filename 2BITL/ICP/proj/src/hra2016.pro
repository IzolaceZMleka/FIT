QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hra2016
TEMPLATE = app
CONFIG += c++11


SOURCES += \
		main.cpp \
        core.cpp \
        messenger.cpp \
        othello.cpp \
        builder.cpp \
        board_history.cpp \
        scout.cpp \
        mainwindow.cpp \
        newgamemenu.cpp \
        loadgamemenu.cpp \
        form.cpp\
        artificial_intelligence.cpp \
        grey_manager.cpp \
        savegamemenu.cpp \
    io.cpp \ 
	#cli_main.cpp \
	cli.cpp

HEADERS += \
        core.h \
        io.h \
        othello.h \
        gamestate.h \
        messenger.h \
        othello_types.h \
        builder.h \
        artificial_intelligence.h \
        mainwindow.h \
        newgamemenu.h \
        loadgamemenu.h \
        form.h \
        savegamemenu.h \ 
    cli.h

FORMS    += mainwindow.ui \
        newgamemenu.ui \
        loadgamemenu.ui \
        form.ui \
        savegamemenu.ui

RESOURCES += \
        svg/ikony.qrc

DISTFILES += \
    README.txt

LIBS += \
	-lboost_system\
	-lboost_serialization

