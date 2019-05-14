TARGET = Frames

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle qt

SOURCES += \
        main.cpp

# Include FEngine
include($$PWD/fengine/FEngine.pri))

# Incremental builds

build_nr.target = $$PWD/build.h
build_nr.commands = build_inc.bat
build_nr.depends = FORCE
#PRE_TARGETDEPS += $$PWD/build.h
QMAKE_EXTRA_TARGETS += build_nr

# Configure build directories
Release:DESTDIR = ../build/release
Release:OBJECTS_DIR = ../build/release/.obj
Release:MOC_DIR = ../build/release/.moc
Release:RCC_DIR = ../build/release/.rcc
Release:UI_DIR = ../build/release/.ui

Debug:DESTDIR = ../build/debug
Debug:OBJECTS_DIR = ../build/debug/.obj
Debug:MOC_DIR = ../build/debug/.moc
Debug:RCC_DIR = ../build/debug/.rcc
Debug:UI_DIR = ../build/debug/.ui
