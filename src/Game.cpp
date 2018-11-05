//! @file
//! @author Vsevolod Ivanov

#include "Game.h"

using namespace std;

Game::Game(Game::UI ui): ui(ui)
{
    switch(ui)
    {
        case Game::UI::SDL2:
            this->sdl2_init();
            break;
        case Game::UI::SHELL:
        default:
            break;
    }
}

Game::~Game()
{
    for (Player* player: this->players)
        delete player;
    this->players.clear();
    delete this->windowEvent;
    delete this->window;
    if (this->map != NULL)
        delete map;
    if (this->deck != NULL)
        delete deck;
}

bool Game::sdl2_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    this->window = new Window(
        "Risky Business", flags, 800, 500, 100, 100
    );
    if (this->window->getWindow() == NULL)
    {
        cerr << "Failed to create SDL window" << endl;
        return false;
    }
    this->windowEvent = new SDL_Event();
    return true;
}

void Game::sdl2_poll()
{
    if (this->windowEvent->type == SDL_QUIT)
        this->state = Game::State::HALT;
    SDL_PollEvent(this->windowEvent);
    SDL_RenderClear(this->window->getRenderer());
    SDL_RenderPresent(this->window->getRenderer());
}

/* Part 1: Game start
 * You must deliver a driver that demonstrates:
 * 1. different valid maps can be loaded
 *    their validity is verified (i.e. it is a connected graph, etc)
 *    invalid maps are gracefully rejected
 * 2. the right number of players is created
 *    a deck with the right number of cards is created
 */
bool Game::shell_init()
{
    cout << "Welcome to Risk!" << endl;
    int nbOfPlayers;

    // 1. select a map from a list of map files as stored in a directory
    string mapName;
    MapReader mapReader;
    stringstream mapFpath; mapFpath << "./assets/maps/";
    cout << "Enter the map name (./assets/maps/): "; cin >> mapName;
    mapFpath << mapName << ".map";
    if (mapReader.read(mapFpath.str()) == false)
        return false;

    // 3. The code should then use the map loader to load the selected map
    this->map = new Map(mapReader.getNbOfNodes());
    mapReader.load(*this->map);

    // 6. create a deck of cards
    this->deck = new Deck();

    // 2. select the number of players in the game (2-6 players)
    cout << "Enter the number of players: "; cin >> nbOfPlayers;
    cout << "Creating " << nbOfPlayers << " players..." << endl;
    // 4. create all the players
    for (int i = 1; i <= nbOfPlayers; ++i)
    {
        Player *player = new Player("Player" + to_string(i));
        // 5. assign dice rolling facilities to the players
        player->rollDices(1);
        // 7. assign an empty hand of cards to each player
        cout << player->getName() << " has " <<
                player->getCards().size() << " cards" << endl;
        this->addPlayer(player);
    }
    return true;
}

/* Part 2: Game play: startup phase
 */
bool Game::shell_start()
{
    // 1. The order of play of the players in the game is determined randomly
    int playerTurn;
    vector<Player*> playersRandomOrder;
    while (this->players.empty() == false)
    {
        playerTurn = rand() % this->players.size();
        playersRandomOrder.push_back(this->players.at(playerTurn));
        this->players.erase(this->players.begin() + playerTurn);
    }
    for (Player *player: playersRandomOrder)
        this->players.push_back(player);

    // 2. All countries in the map are randomly assigned to players one by one in a round-robin fashion
    Player *player;
    playerTurn = 0;
    Country *country;
    int countryTurn = 0;
    vector<Country*> countriesToAssign = this->map->getCountries();

    while (countriesToAssign.empty() == false)
    {
        countryTurn = rand() % countriesToAssign.size();
        country = countriesToAssign.at(countryTurn);
        player = this->players.at(playerTurn);
        // 2.1 All countries in the map have been assigned to one and only one player
        player->addCountry(country);
        cout << player->getName() << " got " << country->getName() << endl;
        countriesToAssign.erase(countriesToAssign.begin() + countryTurn);
        playerTurn = (playerTurn + 1) % this->players.size();
    }

    // 3. Players are given a number of armies (A), to be placed one by one in a round-robin fashion on some of the countries that they own, where A is:
    int nbOfArmies;
    switch (this->players.size())
    {
        case 2:
            nbOfArmies = 40;
            break;
        case 3:
            nbOfArmies = 35;
            break;
        case 4:
            nbOfArmies = 30;
            break;
        case 5:
            nbOfArmies = 25;
            break;
        case 6:
            nbOfArmies = 20;
            break;
        default:
            cerr << "Error: Unsupported number of players..." << endl;
            return false;
    }
    cout << "Each player will have " << nbOfArmies << endl;

    // 3.1 all players have eventually placed the right number of armies on their own countries after army placement is over.


    return true;
}

/* Part 3: Game play: main game loop
 */
void Game::play()
{
    if (this->shell_init() == false)
        return;
    if (this->shell_start() == false)
        return;
    this->state = Game::State::RUN;

    while (Game::state == Game::State::RUN)
    {
        if (this->window != NULL)
            this->sdl2_poll();

        /* Proceeding in a round-robin fashion as setup in the startup phase, every player is given the opportunity to do sequentially each of the following actions during their turn
         */
        for (auto player: this->players)
        {
            cout << "Turn of " << player->getName() <<
                    " (press enter to continue)" << endl;
            if (cin.get() == '\n')
            {
                // action 1
                player->reinforce();
                // action 2
                player->attack();
                // action 3
                player->fortify();
            }
        }
    }
}

bool Game::addPlayer(Player *player)
{
    this->players.push_back(player);
    return true;
}
