SOURCES += \
    $$PWD/fengine.cpp

HEADERS += \
    $$PWD/fengine.h

################################################################################
# Libraries includes
INCLUDEPATH += $$PWD/libs
DEPENDPATH  += $$PWD/libs

################################################################################
# SFML
INCLUDEPATH += $$PWD/libs/SFML/include
DEPENDPATH  += $$PWD/libs/SFML/include

LIBS += -L$$PWD/libs/SFML/lib

CONFIG(release, debug|profile|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|profile|release): LIBS +=   -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d
CONFIG(profile, debug|profile|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d


################################################################################
# ImGui includes
DEFINES +=          IMGUI_DISABLE_INCLUDE_IMCONFIG_H
DEFINES +=          IMGUI_USER_CONFIG=\\\""imconfig_SFML.h"\\\"

LIBS += -lopengl32

SOURCES += \
    $$PWD/libs/ImGui/imgui.cpp \
    $$PWD/libs/ImGui/imgui_draw.cpp \
    $$PWD/libs/ImGui/imgui_widgets.cpp \
    $$PWD/libs/ImGui/imgui_SFML.cpp
    $$PWD/libs/ImGui/imgui_demo.cpp \

INCLUDEPATH += $$PWD/libs/ImGui
DEPENDPATH += $$PWD/libs/ImGui

###################################################################################################
# Easylogging++ includes

SOURCES += $$PWD/libs/easylogging++/easylogging++.cc
HEADERS += $$PWD/libs/easylogging++/easylogging++.h
