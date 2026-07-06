#include "game_interface.h"
#include <psapi.h>
#include <iostream>

#pragma comment(lib, "psapi.lib")

GameInterface::GameInterface() 
    : processHandle(NULL), processID(0), connected(false) {
}

GameInterface::~GameInterface() {
    detachFromGame();
}

bool GameInterface::initializeConnection() {
    if (!findGameProcess()) {
        return false;
    }
    
    if (!attachToGame()) {
        return false;
    }
    
    connected = true;
    return true;
}

bool GameInterface::attachToGame() {
    if (processID == 0) return false;
    
    processHandle = OpenProcess(
        PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
        FALSE,
        processID
    );
    
    return processHandle != NULL;
}

void GameInterface::detachFromGame() {
    if (processHandle) {
        CloseHandle(processHandle);
        processHandle = NULL;
    }
    connected = false;
}

bool GameInterface::findGameProcess() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if (Process32First(hSnapshot, &pe32)) {
        do {
            std::string processName = pe32.szExeFile;
            
            if (processName.find("com.tencent.ig") != std::string::npos ||
                processName.find("BGMI") != std::string::npos ||
                processName.find("tencent") != std::string::npos) {
                
                processID = pe32.th32ProcessID;
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    return false;
}

uintptr_t GameInterface::getBaseAddress(const std::string& moduleName) {
    if (!processHandle) return 0;
    
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
    
    if (Module32First(hSnapshot, &me32)) {
        do {
            if (std::string(me32.szModule).find(moduleName) != std::string::npos) {
                CloseHandle(hSnapshot);
                return (uintptr_t)me32.modBaseAddr;
            }
        } while (Module32Next(hSnapshot, &me32));
    }
    
    CloseHandle(hSnapshot);
    return 0;
}

bool GameInterface::readPlayerData() {
    if (!connected || !processHandle) return false;
    uintptr_t playerListAddr = getBaseAddress("UnityPlayer.dll");
    return true;
}

std::vector<uintptr_t> GameInterface::getPlayerList() {
    return playerAddresses;
}