#include <fstream>

#include "MultiplayerState.h"
#include "network/Protocol.h"

MultiplayerState::MultiplayerState(StateManager& stateManager, State::SharedContext context, bool host)
    : State(stateManager, context), host(host) {
    sf::IpAddress ip;
    if (host) {
        server.reset(new Server());
        ip = LOCALHOST;
    } else {
        ip = getIPFromFile();
    }

    if (socket.connect(ip, SERVER_PORT, CONNECTION_TIMEOUT) == sf::TcpSocket::Done) {
        connected = true;
    } else {
        failedConnection.restart();
    }

    socket.setBlocking(false);
}

void MultiplayerState::draw() {
    return;
}

void MultiplayerState::handleEvent(const sf::Event& event) {
    return;
}

void MultiplayerState::update(float delta) {
    // Handle messages from server
    if (connected) {
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done) {
            sf::Int32 packetHeader;
            packet >> packetHeader;
            handlePacket(packetHeader, packet);
        } else {
            if (lastPacketReceived > CONNECTION_TIMEOUT) {
                connected = false;
                failedConnection.restart();
                return;
            }
        }

        // Position update package
        if (tickClock.getElapsedTime() > sf::seconds(1.0f / 30.0f)) {
            sf::Packet positionUpdate;
            positionUpdate << static_cast<sf::Int32>(Packet::Client::PositionUpdate);
            positionUpdate << playerID;
            // positionUpdate << TODO: Position

            socket.send(positionUpdate);
            tickClock.restart();
        }
    }
}

void MultiplayerState::handlePacket(sf::Int32 packetHeader, sf::Packet& packet) {
    switch (packetHeader) {
        case Packet::Server::BroadcastMessage:
            std::string message;
            packet >> message;
            broadcasts.push_back(message);
            break;
    }
}

void MultiplayerState::updateBroadcastMessage(sf::Time elapsedTime) {
}

// TODO: placeholder until a save file is implemented for last visited server
sf::IpAddress MultiplayerState::getIPFromFile() {
    std::ifstream file("ip.txt");
    std::string ipString;
    file >> ipString;
    return sf::IpAddress(ipString);
}
