#include "ban_bypass.h"
#include "bgmi_esp.h"
#include "game_interface.h"
#include "anti_detect.h"
#include <iostream>
#include <windows.h>

int main() {
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════╗\n";
    std::cout << "║  KEVIN BGMI BAN BYPASS SYSTEM     ║\n";
    std::cout << "║  Version 1.0                      ║\n";
    std::cout << "╚═══════════════════════════════════╝\n\n";
    
    // Initialize bypass system
    BanBypass banBypass;
    AntiDetectSystem antiDetect;
    
    std::cout << "[*] Initializing Ban Bypass System...\n";
    std::cout << "[*] Please wait...\n\n";
    
    // Apply all bypass techniques
    std::cout << "[+] Applying Device Spoofing...\n";
    banBypass.spoofDeviceID();
    banBypass.spoofMachineGUID();
    banBypass.spoofHardwareID();
    banBypass.spoofIMEI();
    
    std::cout << "\n[+] Clearing Ban Cache and Data...\n";
    banBypass.clearLocalBanCache();
    banBypass.clearBGMICacheFiles();
    banBypass.deleteBGMILocalData();
    banBypass.clearBanRegistryEntries();
    
    std::cout << "\n[+] Modifying Registry...\n";
    banBypass.modifyRegistryHardwareID();
    banBypass.modifyRegistryMachineID();
    
    std::cout << "\n[+] Changing IP Address...\n";
    banBypass.changeIPAddress();
    banBypass.enableVPN();
    
    std::cout << "\n[+] Applying Anti-Detection...\n";
    antiDetect.enable();
    antiDetect.obfuscateMemory();
    antiDetect.hideModuleFromList();
    antiDetect.spoofModuleSignature();
    
    std::cout << "\n[+] Creating New Device Profile...\n";
    banBypass.createNewDeviceProfile();
    banBypass.wipeDeviceIdentifiers();
    
    std::cout << "\n[+] Bypassing Hardware Checks...\n";
    banBypass.bypassHardwareChecks();
    
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════╗\n";
    std::cout << "║  BAN BYPASS COMPLETED!            ║\n";
    std::cout << "║  All techniques applied           ║\n";
    std::cout << "╚═══════════════════════════════════╝\n\n";
    
    // Display applied bypasses
    std::cout << "[*] Applied Bypass Techniques:\n";
    for (const auto& bypass : banBypass.getAppliedBypasses()) {
        std::cout << "    ✓ " << bypass << "\n";
    }
    
    std::cout << "\n[*] Status: READY TO USE\n";
    std::cout << "[*] Anti-Detection: " << (antiDetect.isEnabled() ? "ENABLED" : "DISABLED") << "\n";
    std::cout << "[*] Ban Bypass: " << (banBypass.isEnabled() ? "ENABLED" : "DISABLED") << "\n\n";
    
    std::cout << "[!] Now you can restart BGMI and login!\n";
    std::cout << "[!] WARNING: This is for educational purposes only!\n\n";
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}