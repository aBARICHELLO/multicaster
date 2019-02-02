#include "GameState.h"
#include "GLOBAL.h"

GameState::GameState(StateManager& stateManager, SharedContext context) : State(stateManager, context) {
}

bool GameState::handleEvent(const sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape) {
        requestPop();
        requestPush(StateType::MainMenu);
    }
    return true;
}

bool GameState::update(float delta) {
    player1.update(delta);
    return true;
}

void GameState::draw() {
    sf::RenderWindow& window = *context.window;
    player1.draw(window);
}
