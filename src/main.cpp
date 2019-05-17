#include "build.h"
#include <string>

#include "fengine/engine.h"
//#include "game/gamestates/rstateintro.h"

#include <easylogging++/easylogging++.h>

int main(int argc, char* argv[])
{
    frames::Engine game;

    game.init(std::string("Frames (build ") + BUILD_VERSION + ")", 1280, 720);

    //game.changeState(RhymsStateIntro::instance());

    game.start();

    while (game.running()) {
        game.update();
    }

    game.cleanup();

    return 0;
}
