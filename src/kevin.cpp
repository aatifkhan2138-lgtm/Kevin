#include "advanced_cheat_menu.h"
#include "anti_detect.h"
#include <imgui.h>

class FullCheatMenu {
private:
    ESPSystem espSystem;
    AimbotSystem aimbotSystem;
    SilentAimSystem silentAimSystem;
    AntiDetectSystem antiDetectSystem;
    
    bool menuOpen;
    bool espEnabled;
    bool aimbotEnabled;
    bool silentAimEnabled;
    bool antiDetectEnabled;
    
    // Settings
    float espScanRadius;
    float aimbotSensitivity;
    float silentAimFireRate;
    
    int aimbotModeSelected;
    int silentAimModeSelected;
    int silentAimHitboxSelected;
    
public:
    FullCheatMenu() 
        : menuOpen(true), espEnabled(false), aimbotEnabled(false),
          silentAimEnabled(false), antiDetectEnabled(false),
          espScanRadius(500.0f), aimbotSensitivity(1.0f),
          silentAimFireRate(0.1f), aimbotModeSelected(0),
          silentAimModeSelected(0), silentAimHitboxSelected(0) {
    }
    
    void render() {
        if (!menuOpen) return;
        
        ImGui::SetNextWindowSize(ImVec2(600, 850), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Kevin Complete System", &menuOpen)) {
            renderHeader();
            ImGui::Separator();
            
            if (ImGui::CollapsingHeader("Protection & Bypass", ImGuiTreeNodeFlags_DefaultOpen)) {
                renderAntiDetectSettings();
            }
            
            if (ImGui::CollapsingHeader("ESP Settings")) {
                renderESPSettings();
            }
            
            if (ImGui::CollapsingHeader("Aimbot Settings")) {
                renderAimbotSettings();
            }
            
            if (ImGui::CollapsingHeader("Silent Aim Settings")) {
                renderSilentAimSettings();
            }
            
            if (ImGui::CollapsingHeader("Debug Info")) {
                renderDebugInfo();
            }
            
            ImGui::End();
        }
    }
    
    void renderHeader() {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "=== KEVIN COMPLETE SYSTEM ===");
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "With Ban Protection & Bypass");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "!!! EDUCATIONAL USE ONLY !!!");
    }
    
    void renderAntiDetectSettings() {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "ANTI-DETECTION SETTINGS");
        
        if (ImGui::Checkbox("Enable Protection System##main", &antiDetectEnabled)) {
            if (antiDetectEnabled) {
                antiDetectSystem.enable();
            } else {
                antiDetectSystem.disable();
            }
        }
        
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                          antiDetectEnabled ? "PROTECTED" : "UNPROTECTED");
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Anti-Cheat Bypass Options:");
        
        if (ImGui::Button("Apply EAC Bypass", ImVec2(150, 0))) {
            antiDetectSystem.evadeEAC();
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Apply BattlEye Bypass", ImVec2(150, 0))) {
            antiDetectSystem.evadeBattlEye();
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Apply VAC Bypass", ImVec2(120, 0))) {
            antiDetectSystem.evadeVAC();
        }
        
        if (ImGui::Button("Apply WinGuard Bypass", ImVec2(150, 0))) {
            antiDetectSystem.evadeWinGuard();
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Apply XignCode Bypass", ImVec2(150, 0))) {
            antiDetectSystem.evadeXignCode();
        }
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Memory Protection:");
        
        if (ImGui::Button("Obfuscate Memory##protect", ImVec2(140, 0))) {
            antiDetectSystem.obfuscateMemory();
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Hide Module##protect", ImVec2(140, 0))) {
            antiDetectSystem.hideModuleFromList();
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Spoof Signature##protect", ImVec2(140, 0))) {
            antiDetectSystem.spoofModuleSignature();
        }
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Status Info:");
        ImGui::Text("Protection: %s", antiDetectSystem.isEnabled() ? "ACTIVE" : "INACTIVE");
        ImGui::Text("Last Status: %s", antiDetectSystem.getLastBypassStatus());
        ImGui::Text("Bypasses Applied: %d", antiDetectSystem.getSuccessfulBypassCount());
    }
    
    void renderESPSettings() {
        ImGui::Checkbox("ESP Enabled##main", &espEnabled);
        ImGui::SliderFloat("Scan Radius##esp", &espScanRadius, 50.0f, 1000.0f);
        ImGui::Text("Detected Enemies: %zu", espSystem.getEnemies().size());
    }
    
    void renderAimbotSettings() {
        ImGui::Checkbox("Aimbot Enabled##main", &aimbotEnabled);
        
        const char* modes[] = {"OFF", "LOCK", "SMOOTH", "PREDICT"};
        ImGui::Combo("Aimbot Mode##aimbot", &aimbotModeSelected, modes, 4);
        ImGui::SliderFloat("Sensitivity##aimbot", &aimbotSensitivity, 0.1f, 10.0f);
    }
    
    void renderSilentAimSettings() {
        ImGui::Checkbox("Silent Aim Enabled##main", &silentAimEnabled);
        
        const char* modes[] = {"OFF", "AUTO", "MANUAL", "TRIGGERBOT"};
        ImGui::Combo("Silent Aim Mode##silentaim", &silentAimModeSelected, modes, 4);
        
        const char* hitboxes[] = {"HEAD", "CHEST", "LEGS"};
        ImGui::Combo("Hitbox##silentaim", &silentAimHitboxSelected, hitboxes, 3);
        
        ImGui::SliderFloat("Fire Rate##silentaim", &silentAimFireRate, 0.05f, 1.0f);
    }
    
    void renderDebugInfo() {
        ImGui::Text("Protection: %s", antiDetectEnabled ? "ACTIVE" : "INACTIVE");
        ImGui::Text("ESP: %s", espEnabled ? "ACTIVE" : "INACTIVE");
        ImGui::Text("Aimbot: %s", aimbotEnabled ? "ACTIVE" : "INACTIVE");
        ImGui::Text("Silent Aim: %s", silentAimEnabled ? "ACTIVE" : "INACTIVE");
        
        ImGui::Separator();
        if (ImGui::Button("Disable All Systems")) {
            espEnabled = false;
            aimbotEnabled = false;
            silentAimEnabled = false;
            antiDetectEnabled = false;
        }
    }
    
    void update() {
        if (espEnabled) espSystem.updateEnemyList();
        if (aimbotEnabled) aimbotSystem.update();
        if (silentAimEnabled) silentAimSystem.update();
    }
};

int main() {
    FullCheatMenu menu;
    
    while (true) {
        menu.update();
        menu.render();
    }
    
    return 0;
}
