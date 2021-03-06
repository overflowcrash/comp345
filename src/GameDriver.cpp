//! @file
//! @author Vsevolod Ivanov

#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Player.h"

using namespace std;

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::info);

    if (argc > 1)
    {
        string loglevel = argv[1];

        if (loglevel == "debug")
            spdlog::set_level(spdlog::level::debug);
    }

    Game *game = new Game(Game::UI::SHELL);
    PhaseObserver *po1 = new PhaseObserver(1, game);
    game->play();

    delete po1;
    delete game;
    return 0;
}
