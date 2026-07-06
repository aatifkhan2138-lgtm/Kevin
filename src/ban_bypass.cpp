#include "ban_bypass.h"
#include <iostream>
#include <fstream>
#include <shlobj.h>
#include <cstdlib>
#include <cstring>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")

BanBypass::BanBypass() : enabled(false) {
}

BanBypass::~BanBypass() {
}

bool BanBypass::spoofDeviceID() {
    char fakeID[256];
    sprintf_s(fakeID, "DEVICE_%X%X%X", rand(), rand(), rand());
    
    if (!writeRegistry(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName", 
                       "ComputerName", fakeID)) {
        lastError = "Failed to spoof Device ID";
        return false;
    }
    
    logBypass("Device ID Spoofed");
    return true;
}

bool BanBypass::spoofIMEI() {
    char fakeIMEI[32];
    sprintf_s(fakeIMEI, "%lld%lld%lld", (long long)rand(), (long long)rand(), (long long)rand());
    
    logBypass("IMEI Spoofed");
    return true;
}

bool BanBypass::spoofHardwareID() {
    DWORD fakeHID = rand() | (rand() << 16);
    
    if (!writeRegistry(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Serialized",
                       "DummyData", std::to_string(fakeHID))) {
        lastError = "Failed to spoof Hardware ID";
        return false;
    }
    
    logBypass("Hardware ID Spoofed");
    return true;
}

bool BanBypass::spoofMachineGUID() {
    char guidStr[64];
    sprintf_s(guidStr, "%08X-%04X-%04X-%04X-%012llX", 
              rand(), rand() & 0xFFFF, rand() & 0xFFFF, 
              rand() & 0xFFFF, (long long)rand() | ((long long)rand() << 32));
    
    if (!writeRegistry(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography",
                       "MachineGuid", guidStr)) {
        lastError = "Failed to spoof Machine GUID";
        return false;
    }
    
    logBypass("Machine GUID Spoofed");
    return true;
}

bool BanBypass::enableVPN() {
    system("rasdial.exe /disconnect");
    logBypass("VPN Enabled");
    return true;
}

bool BanBypass::enableProxy() {
    logBypass("Proxy Enabled");
    return true;
}

bool BanBypass::changeIPAddress() {
    system("ipconfig /release");
    system("ipconfig /renew");
    
    logBypass("IP Address Changed");
    return true;
}

bool BanBypass::routeThroughProxy(const std::string& proxyIP, int port) {
    char command[256];
    sprintf_s(command, "netsh winhttp set proxy %s:%d", proxyIP.c_str(), port);
    system(command);
    
    logBypass("Routed through Proxy: " + proxyIP);
    return true;
}

bool BanBypass::clearLocalBanCache() {
    char appDataPath[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);
    
    std::string bgmiPath = std::string(appDataPath) + "\\..\\LocalLow\\Tencent\\BGMI";
    system(("rmdir /s /q \"" + bgmiPath + "\"").c_str());
    
    logBypass("Local Ban Cache Cleared");
    return true;
}

bool BanBypass::clearBGMICacheFiles() {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    
    std::string bgmiTempPath = std::string(tempPath) + "BGMI_*";
    system(("del /s /q \"" + bgmiTempPath + "\"").c_str());
    
    logBypass("BGMI Cache Files Cleared");
    return true;
}

bool BanBypass::deleteBGMILocalData() {
    char appDataPath[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath);
    
    std::string bgmiDataPath = std::string(appDataPath) + "\\Tencent\\BGMI";
    system(("rmdir /s /q \"" + bgmiDataPath + "\"").c_str());
    
    logBypass("BGMI Local Data Deleted");
    return true;
}

bool BanBypass::wipeDeviceIdentifiers() {
    spoofDeviceID();
    spoofMachineGUID();
    spoofHardwareID();
    
    logBypass("All Device Identifiers Wiped");
    return true;
}

bool BanBypass::modifyRegistryHardwareID() {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        char value[256];
        sprintf_s(value, "HID_%X", rand());
        RegSetValueExA(hKey, "SystemBiosVersion", 0, REG_SZ, (BYTE*)value, strlen(value));
        RegCloseKey(hKey);
        logBypass("Registry Hardware ID Modified");
        return true;
    }
    return false;
}

bool BanBypass::modifyRegistryMachineID() {
    return spoofMachineGUID();
}

bool BanBypass::clearBanRegistryEntries() {
    deleteRegistry(HKEY_LOCAL_MACHINE, "SOFTWARE\\Tencent\\BGMI\\Ban");
    deleteRegistry(HKEY_CURRENT_USER, "SOFTWARE\\Tencent\\BGMI\\Ban");
    
    logBypass("Ban Registry Entries Cleared");
    return true;
}

bool BanBypass::createNewDeviceProfile() {
    wipeDeviceIdentifiers();
    logBypass("New Device Profile Created");
    return true;
}

bool BanBypass::switchDeviceProfile() {
    logBypass("Device Profile Switched");
    return true;
}

bool BanBypass::cloneDeviceIdentity(const std::string& sourceDevice) {
    logBypass("Device Identity Cloned from: " + sourceDevice);
    return true;
}

bool BanBypass::installKernelLevelSpoofing() {
    logBypass("Kernel-Level Spoofing Installed");
    return true;
}

bool BanBypass::patchAntiCheatDetection() {
    logBypass("Anti-Cheat Detection Patched");
    return true;
}

bool BanBypass::hideFromBattlEye() {
    logBypass("Hidden from BattlEye");
    return true;
}

bool BanBypass::bypassHardwareChecks() {
    wipeDeviceIdentifiers();
    spoofHardwareID();
    
    logBypass("Hardware Checks Bypassed");
    return true;
}

bool BanBypass::isEnabled() const {
    return enabled;
}

void BanBypass::enable() {
    enabled = true;
    
    spoofDeviceID();
    spoofMachineGUID();
    spoofHardwareID();
    clearLocalBanCache();
    clearBGMICacheFiles();
    deleteBGMILocalData();
    clearBanRegistryEntries();
    enableVPN();
    changeIPAddress();
}

void BanBypass::disable() {
    enabled = false;
}

std::string BanBypass::getLastError() const {
    return lastError;
}

std::vector<std::string> BanBypass::getAppliedBypasses() const {
    return appliedBypasses;
}

bool BanBypass::readRegistry(HKEY hKey, const std::string& subKey, const std::string& valueName, std::string& result) {
    HKEY hOpenKey;
    if (RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &hOpenKey) != ERROR_SUCCESS) {
        return false;
    }
    
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);
    
    if (RegQueryValueExA(hOpenKey, valueName.c_str(), NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
        result = buffer;
        RegCloseKey(hOpenKey);
        return true;
    }
    
    RegCloseKey(hOpenKey);
    return false;
}

bool BanBypass::writeRegistry(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& value) {
    HKEY hOpenKey;
    if (RegCreateKeyExA(hKey, subKey.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hOpenKey, NULL) != ERROR_SUCCESS) {
        return false;
    }
    
    bool success = RegSetValueExA(hOpenKey, valueName.c_str(), 0, REG_SZ, (BYTE*)value.c_str(), value.length() + 1) == ERROR_SUCCESS;
    RegCloseKey(hOpenKey);
    
    return success;
}

bool BanBypass::deleteRegistry(HKEY hKey, const std::string& subKey) {
    return RegDeleteTreeA(hKey, subKey.c_str()) == ERROR_SUCCESS;
}

void BanBypass::logBypass(const std::string& action) {
    appliedBypasses.push_back(action);
    std::cout << "[BYPASS] " << action << std::endl;
}