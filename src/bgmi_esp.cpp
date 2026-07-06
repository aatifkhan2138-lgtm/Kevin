#include "bgmi_esp.h"
#include <algorithm>

BGMIESP::BGMIESP(GameInterface* gameInterface) 
    : gameInterface(gameInterface) {
}

BGMIESP::~BGMIESP() {
}

void BGMIESP::scanPlayers() {
    if (!gameInterface || !gameInterface->isConnected()) return;
    
    players.clear();
    std::vector<uintptr_t> playerList = gameInterface->getPlayerList();
    
    for (uintptr_t playerAddr : playerList) {
        BGMIPlayer player;
        player.address = playerAddr;
        
        if (readPlayerFromMemory(playerAddr, player)) {
            if (isPlayerValid(player)) {
                calculateDistance(player);
                players.push_back(player);
            }
        }
    }
}

void BGMIESP::updatePlayerList() {
    scanPlayers();
}

void BGMIESP::renderESP() {
    for (const auto& player : players) {
        if (player.isVisible && player.isAlive) {
            // Render ESP
        }
    }
}

const std::vector<BGMIPlayer>& BGMIESP::getPlayers() const {
    return players;
}

BGMIPlayer* BGMIESP::getNearestPlayer() {
    if (players.empty()) return nullptr;
    
    auto nearest = std::min_element(
        players.begin(),
        players.end(),
        [](const BGMIPlayer& a, const BGMIPlayer& b) {
            return a.distance < b.distance;
        }
    );
    
    return &(*nearest);
}

BGMIPlayer* BGMIESP::getPlayerByID(int id) {
    for (auto& player : players) {
        if (player.playerId == id) {
            return &player;
        }
    }
    return nullptr;
}

bool BGMIESP::isConnected() const {
    return gameInterface && gameInterface->isConnected();
}

bool BGMIESP::readPlayerFromMemory(uintptr_t playerAddr, BGMIPlayer& player) {
    if (!gameInterface) return false;
    
    try {
        player.health = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_HEALTH
        );
        
        player.position.x = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_POSITION_X
        );
        player.position.y = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_POSITION_Y
        );
        player.position.z = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_POSITION_Z
        );
        
        player.headPosition.x = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_HEAD_X
        );
        player.headPosition.y = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_HEAD_Y
        );
        player.headPosition.z = gameInterface->readMemory<float>(
            playerAddr + BGMIOffsets::PLAYER_HEAD_Z
        );
        
        int state = gameInterface->readMemory<int>(
            playerAddr + BGMIOffsets::PLAYER_STATE
        );
        player.isAlive = (state == 1);
        
        return true;
    }
    catch (...) {
        return false;
    }
}

bool BGMIESP::isPlayerValid(const BGMIPlayer& player) {
    if (player.health <= 0) return false;
    if (!player.isAlive) return false;
    if (player.position.x == 0 && player.position.y == 0 && player.position.z == 0) return false;
    return true;
}

void BGMIESP::calculateDistance(BGMIPlayer& player) {
    player.distance = glm::length(player.position);
}