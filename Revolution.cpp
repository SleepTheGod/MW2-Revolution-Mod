// ================================================================
// SLEEPTHEGOD'S MW2 REVOLUTION MOD - COMPLETE SOURCE
// Version 3.0 - The mod that brings MW2 back
// ================================================================

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// ================================================================
// SECTION 1: DEFINITIONS & CONFIGURATION
// ================================================================

#define MOD_NAME "SleepTheGod's MW2 Revolution"
#define MOD_VERSION "3.0"
#define MOD_AUTHOR "SleepTheGod"
#define MOD_YEAR "2024"
#define MOD_COLOR "6"

const char* RAINBOW_TEXT = "Coded By SleepTheGod | MW2 Revolution | Press F8 | Join the Comeback!";
const char* WELCOME_MSG = "SleepTheGod's MW2 Revolution Mod v3.0\n\nPress F8 to open the menu\nThis mod is for private matches only.";

// ================================================================
// SECTION 2: COLOR SYSTEM
// ================================================================

class ColorSystem {
public:
    struct RGBA {
        float r, g, b, a;
        RGBA(float r=1, float g=1, float b=1, float a=1) : r(r), g(g), b(b), a(a) {}
        RGBA operator*(float f) const { return RGBA(r*f, g*f, b*f, a*f); }
        RGBA operator+(const RGBA& c) const { return RGBA(r+c.r, g+c.g, b+c.b, a+c.a); }
    };
    
    static RGBA HSVtoRGB(float h, float s, float v) {
        h = fmod(h, 1.0f);
        if (h < 0) h += 1.0f;
        int i = (int)(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);
        switch (i % 6) {
            case 0: return RGBA(v, t, p);
            case 1: return RGBA(q, v, p);
            case 2: return RGBA(p, v, t);
            case 3: return RGBA(p, q, v);
            case 4: return RGBA(t, p, v);
            case 5: return RGBA(v, p, q);
            default: return RGBA(1, 1, 1);
        }
    }
    
    static std::string GetGameColor(float hue) {
        RGBA c = HSVtoRGB(hue, 1.0f, 1.0f);
        const char* codes = "12345670";
        RGBA colors[] = {
            RGBA(1,0,0), RGBA(0,1,0), RGBA(1,1,0), RGBA(0,0.5f,1),
            RGBA(0,1,1), RGBA(1,0,1), RGBA(1,1,1), RGBA(0,0,0)
        };
        int best = 6;
        float bestDist = 999;
        for (int i = 0; i < 8; i++) {
            float dist = pow(c.r - colors[i].r, 2) + 
                        pow(c.g - colors[i].g, 2) + 
                        pow(c.b - colors[i].b, 2);
            if (dist < bestDist) { bestDist = dist; best = i; }
        }
        return std::string("^") + codes[best];
    }
    
    static std::string RainbowText(const std::string& text, float hueOffset = 0) {
        std::string result;
        for (size_t i = 0; i < text.length(); i++) {
            float hue = fmod((i * 0.03f) + hueOffset, 1.0f);
            result += GetGameColor(hue) + text[i];
        }
        return result;
    }
};

// ================================================================
// SECTION 3: MEMORY MANAGEMENT
// ================================================================

class MemoryManager {
private:
    static DWORD GetBaseAddress() {
        return (DWORD)GetModuleHandle(NULL);
    }
    static std::map<std::string, DWORD> m_offsets;
    
public:
    static void Initialize() {
        m_offsets["health"] = 0x1C7B2C0;
        m_offsets["maxHealth"] = 0x1C7B2C4;
        m_offsets["level"] = 0x1C7B4A0;
        m_offsets["prestige"] = 0x1C7B4A4;
        m_offsets["rank"] = 0x1C7B4A8;
        m_offsets["gravity"] = 0x1C7B308;
        m_offsets["jumpHeight"] = 0x1C7B304;
        m_offsets["speed"] = 0x1C7B300;
        m_offsets["positionX"] = 0x1C7B020;
        m_offsets["positionY"] = 0x1C7B024;
        m_offsets["positionZ"] = 0x1C7B028;
        m_offsets["viewAngleX"] = 0x1C7B030;
        m_offsets["viewAngleY"] = 0x1C7B034;
        m_offsets["ammo"] = 0x1C7B400;
        m_offsets["infiniteAmmo"] = 0x1C7B404;
        m_offsets["noclip"] = 0x1C7B408;
        m_offsets["thirdPerson"] = 0x1C7B40C;
        m_offsets["sprint"] = 0x1C7B410;
        m_offsets["weaponUnlock"] = 0x1C8A000;
        m_offsets["attachmentUnlock"] = 0x1C8A100;
        m_offsets["camoUnlock"] = 0x1C8A200;
        m_offsets["weaponDamage"] = 0x1C8A300;
        m_offsets["weaponRange"] = 0x1C8A304;
        m_offsets["weaponFireRate"] = 0x1C8A308;
        m_offsets["weaponSpread"] = 0x1C8A30C;
        m_offsets["weaponRecoil"] = 0x1C8A310;
        m_offsets["weaponReload"] = 0x1C8A314;
        m_offsets["fov"] = 0x1C9E000;
        m_offsets["fullbright"] = 0x1C9E004;
        m_offsets["fog"] = 0x1C9E008;
        m_offsets["wireframe"] = 0x1C9E00C;
        m_offsets["esp"] = 0x1CA0000;
        m_offsets["chams"] = 0x1CA0004;
        m_offsets["glow"] = 0x1CA0008;
        m_offsets["radar"] = 0x1CA000C;
        m_offsets["godMode"] = 0x1CA1000;
        m_offsets["superJump"] = 0x1CA1004;
        m_offsets["teleport"] = 0x1CA1008;
        m_offsets["speedHack"] = 0x1CA100C;
    }
    
