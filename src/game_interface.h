#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>

namespace BGMIOffsets {
    const uintptr_t PLAYER_HEALTH = 0x2A4;
    const uintptr_t PLAYER_POSITION_X = 0x118;
    const uintptr_t PLAYER_POSITION_Y = 0x11C;
    const uintptr_t PLAYER_POSITION_Z = 0x120;
    const uintptr_t PLAYER_HEAD_X = 0x128;
    const uintptr_t PLAYER_HEAD_Y = 0x12C;
    const uintptr_t PLAYER_HEAD_Z = 0x130;
    const uintptr_t PLAYER_STATE = 0x200;
    const uintptr_t ENTITY_LIST_OFFSET = 0x8B4E9C;
}

class GameInterface {
public:
    GameInterface();
    ~GameInterface();
    
    bool initializeConnection();
    bool attachToGame();
    void detachFromGame();
    
    template<typename T>
    T readMemory(uintptr_t address) {
        T value = 0;
        if (processHandle) {
            ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(T), NULL);
        }
        return value;
    }
    
    template<typename T>
    bool writeMemory(uintptr_t address, T value) {
        if (!processHandle) return false;
        return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), NULL);
    }
    
    bool findGameProcess();
    uintptr_t getBaseAddress(const std::string& moduleName);
    bool readPlayerData();
    std::vector<uintptr_t> getPlayerList();
    
    HANDLE getProcessHandle() const { return processHandle; }
    DWORD getProcessID() const { return processID; }
    bool isConnected() const { return connected; }
    
private:
    HANDLE processHandle;
    DWORD processID;
    bool connected;
    std::vector<uintptr_t> playerAddresses;
};

#endif