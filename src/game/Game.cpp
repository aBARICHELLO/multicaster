#include "Game.h"
#include "../Path.h"
#include "../states/GameState.h"
#include "../states/MainMenuState.h"
#include "../states/State.h"

Game::Game()
    : window(sf::VideoMode().getDesktopMode(), "multicaster", sf::Style::Fullscreen),
      stateManager(State::SharedContext(window, textures, fonts)) {
    window.setFramerateLimit(Global::MAX_FRAMERATE);
    window.setVerticalSyncEnabled(true);

    loadResources();
    registerStates();

    stateManager.push(StateType::MainMenu);
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float delta = clock.restart().asSeconds();
        event();
        update(delta);
        draw();

        if (stateManager.isEmpty()) {
            window.close();
        }
    }
}

void Game::event() {
    sf::Event event;
    while (window.pollEvent(event)) {
        stateManager.input(event);
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update(float delta) {
    stateManager.update(delta);
}

void Game::draw() {
    window.clear(sf::Color::Black);
    stateManager.draw();
    window.display();
}

void Game::loadResources() {
    fonts.load(Resources::DEBUG_FONT, Path::DEBUG_FONT);
    fonts.load(Resources::MENU_FONT, Path::MENU_FONT);
}

void Game::registerStates() {
    stateManager.registerState<MainMenuState>(StateType::MainMenu);
    stateManager.registerState<GameState>(StateType::Game);
}
