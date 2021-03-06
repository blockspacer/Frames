#include "build.h"
#include <string>

#include "fengine/engine.h"
#include "game/states/introstate.h"

#include "game/states/ingamestate.h"

#include <easylogging++/easylogging++.h>

int main(int argc, char* argv[])
{
    frames::Engine game;

    game.init(std::string("Frames (build ") + BUILD_VERSION + ")", 1280, 720);
    game.setFramerateLimit(0);
    game.setAutoSnap(32, 18); // 16:9 aspect ratio

    game.changeState(IngameState::instance());

    game.start();

    while (game.running()) {
        game.update();
    }

    game.cleanup();

    return 0;
}
