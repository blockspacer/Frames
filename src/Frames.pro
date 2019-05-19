TARGET = Frames

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle qt

SOURCES += \
        game/states/gamestate.cpp \
        game/states/introstate.cpp \
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
    game/states/gamestate.h \
    game/states/introstate.h
