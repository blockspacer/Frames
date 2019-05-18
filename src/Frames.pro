TARGET = Frames

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle qt

SOURCES += \
        game/states/introstate.cpp \
        game/states/menustate.cpp \
        main.cpp

# Include FEngine
include($$PWD/fengine/FEngine.pri))

# Configure build directories
release: DESTDIR = ../build/release
debug:   DESTDIR = ../build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

HEADERS += \
    game/states/introstate.h \
    game/states/menustate.h
