#include "bgmi_esp.h"
#include "anti_detect.h"
#include <imgui.h>
#include <iostream>

class BGMICheatMenu {
private:
    GameInterface gameInterface;
    BGMIESP espSystem;
    AntiDetectSystem antiDetectSystem;
    
    bool menuOpen;
    bool espEnabled;
    bool connectionActive;
    float scanRadius;
    
public:
    BGMICheatMenu() 
        : espSystem(&gameInterface), menuOpen(true), espEnabled(false),
          connectionActive(false), scanRadius(500.0f) {
    }
    
    bool initialize() {
        // Try to connect to BGMI process
        if (!gameInterface.initializeConnection()) {
            return false;
        }
        
        connectionActive = true;
        return true;
    }
    
    void render() {
        if (!menuOpen) return;
        
        ImGui::SetNextWindowSize(ImVec2(600, 700), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.9f);
        
        if (ImGui::Begin("Kevin BGMI Cheat", &menuOpen, ImGuiWindowFlags_NoMove)) {
            renderConnectionStatus();
            ImGui::Separator();
            
            if (connectionActive) {
                renderESPSettings();
                ImGui::Separator();
                renderPlayerList();
                ImGui::Separator();
                renderProtection();
            } else {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
                                  "NOT CONNECTED TO BGMI!");
                if (ImGui::Button("Reconnect##main")) {
                    initialize();
                }
            }
            
            ImGui::End();
        }
    }
    
    void renderConnectionStatus() {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                          "=== KEVIN BGMI CHEAT ===");
        
        if (connectionActive) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                              "Status: CONNECTED");
            ImGui::Text("Process ID: %d", gameInterface.getProcessID());
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
                              "Status: DISCONNECTED");
        }
    }
    
    void renderESPSettings() {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ESP SETTINGS");
        
        if (ImGui::Checkbox("Enable ESP##main", &espEnabled)) {
            if (espEnabled) {
                espSystem.scanPlayers();
            }
        }
        
        ImGui::SliderFloat("Scan Radius", &scanRadius, 50.0f, 2000.0f);
        
        if (ImGui::Button("Scan Now##esp")) {
            espSystem.updatePlayerList();
        }
        
        ImGui::SameLine();
        ImGui::Text("Players: %zu", espSystem.getPlayers().size());
    }
    
    void renderPlayerList() {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "DETECTED PLAYERS");
        
        const auto& players = espSystem.getPlayers();
        
        if (players.empty()) {
            ImGui::Text("No players detected");
            return;
        }
        
        ImGui::BeginTable("PlayerTable", 5, ImGuiTableFlags_Borders);
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Health");
        ImGui::TableSetupColumn("Distance");
        ImGui::TableSetupColumn("X");
        ImGui::TableSetupColumn("Y");
        ImGui::TableHeadersRow();
        
        for (const auto& player : players) {
            if (!player.isAlive) continue;
            
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", player.playerId);
            
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(
                player.health > 50 ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                "%.1f", player.health
            );
            
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f", player.distance);
            
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f", player.position.x);
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.1f", player.position.y);
        }
        
        ImGui::EndTable();
    }
    
    void renderProtection() {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "PROTECTION");
        
        static bool protectionEnabled = false;
        if (ImGui::Checkbox("Enable Anti-Detection##main", &protectionEnabled)) {
            if (protectionEnabled) {
                antiDetectSystem.enable();
            } else {
                antiDetectSystem.disable();
            }
        }
    }
    
    void update() {
        if (connectionActive && espEnabled) {
            espSystem.updatePlayerList();
        }
    }
    
    bool isConnected() const {
        return connectionActive;
    }
};

int main() {
    BGMICheatMenu menu;
    
    // Initialize connection
    if (!menu.initialize()) {
        std::cerr << "Failed to connect to BGMI process!" << std::endl;
        std::cerr << "Make sure BGMI is running!" << std::endl;
        return 1;
    }
    
    std::cout << "Successfully connected to BGMI!" << std::endl;
    
    // Main loop
    while (true) {
        menu.update();
        menu.render();
    }
    
    return 0;
}
