//! @file
//! @author Vsevolod Ivanov

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <spdlog/logger.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"
#include "MapReader.h"
#include "Deck.h"

using namespace std;

class Game
{
    public:
        enum UI {
            SHELL, SDL2
        };
        enum State {
            RUN, PAUSE, HALT
        };

        Game(UI ui);
        ~Game();

        void play();
        bool addPlayer(Player *player);

    private:
        Game::UI ui;
        Game::State state;

        Map* map;
        Deck* deck;
        vector<Player*> players;

        Window* window;
        SDL_Event* windowEvent;

        const string logname;
        stringstream logbuf;
        std::shared_ptr<spdlog::logger> logger;

        bool sdl2_init();
        void sdl2_poll();

        bool shell_init();
        bool shell_start();
};
