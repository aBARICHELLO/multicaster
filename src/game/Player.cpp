#include <cmath>
#include <iostream>

#include "Player.h"

Player::Player()
    : position(2.0f, 2.0f),
      direction(0.0f, 1.0f),
      plane(-0.65f, 0.0f),
      lines(sf::Lines, screenRes.width),
      fps(),
      debug(sf::Vector2f(0.0f, 30.0f)) {
}

Player::Player(sf::Vector2f startPosition) : Player() {
    this->position = startPosition;
}

void Player::input() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        moveForward();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        moveBackward();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        turnLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        turnRight();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace)) {
        position = sf::Vector2f(2.0f, 2.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tilde)) {
        this->debugMode = !debugMode;
    }
}

void Player::update(float delta) {
    this->delta = delta;
    input();
    raycast();
    fps.update(delta);
}

void Player::raycast() {
    lines.clear();

    for (int i = 0; i < screenRes.width; ++i) {
        // Rays initial positions
        sf::Vector2f rayPos = position;
        sf::Vector2i worldPos(position);

        float cameraX = 2.0f * (float)i / (float)screenRes.width - 1.0f;
        sf::Vector2f rayDir = direction + plane * cameraX;
        sf::Vector2f deltaDist;
        deltaDist.x = std::abs(1.0f / rayDir.x);
        deltaDist.y = std::abs(1.0f / rayDir.y);

        sf::Vector2i step;
        sf::Vector2f sideDist;
        if (rayDir.x < 0.0f) {
            step.x = -1;
            sideDist.x = (rayPos.x - worldPos.x) * deltaDist.x;
        } else {
            step.x = 1;
            sideDist.x = (worldPos.x + 1 - rayPos.x) * deltaDist.x;
        }

        if (rayDir.y < 0.0f) {
            step.y = -1;
            sideDist.y = (rayPos.y - worldPos.y) * deltaDist.y;
        } else {
            step.y = 1;
            sideDist.y = (worldPos.y + 1 - rayPos.y) * deltaDist.y;
        }

        // Cast rays using Digital Differential Analysis(DDA) until hitting a wall
        bool hit = false;
        bool horizontal = false;
        while (!hit) {
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                worldPos.x += step.x;
                horizontal = true;
            } else {
                sideDist.y += deltaDist.y;
                worldPos.y += step.y;
                horizontal = false;
            }

            if (map[(int)worldPos.x][(int)worldPos.y] > 0) {
                hit = true;
            }
        }

        float perpWallDist = 0.0f;
        if (horizontal) {
            perpWallDist = std::fabs(((float)worldPos.x - rayPos.x + (1.0f - (float)step.x) / 2.0f) / rayDir.x);
        } else {
            perpWallDist = std::fabs(((float)worldPos.y - rayPos.y + (1.0f - (float)step.y) / 2.0f) / rayDir.y);
        }

        // Determine line height
        int lineHeight = (int)std::abs(screenRes.height / perpWallDist);
        int drawStart = -lineHeight / 2 + screenRes.height / 2;
        if (drawStart < 0) {
            drawStart = 0;
        }
        int drawEnd = lineHeight / 2 + screenRes.height / 2;
        if (drawEnd >= screenRes.height) {
            drawEnd = screenRes.height - 1;
        }

        // Shadow horizontal walls
        sf::Color color = sf::Color::Red;
        if (horizontal) {
            color.r /= 2;
            color.g /= 2;
            color.b /= 2;
        }

        if (debugMode) {
            std::string debugText =
                "PositionX: " + std::to_string(position.x) + " PositionY: " + std::to_string(position.y);
            debug.setText(debugText);
        }

        // Append columns to VertexArray
        lines.append(sf::Vertex(sf::Vector2f((float)i, (float)drawStart), color));
        lines.append(sf::Vertex(sf::Vector2f((float)i, (float)drawEnd), color));
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(lines);
    if (debugMode) {
        fps.draw(window);
        debug.draw(window);
    }
}

void Player::moveForward() {
    float deltaMovement = direction.x * movementSpeed * delta;
    int x = int(position.x + deltaMovement);
    int y = int(position.y);
    if (map[x][y] == 0) {
        position.x += deltaMovement;
    }

    deltaMovement = direction.y * movementSpeed * delta;
    x = int(position.x);
    y = int(position.y + deltaMovement);
    if (map[x][y] == 0) {
        position.y += deltaMovement;
    }
}

void Player::moveBackward() {
    float deltaMovement = direction.x * movementSpeed * delta;
    int x = int(position.x - deltaMovement);
    int y = int(position.y);
    if (map[x][y] == 0) {
        position.x -= deltaMovement;
    }

    deltaMovement = direction.y * movementSpeed * delta;
    x = int(position.x);
    y = int(position.y - deltaMovement);
    if (map[x][y] == 0) {
        position.y -= deltaMovement;
    }
}

void Player::turnLeft() {
    direction = rotateVector(direction, -1.0f * turnSpeed * delta);
    plane = rotateVector(plane, turnSpeed * delta * -1.0f);
}

void Player::turnRight() {
    direction = rotateVector(direction, 1.0f * turnSpeed * delta);
    plane = rotateVector(plane, turnSpeed * delta);
}

// https://en.wikipedia.org/wiki/Rotation_matrix
sf::Vector2f Player::rotateVector(sf::Vector2f input, float value) {
    float x = (input.x * std::cos(value) - input.y * std::sin(value));
    float y = (input.x * std::sin(value) + input.y * std::cos(value));
    return sf::Vector2f(x, y);
}