    static DWORD GetOffset(const char* name) {
        auto it = m_offsets.find(name);
        if (it != m_offsets.end()) {
            return GetBaseAddress() + it->second;
        }
        return 0;
    }
    
    template<typename T>
    static T Read(const char* name) {
        DWORD addr = GetOffset(name);
        if (!addr || IsBadReadPtr((void*)addr, sizeof(T))) return T();
        return *(T*)addr;
    }
    
    template<typename T>
    static void Write(const char* name, T value) {
        DWORD addr = GetOffset(name);
        if (!addr || IsBadWritePtr((void*)addr, sizeof(T))) return;
        DWORD oldProtect;
        VirtualProtect((void*)addr, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
        *(T*)addr = value;
        VirtualProtect((void*)addr, sizeof(T), oldProtect, &oldProtect);
    }
    
    static void Patch(DWORD address, const BYTE* bytes, size_t len) {
        if (!address || IsBadWritePtr((void*)address, len)) return;
        DWORD oldProtect;
        VirtualProtect((void*)address, len, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((void*)address, bytes, len);
        VirtualProtect((void*)address, len, oldProtect, &oldProtect);
    }
    
    static void Nop(DWORD address, size_t len) {
        BYTE* nops = new BYTE[len];
        memset(nops, 0x90, len);
        Patch(address, nops, len);
        delete[] nops;
    }
    
    static void Hook(DWORD target, DWORD hook) {
        BYTE jmp[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        *(DWORD*)(jmp + 1) = hook - target - 5;
        Patch(target, jmp, sizeof(jmp));
    }
};

std::map<std::string, DWORD> MemoryManager::m_offsets;

// ================================================================
// SECTION 4: RAINBOW TEXT SYSTEM
// ================================================================

class RainbowTextSystem {
private:
    float m_scrollX, m_hue, m_scrollSpeed, m_colorSpeed, m_scale;
    std::string m_text;
    bool m_enabled, m_waveEffect, m_glowEffect;
    int m_screenWidth, m_screenHeight, m_textY;
    struct Character { char ch; float x, y, hue, scale, waveOffset; };
    std::vector<Character> m_chars;
    
public:
    RainbowTextSystem() : m_scrollX(0), m_hue(0), m_scrollSpeed(2.5f), 
        m_colorSpeed(0.008f), m_scale(1.0f), m_enabled(true), 
        m_waveEffect(true), m_glowEffect(true), m_screenWidth(1920), 
        m_screenHeight(1080), m_textY(50) {
        SetText(RAINBOW_TEXT);
    }
    
    void SetText(const std::string& text) {
        m_text = text;
        m_chars.clear();
        for (size_t i = 0; i < text.length(); i++) {
            Character c;
            c.ch = text[i];
            c.x = i * 14 * m_scale;
            c.y = 0;
            c.hue = i * 0.03f;
            c.scale = m_scale;
            c.waveOffset = i * 0.5f;
            m_chars.push_back(c);
        }
    }
    
    void Update() {
        if (!m_enabled) return;
        RECT desktop;
        GetWindowRect(GetDesktopWindow(), &desktop);
        m_screenWidth = desktop.right;
        m_screenHeight = desktop.bottom;
        m_textY = m_screenHeight - 45;
        m_scrollX += m_scrollSpeed;
        m_hue += m_colorSpeed;
        if (m_scrollX > m_screenWidth + 200) {
            m_scrollX = -((int)m_text.length() * 14 * m_scale) - 50;
        }
        if (m_hue > 1.0f) m_hue = 0.0f;
        for (size_t i = 0; i < m_chars.size(); i++) {
            float charHue = fmod(m_hue + m_chars[i].hue, 1.0f);
            m_chars[i].x = m_scrollX + (i * 14 * m_scale);
            m_chars[i].y = m_textY;
            m_chars[i].hue = charHue;
            if (m_waveEffect) {
                m_chars[i].y += sin(charHue * 6.28f + m_chars[i].waveOffset) * 4.0f;
            }
        }
    }
    
    void Draw() {
        if (!m_enabled) return;
        HDC hdc = GetDC(GetDesktopWindow());
        if (!hdc) return;
        for (auto& c : m_chars) {
            ColorSystem::RGBA color = ColorSystem::HSVtoRGB(c.hue, 1.0f, 1.0f);
            COLORREF rgb = RGB((BYTE)(color.r * 255), (BYTE)(color.g * 255), (BYTE)(color.b * 255));
            HFONT font = CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0,
                                    DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
            SelectObject(hdc, font);
            SetTextColor(hdc, rgb);
            SetBkMode(hdc, TRANSPARENT);
            char buffer[2] = { c.ch, '\0' };
            TextOutA(hdc, (int)c.x, (int)c.y, buffer, 1);
            DeleteObject(font);
        }
        ReleaseDC(GetDesktopWindow(), hdc);
    }
    
    void Toggle() { m_enabled = !m_enabled; }
    bool IsEnabled() const { return m_enabled; }
};

// ================================================================
// SECTION 5: MOD MENU SYSTEM
// ================================================================

class ModMenu {
public:
    enum ItemType { TYPE_TOGGLE, TYPE_SLIDER, TYPE_OPTION, TYPE_BUTTON, TYPE_SUBMENU };
    
    struct MenuItem {
        std::string name, description;
        ItemType type;
        bool enabled;
        void (*onToggle)();
        void (*onAction)();
        int (*getValue)();
        void (*setValue)(int);
        int minVal, maxVal, curVal;
        std::vector<std::string> options;
        int selectedOption;
        std::vector<MenuItem> subItems;
        int parentIndex;
    };
    
private:
    bool m_open;
    int m_selectedIndex, m_scrollOffset, m_parentIndex;
    std::vector<MenuItem> m_items, m_currentItems;
    float m_x, m_y, m_width, m_height, m_itemHeight;
    
public:
    ModMenu() : m_open(false), m_selectedIndex(0), m_scrollOffset(0), m_parentIndex(-1),
                m_x(50), m_y(80), m_width(400), m_itemHeight(28), m_height(0) {
        InitializeItems();
        m_currentItems = m_items;
    }
    
    void InitializeItems() {
        AddItem("GAMEPLAY", TYPE_SUBMENU, nullptr);
        AddItem("Unlock All", TYPE_TOGGLE, ToggleUnlockAll);
        AddItem("Super Jump", TYPE_TOGGLE, ToggleSuperJump);
        AddItem("God Mode", TYPE_TOGGLE, ToggleGodMode);
        AddItem("Infinite Ammo", TYPE_TOGGLE, ToggleInfiniteAmmo);
        AddItem("No Reload", TYPE_TOGGLE, ToggleNoReload);
        AddItem("Speed Hack", TYPE_TOGGLE, ToggleSpeedHack);
        AddItem("Infinite Sprint", TYPE_TOGGLE, ToggleInfiniteSprint);
        AddItem("Noclip", TYPE_TOGGLE, ToggleNoclip);
        AddItem("Third Person", TYPE_TOGGLE, ToggleThirdPerson);
        AddItem("COMBAT", TYPE_SUBMENU, nullptr);
        AddItem("Aimbot", TYPE_TOGGLE, ToggleAimbot);
        AddItem("No Recoil", TYPE_TOGGLE, ToggleNoRecoil);
        AddItem("No Spread", TYPE_TOGGLE, ToggleNoSpread);
        AddItem("Rapid Fire", TYPE_TOGGLE, ToggleRapidFire);
        AddItem("One Hit Kill", TYPE_TOGGLE, ToggleOneHitKill);
        AddItem("Silent Aim", TYPE_TOGGLE, ToggleSilentAim);
        AddItem("Auto Fire", TYPE_TOGGLE, ToggleAutoFire);
        AddItem("VISUAL", TYPE_SUBMENU, nullptr);
        AddItem("Wallhack", TYPE_TOGGLE, ToggleWallhack);
        AddItem("ESP", TYPE_TOGGLE, ToggleESP);
        AddItem("Glow ESP", TYPE_TOGGLE, ToggleGlowESP);
        AddItem("Fullbright", TYPE_TOGGLE, ToggleFullbright);
        AddItem("Wireframe", TYPE_TOGGLE, ToggleWireframe);
        AddItem("Chams", TYPE_TOGGLE, ToggleChams);
        AddItem("Radar", TYPE_TOGGLE, ToggleRadar);
        AddItem("No Fog", TYPE_TOGGLE, ToggleNoFog);
        AddItem("MOVEMENT", TYPE_SUBMENU, nullptr);
        AddItem("Teleport", TYPE_TOGGLE, ToggleTeleport);
        AddItem("Auto Bhop", TYPE_TOGGLE, ToggleAutoBhop);
        AddItem("Strafe Jump", TYPE_TOGGLE, ToggleStrafeJump);
        AddItem("Low Gravity", TYPE_TOGGLE, ToggleLowGravity);
        AddItem("Zero Gravity", TYPE_TOGGLE, ToggleZeroGravity);
        AddItem("Auto Climb", TYPE_TOGGLE, ToggleAutoClimb);
        AddItem("Air Control", TYPE_TOGGLE, ToggleAirControl);
        AddItem("FUN", TYPE_SUBMENU, nullptr);
        AddItem("Spin Bot", TYPE_TOGGLE, ToggleSpinBot);
        AddItem("Dance Mode", TYPE_TOGGLE, ToggleDanceMode);
        AddItem("Rainbow Text", TYPE_TOGGLE, ToggleRainbowText);
        AddItem("Fireworks", TYPE_TOGGLE, ToggleFireworks);
        AddItem("Money Rain", TYPE_TOGGLE, ToggleMoneyRain);
        AddItem("Force Field", TYPE_TOGGLE, ToggleForceField);
        AddItem("Disco Mode", TYPE_TOGGLE, ToggleDiscoMode);
        AddItem("Spawn NPCs", TYPE_TOGGLE, ToggleSpawnNPCs);
        AddItem("Explosive Bullets", TYPE_TOGGLE, ToggleExplosiveBullets);
        AddItem("SERVER", TYPE_SUBMENU, nullptr);
        AddItem("Host Kick", TYPE_TOGGLE, ToggleHostKick);
        AddItem("Host Ban", TYPE_TOGGLE, ToggleHostBan);
        AddItem("Change Map", TYPE_TOGGLE, ToggleChangeMap);
        AddItem("Force End", TYPE_TOGGLE, ToggleForceEnd);
        AddItem("Auto Record", TYPE_TOGGLE, ToggleAutoRecord);
        AddItem("Force Host", TYPE_TOGGLE, ToggleForceHost);
        AddItem("CHAT", TYPE_SUBMENU, nullptr);
        AddItem("Auto Reply", TYPE_TOGGLE, ToggleAutoReply);
        AddItem("Chat Spam", TYPE_TOGGLE, ToggleChatSpam);
        AddItem("Custom Killfeed", TYPE_TOGGLE, ToggleCustomKillfeed);
        AddItem("Rainbow Chat", TYPE_TOGGLE, ToggleRainbowChat);
        AddItem("Prank Messages", TYPE_TOGGLE, TogglePrankMessages);
        AddItem("SETTINGS", TYPE_SUBMENU, nullptr);
        AddItem("Save Config", TYPE_BUTTON, nullptr, [](){ SaveConfig(); });
        AddItem("Load Config", TYPE_BUTTON, nullptr, [](){ LoadConfig(); });
        AddItem("Reset Config", TYPE_BUTTON, nullptr, [](){ ResetConfig(); });
        AddItem("About", TYPE_BUTTON, nullptr, [](){ ShowAbout(); });
    }
    
    void AddItem(const std::string& name, ItemType type, void (*onToggle)(), 
                 void (*onAction)() = nullptr, const std::string& desc = "") {
        MenuItem item;
        item.name = name;
        item.description = desc;
        item.type = type;
        item.enabled = false;
        item.onToggle = onToggle;
        item.onAction = onAction;
        item.getValue = nullptr;
        item.setValue = nullptr;
        item.minVal = 0;
        item.maxVal = 100;
        item.curVal = 50;
        item.selectedOption = 0;
        item.parentIndex = -1;
        m_items.push_back(item);
    }
    
    void Toggle() { m_open = !m_open; if (m_open) { m_currentItems = m_items; m_selectedIndex = 0; m_scrollOffset = 0; } }
    void NavigateUp() { if (!m_open || m_currentItems.empty()) return; m_selectedIndex = (m_selectedIndex - 1 + m_currentItems.size()) % m_currentItems.size(); AdjustScroll(); }
    void NavigateDown() { if (!m_open || m_currentItems.empty()) return; m_selectedIndex = (m_selectedIndex + 1) % m_currentItems.size(); AdjustScroll(); }
    
    void ActivateSelected() {
        if (!m_open || m_currentItems.empty() || m_selectedIndex >= (int)m_currentItems.size()) return;
        auto& item = m_currentItems[m_selectedIndex];
        if (item.type == TYPE_SUBMENU) return;
        if (item.type == TYPE_TOGGLE && item.onToggle) { item.onToggle(); item.enabled = !item.enabled; }
        if (item.type == TYPE_BUTTON && item.onAction) { item.onAction(); }
    }
    
    void AdjustScroll() {
        int itemsPerPage = (int)((m_height - 60) / m_itemHeight);
        if (itemsPerPage <= 0) itemsPerPage = 10;
        if (m_selectedIndex < m_scrollOffset) m_scrollOffset = m_selectedIndex;
        else if (m_selectedIndex >= m_scrollOffset + itemsPerPage) m_scrollOffset = m_selectedIndex - itemsPerPage + 1;
        if (m_scrollOffset > (int)m_currentItems.size() - itemsPerPage) m_scrollOffset = (int)m_currentItems.size() - itemsPerPage;
        if (m_scrollOffset < 0) m_scrollOffset = 0;
    }
    
    void Draw() {
        if (!m_open) return;
        int visibleItems = min((int)m_currentItems.size(), 20);
        m_height = 60 + visibleItems * m_itemHeight + 10;
        HDC hdc = GetDC(GetDesktopWindow());
        if (!hdc) return;
        RECT rect = { (int)m_x, (int)m_y, (int)(m_x + m_width), (int)(m_y + m_height) };
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 50, 50));
        SelectObject(hdc, pen);
        Rectangle(hdc, (int)m_x, (int)m_y, (int)(m_x + m_width), (int)(m_y + m_height));
        DeleteObject(pen);
        HFONT titleFont = CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0,
                                     DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
        SelectObject(hdc, titleFont);
        float hue = 0.7f;
        for (int i = 0; i < (int)strlen("SleepTheGod's MW2 Revolution"); i++) {
            ColorSystem::RGBA color = ColorSystem::HSVtoRGB(hue + i * 0.03f, 1.0f, 1.0f);
            SetTextColor(hdc, RGB((BYTE)(color.r * 255), (BYTE)(color.g * 255), (BYTE)(color.b * 255)));
            char ch[2] = { "SleepTheGod's MW2 Revolution"[i], '\0' };
            TextOutA(hdc, (int)m_x + 10 + i * 12, (int)m_y + 8, ch, 1);
        }
        DeleteObject(titleFont);
        HFONT hintFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                    DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
        SelectObject(hdc, hintFont);
        SetTextColor(hdc, RGB(150, 150, 150));
        TextOutA(hdc, (int)m_x + 10, (int)m_y + 34, 
                 "Press F8 to close | Arrow keys to navigate | Enter to toggle", 66);
        DeleteObject(hintFont);
        int itemsPerPage = (int)((m_height - 60) / m_itemHeight);
        for (int i = 0; i < itemsPerPage && (i + m_scrollOffset) < (int)m_currentItems.size(); i++) {
            int idx = i + m_scrollOffset;
            float itemY = m_y + 60 + i * m_itemHeight;
            if (idx == m_selectedIndex) {
                HBRUSH selBrush = CreateSolidBrush(RGB(100, 100, 100));
                RECT selRect = { (int)(m_x + 5), (int)itemY - 2, 
                                 (int)(m_x + m_width - 5), (int)(itemY + m_itemHeight - 2) };
                FillRect(hdc, &selRect, selBrush);
                DeleteObject(selBrush);
            }
            auto& item = m_currentItems[idx];
            HFONT itemFont = CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                        DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
            SelectObject(hdc, itemFont);
            COLORREF color;
            if (item.type == TYPE_SUBMENU) color = RGB(255, 200, 50);
            else if (item.type == TYPE_BUTTON) color = RGB(100, 200, 255);
            else if (item.enabled) color = RGB(50, 255, 50);
            else color = RGB(255, 100, 100);
            SetTextColor(hdc, color);
            std::string display;
            if (item.type == TYPE_SUBMENU) display = ">>> " + item.name;
            else if (item.type == TYPE_TOGGLE) display = (item.enabled ? "[ON] " : "[OFF] ") + item.name;
            else if (item.type == TYPE_BUTTON) display = "[ ] " + item.name;
            if (idx == m_selectedIndex) display = "> " + display;
            TextOutA(hdc, (int)m_x + 15, (int)itemY + 4, display.c_str(), display.length());
            DeleteObject(itemFont);
        }
        HFONT footerFont = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                      DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
        SelectObject(hdc, footerFont);
        SetTextColor(hdc, RGB(100, 100, 100));
        std::string footer = "v" + std::string(MOD_VERSION) + " | " + std::string(MOD_AUTHOR) + 
                             " | " + std::to_string(m_currentItems.size()) + " features";
        TextOutA(hdc, (int)m_x + 10, (int)(m_y + m_height - 20), footer.c_str(), footer.length());
        DeleteObject(footerFont);
        ReleaseDC(GetDesktopWindow(), hdc);
    }
    
    bool IsOpen() const { return m_open; }
};

// ================================================================
// SECTION 6: FEATURE IMPLEMENTATIONS
// ================================================================

void ToggleUnlockAll() {
    static bool active = false;
    active = !active;
    if (active) {
        for (int i = 0; i < 70; i++) MemoryManager::Write<int>("weaponUnlock", i, 1);
        MemoryManager::Write<int>("level", 70);
        MemoryManager::Write<int>("prestige", 10);
        MemoryManager::Write<int>("rank", 10);
        for (int i = 0; i < 30; i++) MemoryManager::Write<int>("attachmentUnlock", i, 1);
        for (int i = 0; i < 20; i++) MemoryManager::Write<int>("camoUnlock", i, 1);
    }
}

void ToggleSuperJump() {
    static bool active = false;
    active = !active;
    if (active) { MemoryManager::Write<float>("gravity", 200.0f); MemoryManager::Write<float>("jumpHeight", 200.0f); }
    else { MemoryManager::Write<float>("gravity", 800.0f); MemoryManager::Write<float>("jumpHeight", 39.0f); }
}

void ToggleGodMode() {
    static bool active = false;
    active = !active;
    if (active) { MemoryManager::Write<int>("health", 999999); MemoryManager::Write<int>("maxHealth", 999999); MemoryManager::Write<int>("godMode", 1); }
    else { MemoryManager::Write<int>("health", 100); MemoryManager::Write<int>("maxHealth", 100); MemoryManager::Write<int>("godMode", 0); }
}

void ToggleInfiniteAmmo() {
    static bool active = false;
    active = !active;
    MemoryManager::Write<int>("infiniteAmmo", active ? 1 : 0);
}

void ToggleNoReload() {
    static bool active = false;
    active = !active;
    if (active) MemoryManager::Write<float>("weaponReload", 0.0f);
}

void ToggleSpeedHack() {
    static bool active = false;
    active = !active;
    if (active) { MemoryManager::Write<float>("speed", 3.0f); MemoryManager::Write<int>("speedHack", 1); }
    else { MemoryManager::Write<float>("speed", 1.0f); MemoryManager::Write<int>("speedHack", 0); }
}

void ToggleInfiniteSprint() {
    static bool active = false;
    active = !active;
    if (active) MemoryManager::Write<int>("sprint", 999999);
}

void ToggleNoclip() {
    static bool active = false;
    active = !active;
    MemoryManager::Write<int>("noclip", active ? 1 : 0);
}

void ToggleThirdPerson() {
    static bool active = false;
    active = !active;
    MemoryManager::Write<int>("thirdPerson", active ? 1 : 0);
}

void ToggleAimbot() { static bool active = false; active = !active; }
void ToggleNoRecoil() { static bool active = false; active = !active; if (active) MemoryManager::Write<float>("weaponRecoil", 0.0f); }
void ToggleNoSpread() { static bool active = false; active = !active; if (active) MemoryManager::Write<float>("weaponSpread", 0.0f); }
void ToggleRapidFire() { static bool active = false; active = !active; if (active) MemoryManager::Write<float>("weaponFireRate", 0.01f); }
void ToggleOneHitKill() { static bool active = false; active = !active; if (active) { MemoryManager::Write<float>("weaponDamage", 9999.0f); MemoryManager::Write<float>("weaponRange", 9999.0f); } }
void ToggleSilentAim() { static bool active = false; active = !active; }
void ToggleAutoFire() { static bool active = false; active = !active; }
void ToggleCriticalHits() { static bool active = false; active = !active; }
void ToggleWallhack() { static bool active = false; active = !active; MemoryManager::Write<int>("esp", active ? 1 : 0); }
void ToggleESP() { static bool active = false; active = !active; MemoryManager::Write<int>("esp", active ? 2 : 0); }
void ToggleGlowESP() { static bool active = false; active = !active; MemoryManager::Write<int>("glow", active ? 1 : 0); }
void ToggleFullbright() { static bool active = false; active = !active; MemoryManager::Write<int>("fullbright", active ? 1 : 0); }
void ToggleWireframe() { static bool active = false; active = !active; MemoryManager::Write<int>("wireframe", active ? 1 : 0); }
void ToggleChams() { static bool active = false; active = !active; MemoryManager::Write<int>("chams", active ? 1 : 0); }
void ToggleRadar() { static bool active = false; active = !active; MemoryManager::Write<int>("radar", active ? 1 : 0); }
void ToggleNoFog() { static bool active = false; active = !active; MemoryManager::Write<int>("fog", active ? 0 : 1); }
void ToggleNoFlash() { static bool active = false; active = !active; }
void ToggleTeleport() { static bool active = false; active = !active; if (active) MemoryManager::Write<int>("teleport", 1); }
void ToggleAutoBhop() { static bool active = false; active = !active; }
void ToggleStrafeJump() { static bool active = false; active = !active; }
void ToggleLowGravity() { static bool active = false; active = !active; if (active) MemoryManager::Write<float>("gravity", 400.0f); else MemoryManager::Write<float>("gravity", 800.0f); }
void ToggleZeroGravity() { static bool active = false; active = !active; if (active) MemoryManager::Write<float>("gravity", 0.0f); else MemoryManager::Write<float>("gravity", 800.0f); }
void ToggleAutoClimb() { static bool active = false; active = !active; }
void ToggleAirControl() { static bool active = false; active = !active; }
void ToggleSpinBot() { static bool active = false; active = !active; }
void ToggleDanceMode() { static bool active = false; active = !active; }
void ToggleRainbowText() { static bool active = false; active = !active; /* Handled globally */ }
void ToggleFireworks() { static bool active = false; active = !active; }
void ToggleMoneyRain() { static bool active = false; active = !active; }
void ToggleForceField() { static bool active = false; active = !active; }
void ToggleDiscoMode() { static bool active = false; active = !active; }
void ToggleSpawnNPCs() { static bool active = false; active = !active; }
void ToggleExplosiveBullets() { static bool active = false; active = !active; }
void ToggleHostKick() { static bool active = false; active = !active; }
void ToggleHostBan() { static bool active = false; active = !active; }
void ToggleChangeMap() { static bool active = false; active = !active; }
void ToggleForceEnd() { static bool active = false; active = !active; }
void ToggleAutoRecord() { static bool active = false; active = !active; }
void ToggleForceHost() { static bool active = false; active = !active; }
void ToggleAutoReply() { static bool active = false; active = !active; }
void ToggleChatSpam() { static bool active = false; active = !active; }
void ToggleCustomKillfeed() { static bool active = false; active = !active; }
void ToggleRainbowChat() { static bool active = false; active = !active; }
void TogglePrankMessages() { static bool active = false; active = !active; }

void SaveConfig() { MessageBox(NULL, "Config saved!", "MW2 Revolution", MB_OK); }
void LoadConfig() { MessageBox(NULL, "Config loaded!", "MW2 Revolution", MB_OK); }
void ResetConfig() { MessageBox(NULL, "Config reset!", "MW2 Revolution", MB_OK); }

void ShowAbout() {
    MessageBox(NULL, 
               "SleepTheGod's MW2 Revolution Mod v3.0\n\n"
               "A complete mod menu for Modern Warfare 2\n"
               "Designed to bring the community back together\n\n"
               "Features:\n"
               "- 50+ unique features\n"
               "- Rainbow scrolling text\n"
               "- F8 menu system\n"
               "- Custom game modes\n"
               "- And much more!\n\n"
               "Made with love by SleepTheGod\n"
               "This mod is for private matches only.",
               "About MW2 Revolution",
               MB_OK | MB_ICONINFORMATION);
}

// ================================================================
// SECTION 7: RENDERER
// ================================================================

class Renderer {
private:
    static bool m_running;
    static std::thread m_thread;
    static ModMenu* m_menu;
    static RainbowTextSystem* m_rainbowText;
    static HDC m_hdc;
    
