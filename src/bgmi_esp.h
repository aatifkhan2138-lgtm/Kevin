#ifndef BGMI_ESP_H
#define BGMI_ESP_H

#include "game_interface.h"
#include <vector>
#include <glm/glm.hpp>

struct BGMIPlayer {
    uintptr_t address;
    glm::vec3 position;
    glm::vec3 headPosition;
    float health;
    int playerId;
    bool isAlive;
    bool isVisible;
    float distance;
};

class BGMIESP {
public:
    BGMIESP(GameInterface* gameInterface);
    ~BGMIESP();
    
    void scanPlayers();
    void updatePlayerList();
    void renderESP();
    
    const std::vector<BGMIPlayer>& getPlayers() const;
    BGMIPlayer* getNearestPlayer();
    BGMIPlayer* getPlayerByID(int id);
    
    bool isConnected() const;
    
private:
    GameInterface* gameInterface;
    std::vector<BGMIPlayer> players;
    
    bool readPlayerFromMemory(uintptr_t playerAddr, BGMIPlayer& player);
    bool isPlayerValid(const BGMIPlayer& player);
    void calculateDistance(BGMIPlayer& player);
};

#endif