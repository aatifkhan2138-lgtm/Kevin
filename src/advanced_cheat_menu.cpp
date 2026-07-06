#include "advanced_cheat_menu.h"
#include <imgui.h>

AdvancedCheatMenu::AdvancedCheatMenu()
    : menuOpen(true), espEnabled(false), aimbotEnabled(false),
      silentAimEnabled(false), espScanRadius(500.0f),
      espDrawBox(true), espDrawHealth(true), espDrawDistance(true),
      aimbotSensitivity(1.0f), aimbotFOV(30.0f), aimbotModeSelected(0),
      silentAimFireRate(0.1f), silentAimModeSelected(0),
      silentAimHitboxSelected(0), silentAimSmoothing(0.8f),
      autoFireEnabled(false), triggerbotEnabled(false) {
}

AdvancedCheatMenu::~AdvancedCheatMenu() {
}

void AdvancedCheatMenu::render() {
    if (!menuOpen) return;
    
    ImGui::SetNextWindowSize(ImVec2(500, 700), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Kevin Advanced Cheat Menu", &menuOpen, ImGuiWindowFlags_NoMove)) {
        renderMainMenu();
        
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("ESP Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            renderESPSettings();
        }
        
        if (ImGui::CollapsingHeader("Aimbot Settings")) {
            renderAimbotSettings();
        }
        
        if (ImGui::CollapsingHeader("Silent Aim Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            renderSilentAimSettings();
        }
        
        if (ImGui::CollapsingHeader("Debug Info")) {
            renderDebugInfo();
        }
        
        ImGui::End();
    }
}

void AdvancedCheatMenu::renderMainMenu() {
    ImGui::Text("=== KEVIN ADVANCED CHEAT SYSTEM ===");
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Educational Purpose Only");
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "WARNING: Use at own risk!");
    
    ImGui::Separator();
    
    if (ImGui::Checkbox("ESP Enabled##main", &espEnabled)) {
        if (espEnabled) {
            espSystem.scanScreen();
        }
    }
    ImGui::SameLine();
    
    if (ImGui::Checkbox("Aimbot Enabled##main", &aimbotEnabled)) {
        if (aimbotEnabled) {
            aimbotSystem.enable();
        } else {
            aimbotSystem.disable();
        }
    }
    ImGui::SameLine();
    
    if (ImGui::Checkbox("Silent Aim Enabled##main", &silentAimEnabled)) {
        if (silentAimEnabled) {
            silentAimSystem.enable();
        } else {
            silentAimSystem.disable();
        }
    }
}

void AdvancedCheatMenu::renderESPSettings() {
    ImGui::SliderFloat("ESP Scan Radius##esp", &espScanRadius, 50.0f, 1000.0f);
    
    ImGui::Checkbox("Draw Box##esp", &espDrawBox);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Health##esp", &espDrawHealth);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Distance##esp", &espDrawDistance);
    
    ImGui::Text("Detected Enemies: %zu", espSystem.getEnemies().size());
    
    Enemy* nearest = espSystem.getNearestEnemy();
    if (nearest) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                          "Nearest Enemy - Distance: %.2f, Health: %.0f", 
                          nearest->distance, nearest->health);
    }
    
    if (ImGui::Button("Force Scan##esp")) {
        espSystem.scanScreen();
    }
}

void AdvancedCheatMenu::renderAimbotSettings() {
    const char* aimbotModes[] = {"OFF", "LOCK", "SMOOTH", "PREDICT"};
    ImGui::Combo("Aimbot Mode##aimbot", &aimbotModeSelected, aimbotModes, 4);
    aimbotSystem.setMode(static_cast<AimbotMode>(aimbotModeSelected));
    
    ImGui::SliderFloat("Aimbot Sensitivity##aimbot", &aimbotSensitivity, 0.1f, 10.0f);
    aimbotSystem.setSensitivity(aimbotSensitivity);
    
    ImGui::SliderFloat("Field of View##aimbot", &aimbotFOV, 5.0f, 180.0f);
    aimbotSystem.setFOV(aimbotFOV);
    
    ImGui::Text("Status: %s | Sensitivity: %.2f | FOV: %.1f°", 
               aimbotModes[aimbotModeSelected], 
               aimbotSystem.getSensitivity(), 
               aimbotSystem.getFOV());
}