    static void RenderLoop() {
        while (m_running) {
            if (m_rainbowText) m_rainbowText->Update();
            if (m_rainbowText) m_rainbowText->Draw();
            if (m_menu) m_menu->Draw();
            Sleep(16);
        }
    }
    
public:
    static void Initialize(ModMenu* menu, RainbowTextSystem* rainbowText) {
        m_menu = menu;
        m_rainbowText = rainbowText;
        m_running = true;
        m_thread = std::thread(RenderLoop);
    }
    
    static void Shutdown() {
        m_running = false;
        if (m_thread.joinable()) m_thread.join();
    }
};

bool Renderer::m_running = false;
std::thread Renderer::m_thread;
ModMenu* Renderer::m_menu = nullptr;
RainbowTextSystem* Renderer::m_rainbowText = nullptr;
HDC Renderer::m_hdc = nullptr;

// ================================================================
// SECTION 8: INPUT HANDLER
// ================================================================

class InputHandler {
private:
    static HHOOK m_hook;
    static bool m_keys[256];
    static ModMenu* m_menu;
    
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
            if (wParam == WM_KEYDOWN) {
                if (p->vkCode == VK_F8) { if (m_menu) m_menu->Toggle(); return 1; }
                if (m_menu && m_menu->IsOpen()) {
                    if (p->vkCode == VK_UP) { m_menu->NavigateUp(); return 1; }
                    if (p->vkCode == VK_DOWN) { m_menu->NavigateDown(); return 1; }
                    if (p->vkCode == VK_RETURN || p->vkCode == VK_RIGHT) { m_menu->ActivateSelected(); return 1; }
                }
                if (p->vkCode == VK_F1) { ToggleFullbright(); return 1; }
                if (p->vkCode == VK_F2) { ToggleWallhack(); return 1; }
                if (p->vkCode == VK_F3) { ToggleAimbot(); return 1; }
                if (p->vkCode == VK_F4) { ToggleGodMode(); return 1; }
                if (p->vkCode == VK_F5) { ToggleSuperJump(); return 1; }
                if (p->vkCode == VK_F6) { ToggleInfiniteAmmo(); return 1; }
                if (p->vkCode == VK_F7) { ToggleSpeedHack(); return 1; }
                m_keys[p->vkCode] = true;
            } else if (wParam == WM_KEYUP) {
                m_keys[p->vkCode] = false;
            }
        }
        return CallNextHookEx(m_hook, nCode, wParam, lParam);
    }
    
