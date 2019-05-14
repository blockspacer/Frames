#include <string>

#include "build.h"
#include "fengine/fengine.h"
//#include "game/gamestates/rstateintro.h"

int main(int argc, char* argv[])
{
    FEngine game;

    game.init("Frames b" + std::to_string(BUILD), 1280, 720);

    //game.changeState(RhymsStateIntro::instance());

    game.start();

    while (game.running()) {
        game.update();
    }

    game.cleanup();

    return 0;
}
