TARGET = Frames

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle qt

SOURCES += \
        main.cpp

# Include FEngine
include($$PWD/fengine/FEngine.pri))

# Save the git commit hash to build.h
system(VersionManager)

# Configure build directories
release: DESTDIR = ../build/release
debug:   DESTDIR = ../build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

# Force Qt Creator to run qmake at every build
qmakeforce.target = dummy
qmakeforce.commands = $$QMAKE_DEL_FILE Makefile
qmakeforce.depends = FORCE
PRE_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce
