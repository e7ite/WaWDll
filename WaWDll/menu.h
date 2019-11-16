#pragma once

#include "structures.h"

enum ScreenAlignment
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

enum Submenus
{
    MAIN_MENU,
    MISC_MENU,
    AIMBOT_MENU,
    ESP_MENU,
    HUD_MENU,
};

enum OptionType
{
    TYPE_VOID,
    TYPE_SUB,
    TYPE_INT,
    TYPE_BOOL,
    TYPE_ARRAY,
};

union OptionData
{
    bool boolean;
    int integer;
    float rational;
};

struct Option
{
    const char *option;
    OptionData *data;
    OptionType type;
    std::function<void()> callback;

    Option(const char *option, OptionData *data,
        OptionType type, std::function<void()> callback)
        : option(option), data(data), type(type), callback(callback) {}
};

namespace Menu
{
    extern std::vector<std::vector<Option>> options;
    extern int open;
    extern int toggled;
    extern int currentSub;
    extern int timer;

    void Build();
    void Execute();
    void Insert(int sub, const char *option, OptionData *data,
        OptionType type, const std::function<void()> &func);
    void LoadSub(int sub);
    void CloseSub();
    void BoolModify(OptionData *var);
    void IntModify(OptionData *var, OptionType type, int min, int max);
    bool Ready();
    void Wait(int ms);
    bool MonitorMouse(const Option &opt, float optionX, float optionY,
        float optionW, float optionH);
    void MonitorKeys();
};

namespace Variables
{
    extern OptionData enableAimbot;
    extern OptionData aimKey;
    extern OptionData aimType;
    extern OptionData autoAim;
    extern OptionData autoShoot;
    extern OptionData noSpread;
    extern OptionData noRecoil;

    extern OptionData enemyESP;
    extern OptionData friendlyESP;
    
    extern OptionData serverInfo;
    extern OptionData healthBar;

    extern OptionData fov;    
    extern OptionData steadyAim;
    extern OptionData cheatsEnabled;
    extern OptionData godMode;
    extern OptionData infAmmo;
    extern OptionData noFlinch;
}

void DrawFillRect(float x, float y, float width, float height,
    const float *color, float rotation, int type = 0);
void DrawEmptyRect(float x, float y, float width, float height, float size,
    const float *color, int type = 0);
float AlignText(const char *text, const GameData::Font &font, float scale, float initX,
    ScreenAlignment align, bool ui, bool bg, Font_s **fOut = nullptr,
    float *wOut = nullptr, float *hOut = nullptr);
float RenderGameText(const char *text, float x, float y, float scale,
    const float *color, Font_s *font, float rotation);
float RenderGameTextWithBackground(const char *text, float x, float y,
    float textW, float textH, const float *borderColor, const float *textColor,
    Font_s *font, float scale);
float RenderUIText(const char *text, float x, float y, float scale,
    const float *color, Font_s *font);
float RenderUITextWithBackground(const char *text, float x, float y,
    float textW, float textH, const float *borderColor, const float *textColor,
    Font_s *font, float scale);
void WriteBytes(DWORD addr, const char *bytes, size_t len);
void ReadBytes(DWORD addr, char *buf, size_t len); 
