#ifndef BAN_BYPASS_H
#define BAN_BYPASS_H

#include <windows.h>
#include <string>
#include <vector>

class BanBypass {
public:
    BanBypass();
    ~BanBypass();
    
    // Device spoofing
    bool spoofDeviceID();
    bool spoofIMEI();
    bool spoofHardwareID();
    bool spoofMachineGUID();
    
    // IP spoofing
    bool enableVPN();
    bool enableProxy();
    bool changeIPAddress();
    bool routeThroughProxy(const std::string& proxyIP, int port);
    
    // Ban cache clearing
    bool clearLocalBanCache();
    bool clearBGMICacheFiles();
    bool deleteBGMILocalData();
    bool wipeDeviceIdentifiers();
    
    // Registry manipulation
    bool modifyRegistryHardwareID();
    bool modifyRegistryMachineID();
    bool clearBanRegistryEntries();
    
    // Account spoofing
    bool createNewDeviceProfile();
    bool switchDeviceProfile();
    bool cloneDeviceIdentity(const std::string& sourceDevice);
    
    // Advanced techniques
    bool installKernelLevelSpoofing();
    bool patchAntiCheatDetection();
    bool hideFromBattlEye();
    bool bypassHardwareChecks();
    
    // Status
    bool isEnabled() const;
    void enable();
    void disable();
    
    std::string getLastError() const;
    std::vector<std::string> getAppliedBypasses() const;
    
private:
    bool enabled;
    std::string lastError;
    std::vector<std::string> appliedBypasses;
    
    bool readRegistry(HKEY hKey, const std::string& subKey, const std::string& valueName, std::string& result);
    bool writeRegistry(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& value);
    bool deleteRegistry(HKEY hKey, const std::string& subKey);
    
    void logBypass(const std::string& action);
};

#endif