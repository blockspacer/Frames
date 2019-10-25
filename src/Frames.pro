TARGET = Frames

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle qt

SOURCES += \
        game/states/ingamestate.cpp \
        game/states/introstate.cpp \
        game/systems/tilemapeditor.cpp \
        game/systems/tilemapsimplesystem.cpp \
        game/systems/tilemapsystem.cpp \
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
    game/components/movement.h \
    game/components/position.h \
    game/components/sprite.h \
    game/components/tilemap.h \
    game/components/tileset.h \
    game/components/velocity.h \
    game/states/ingamestate.h \
    game/states/introstate.h \
    game/systems/tilemapeditor.hpp \
    game/systems/tilemapsimplesystem.hpp \
    game/systems/tilemapsystem.h