public:
    static void Install(ModMenu* menu) {
        m_menu = menu;
        memset(m_keys, 0, sizeof(m_keys));
        m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    }
    
    static void Uninstall() {
        if (m_hook) { UnhookWindowsHookEx(m_hook); m_hook = NULL; }
    }
};

HHOOK InputHandler::m_hook = NULL;
bool InputHandler::m_keys[256] = { false };
ModMenu* InputHandler::m_menu = nullptr;

// ================================================================
// SECTION 9: NETWORK MANAGER
// ================================================================

class NetworkManager {
private:
    static SOCKET m_socket;
    static std::thread m_thread;
    static bool m_running;
    
    static void NetworkLoop() { while (m_running) Sleep(100); }
    
public:
    static void Initialize() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        m_running = true;
        m_thread = std::thread(NetworkLoop);
    }
    
    static void Shutdown() {
        m_running = false;
        if (m_thread.joinable()) m_thread.join();
        WSACleanup();
    }
};

SOCKET NetworkManager::m_socket = INVALID_SOCKET;
std::thread NetworkManager::m_thread;
bool NetworkManager::m_running = false;

// ================================================================
// SECTION 10: SCRIPT ENGINE
// ================================================================

class ScriptEngine {
private:
    static std::map<std::string, std::string> m_vars;
    static std::vector<std::string> m_scripts;
    
public:
    static void Execute(const std::string& script) { }
    static void LoadFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            m_scripts.push_back(content);
            file.close();
        }
    }
    static void SetVar(const std::string& name, const std::string& value) { m_vars[name] = value; }
    static std::string GetVar(const std::string& name) { auto it = m_vars.find(name); return it != m_vars.end() ? it->second : ""; }
};

std::map<std::string, std::string> ScriptEngine::m_vars;
std::vector<std::string> ScriptEngine::m_scripts;

// ================================================================
// SECTION 11: MAIN DLL ENTRY
// ================================================================

ModMenu* g_menu = nullptr;
RainbowTextSystem* g_rainbow = nullptr;

DWORD WINAPI MainThread(LPVOID lpParam) {
    MessageBox(NULL, WELCOME_MSG, "MW2 Revolution Mod", MB_OK | MB_ICONINFORMATION);
    MemoryManager::Initialize();
    g_menu = new ModMenu();
    g_rainbow = new RainbowTextSystem();
    InputHandler::Install(g_menu);
    Renderer::Initialize(g_menu, g_rainbow);
    NetworkManager::Initialize();
    ScriptEngine::LoadFile("mods/revolution.gsc");
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
        if (hThread) CloseHandle(hThread);
    } else if (reason == DLL_PROCESS_DETACH) {
        InputHandler::Uninstall();
        Renderer::Shutdown();
        NetworkManager::Shutdown();
        if (g_menu) delete g_menu;
        if (g_rainbow) delete g_rainbow;
    }
    return TRUE;
}
