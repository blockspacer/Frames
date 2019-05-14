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
