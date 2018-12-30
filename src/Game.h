#pragma once

#include "SFML/Graphics.hpp"

#include "FPS.h"
#include "Player.h"
#include "ResourceHolder.h"
#include "states/StateManager.h"

class Game {
public:
    Game();

    void run();

private:
    void event();
    void process(float delta);
    void draw();

    void loadResources();

    sf::RenderWindow window;
    StateManager stateManager;
    TextureHolder textures;
    FontHolder fonts;
    Player player1;
};