void AdvancedCheatMenu::renderSilentAimSettings() {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "SILENT AIM SETTINGS");
    
    const char* silentAimModes[] = {"OFF", "AUTO", "MANUAL", "TRIGGERBOT"};
    ImGui::Combo("Silent Aim Mode##silentaim", &silentAimModeSelected, silentAimModes, 4);
    silentAimSystem.setMode(static_cast<SilentAimMode>(silentAimModeSelected));
    
    const char* hitboxModes[] = {"HEAD", "CHEST", "LEGS"};
    ImGui::Combo("Hitbox Target##silentaim", &silentAimHitboxSelected, hitboxModes, 3);
    silentAimSystem.setHitboxMode(silentAimHitboxSelected);
    
    ImGui::SliderFloat("Fire Rate (Auto)##silentaim", &silentAimFireRate, 0.05f, 1.0f);
    silentAimSystem.setAutoFireRate(silentAimFireRate);
    
    ImGui::SliderFloat("Aim Smoothing##silentaim", &silentAimSmoothing, 0.0f, 1.0f);
    silentAimSystem.setSmoothing(silentAimSmoothing);
    
    ImGui::Separator();
    
    if (ImGui::Checkbox("Auto Fire##silentaim", &autoFireEnabled)) {
        if (autoFireEnabled) {
            silentAimSystem.setMode(SILENT_AIM_AUTO);
        }
    }
    ImGui::SameLine();
    
    if (ImGui::Checkbox("Triggerbot##silentaim", &triggerbotEnabled)) {
        if (triggerbotEnabled) {
            silentAimSystem.setMode(SILENT_AIM_TRIGGERBOT);
        }
    }
    
    ImGui::Text("Successful Shots: %d / %d", 
               silentAimSystem.getSuccessfulShots(), 
               silentAimSystem.getTotalShots());
    
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), 
                      "Mode: %s | Hitbox: %s | Fire Rate: %.3f", 
                      silentAimModes[silentAimModeSelected],
                      hitboxModes[silentAimHitboxSelected],
                      silentAimFireRate);
}

void AdvancedCheatMenu::renderDebugInfo() {
    ImGui::Text("=== DEBUG INFORMATION ===");
    ImGui::Separator();
    
    ImGui::Text("ESP Status: %s", espEnabled ? "ACTIVE" : "INACTIVE");
    ImGui::Text("Aimbot Status: %s", aimbotEnabled ? "ACTIVE" : "INACTIVE");
    ImGui::Text("Silent Aim Status: %s", silentAimEnabled ? "ACTIVE" : "INACTIVE");
    
    ImGui::Separator();
    ImGui::Text("Total Enemies: %zu", espSystem.getEnemies().size());
    ImGui::Text("Total Shots: %d", silentAimSystem.getTotalShots());
    ImGui::Text("Accuracy: %.1f%%", 
               silentAimSystem.getTotalShots() > 0 ? 
               (float)silentAimSystem.getSuccessfulShots() / silentAimSystem.getTotalShots() * 100.0f : 0.0f);
    
    ImGui::Separator();
    
    if (ImGui::Button("Reset All Statistics")) {
        espEnabled = false;
        aimbotEnabled = false;
        silentAimEnabled = false;
        aimbotSystem.disable();
        silentAimSystem.disable();
    }
}

void AdvancedCheatMenu::update() {
    espSystem.updateEnemyList();
    aimbotSystem.update();
    silentAimSystem.update();
    
    if (aimbotEnabled) {
        Enemy* target = espSystem.getClosestToCenter();
        if (target) {
            aimbotSystem.aim(target);
        }
    }
    
    if (silentAimEnabled) {
        Enemy* target = espSystem.getClosestToCenter();
        if (target) {
            silentAimSystem.fireWithSilentAim(target);
        }
    }
}

ESPSystem& AdvancedCheatMenu::getESP() {
    return espSystem;
}

AimbotSystem& AdvancedCheatMenu::getAimbot() {
    return aimbotSystem;
}

SilentAimSystem& AdvancedCheatMenu::getSilentAim() {
    return silentAimSystem;
}
