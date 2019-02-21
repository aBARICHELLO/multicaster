#include "Game.h"
#include "states/GameState.h"
#include "states/MainMenuState.h"
#include "states/MultiplayerState.h"
#include "states/PauseState.h"
#include "states/State.h"
#include "util/Filepath.h"

Game::Game()
    : window(sf::VideoMode().getDesktopMode(), "multicaster", sf::Style::Default),
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
        stateManager.handleEvent(event);
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::Resized) {
            window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
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
    fonts.load(Resources::DEBUG_FONT, Filepath::DEBUG_FONT);
    fonts.load(Resources::MENU_FONT, Filepath::MENU_FONT);
}

void Game::registerStates() {
    stateManager.registerState<MainMenuState>(StateType::MainMenu);
    stateManager.registerState<GameState>(StateType::SingleplayerGame);
    stateManager.registerState<MultiplayerState>(StateType::HostMultiplayer, true);
    stateManager.registerState<MultiplayerState>(StateType::PeerMultiplayer, false);
}
