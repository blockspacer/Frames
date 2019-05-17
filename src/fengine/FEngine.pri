SOURCES += \
    $$PWD/engine.cpp \
    $$PWD/scheduler.cpp \
    $$PWD/utils/timing.cpp

HEADERS += \
    $$PWD/engine.h \
    $$PWD/igamestate.h \
    $$PWD/scheduler.h \
    $$PWD/ui/graph.h \
    $$PWD/ui/graph.tpp \
    $$PWD/ui/timegraph.h \
    $$PWD/utils/string.h \
    $$PWD/utils/timing.h

DEFINES += NOMINMAX

################################################################################
# Libraries includes
INCLUDEPATH += $$PWD/libs
DEPENDPATH  += $$PWD/libs

################################################################################
# SFML
INCLUDEPATH += $$PWD/libs/SFML/include
DEPENDPATH  += $$PWD/libs/SFML/include

LIBS += -L$$PWD/libs/SFML/lib

CONFIG(release, debug|profile|release): LIBS += -lsfml-audio -lsfml-graphics \
                                                -lsfml-main -lsfml-network \
                                                -lsfml-window -lsfml-system
CONFIG(debug, debug|profile|release): LIBS +=   -lsfml-audio-d \
                                                -lsfml-graphics-d \
                                                -lsfml-main-d -lsfml-network-d \
                                                -lsfml-window-d -lsfml-system-d
CONFIG(profile, debug|profile|release): LIBS += -lsfml-audio-d \
                                                -lsfml-graphics-d \
                                                -lsfml-main-d -lsfml-network-d \
                                                -lsfml-window-d -lsfml-system-d

################################################################################
# ImGui includes
DEFINES +=          IMGUI_DISABLE_INCLUDE_IMCONFIG_H
DEFINES +=          IMGUI_USER_CONFIG=\\\""imgui-SFML\imconfig_SFML.h"\\\"
LIBS +=             -lopengl32

SOURCES += \
    $$PWD/libs/ImGui/imgui.cpp \
    $$PWD/libs/ImGui/imgui_draw.cpp \
    $$PWD/libs/ImGui/imgui_widgets.cpp \
    $$PWD/libs/ImGui/imgui_demo.cpp \
    $$PWD/libs/ImGui-SFML/imgui_SFML.cpp

################################################################################
# Easylogging++ includes

SOURCES += $$PWD/libs/easylogging++/easylogging++.cc
HEADERS += $$PWD/libs/easylogging++/easylogging++.h

################################################################################
# Entt
INCLUDEPATH += $$PWD/libs/entt/single_include/
DEPENDPATH  += $$PWD/libs/entt/single_include/

################################################################################
# Tracy profiler
SOURCES += $$PWD/libs/tracy/TracyClient.cpp
INCLUDEPATH += $$PWD/libs/tracy/
DEPENDPATH  += $$PWD/libs/tracy/
LIBS += -ladvapi32

CONFIG(debug, debug|profile|release): DEFINES += TRACY_ENABLE
CONFIG(profile, debug|profile|release): DEFINES += TRACY_ENABLE
