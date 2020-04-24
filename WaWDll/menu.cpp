#include "stdafx.h"
#include "menu.h"

OptionData::OptionData(OptionType type) : type(type)
{
    switch (type)
    {
        case TYPE_INT:
            this->data = Data(0);
        break;
        case TYPE_BOOL:
            this->data = Data(false);
        break;
        case TYPE_FLOAT:
            this->data = Data(0.0f);
        break;
        default:
            this->data = Data(0);
        break;
    }
}

Menu::Menu() :
    open(false), 
    toggled(false),
    currentSub(MAIN_MENU),
    timer(0)
{
    GameData::InitializeCriticalSection(&this->critSection);

    for (int sub = MAIN_MENU; sub <= HUD_MENU; sub++)
        this->options.push_back(std::unordered_map<std::string, Option>());

    // Main menu options
    Insert(MAIN_MENU, "Aimbot Menu", TYPE_SUB, 
        []() 
        { 
            Menu::Instance().LoadSub(AIMBOT_MENU); 
        });
    Insert(MAIN_MENU, "ESP Menu", TYPE_SUB, 
        []() 
        { 
            Menu::Instance().LoadSub(ESP_MENU); 
        });
    Insert(MAIN_MENU, "HUD Menu", TYPE_SUB, 
        []() 
        { 
            Menu::Instance().LoadSub(HUD_MENU); 
        });
    Insert(MAIN_MENU, "Misc Menu", TYPE_SUB, 
        []() 
        { 
            Menu::Instance().LoadSub(MISC_MENU); 
        });

    // ESP menu options
    Insert(ESP_MENU, "Enemy ESP", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("Enemy ESP"); 
        });
    Insert(ESP_MENU, "Friendly ESP", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("Friendly ESP"); 
        });

    // HUD menu options
    Insert(HUD_MENU, "Server Info", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("Server Info"); 
        });

    // Aimbot menu options
    Insert(AIMBOT_MENU, "Enable Aimbot", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("Enable Aimbot"); 
        });
    Insert(AIMBOT_MENU, "Aim Key",  TYPE_INT, 
        []() 
        { 
            Menu::Instance().IntModify("Aim Key", TYPE_INT, 0, 2); 
        });
    Insert(AIMBOT_MENU, "Aim Type",  TYPE_INT, 
        []() 
        { 
            Menu::Instance().IntModify("Aim Type", TYPE_INT, 0, 2); 
        });
    Insert(AIMBOT_MENU, "Auto Aim", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("Auto Aim"); 
        });
    Insert(AIMBOT_MENU, "Auto Shoot", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("Auto Shoot"); 
        });
    Insert(AIMBOT_MENU, "No Recoil", TYPE_BOOL, 
        []() 
        {
            WriteBytes(0x46A87E,
                Menu::Instance().BoolModify("No Recoil") ? "\xEB" : "\x74", 1);
        });
    Insert(AIMBOT_MENU, "No Spread", TYPE_BOOL, 
        []() { Menu::Instance().BoolModify("No Spread"); });

    // Miscellaneous menu options
    Insert(MISC_MENU, "FOV",  TYPE_INT, 
        []() 
        {
            dvars.at("cg_fov")->current.value 
                = static_cast<float>(Menu::Instance().IntModify("FOV", TYPE_INT, 65, 125));
        });
    Insert(MISC_MENU, "Super Steady Aim", TYPE_BOOL, 
        []() 
        { 
            WriteBytes(0x41DB2B,
                Menu::Instance().BoolModify("Super Steady Aim")
                ? "\x90\x90\x90\x90\x90"
                : "\x83\xFF\x02\x75\x15", 5);
        });
    Insert(MISC_MENU, "Enable Cheats", TYPE_BOOL, 
        []() 
        { 
            dvars.at("sv_cheats")->current.enabled 
                = Menu::Instance().BoolModify("Enable Cheats");
        });
    Insert(MISC_MENU, "God Mode", TYPE_VOID, 
        []() 
        { 
            GameData::Cbuf_AddText("god"); 
        });
    Insert(MISC_MENU, "No Clip", TYPE_VOID, 
        []() 
        { 
            GameData::Cbuf_AddText("noclip"); 
        });
    Insert(MISC_MENU, "Give All Weapons", TYPE_VOID, 
        []() 
        { 
            GameData::Cbuf_AddText("give all"); 
        });
    Insert(MISC_MENU, "No Target", TYPE_VOID, 
        []() 
        { 
            GameData::Cbuf_AddText("notarget"); 
        });
    Insert(MISC_MENU, "Infinite Ammo", TYPE_BOOL, 
        []() 
        {
            dvars.at("player_sustainAmmo")->current.enabled 
                = Menu::Instance().BoolModify("Infinite Ammo");
        });
    Insert(MISC_MENU, "No Flinch", TYPE_BOOL, 
        []() 
        { 
            Menu::Instance().BoolModify("No Flinch"); 
        });
}

void Menu::Insert(int sub, const char* option, OptionType type, 
    std::function<void()>&& callback)
{
    this->options.at(sub).insert(
        std::pair<std::string, Option>(
            option, Option(option, type, std::forward<std::function<void()>>(callback))
        )
    );
}

void Menu::LoadSub(Submenu sub)
{
    this->currentSub = sub;
}

void Menu::CloseSub()
{
    switch (this->currentSub)
    {
    case MAIN_MENU:
        this->open = false;
        break;
    case AIMBOT_MENU:
    case ESP_MENU:
    case HUD_MENU:
    case MISC_MENU:
        this->currentSub = MAIN_MENU;
        break;
    default:
        this->open = false;
        break;
    }
}

bool Menu::BoolModify(const std::string& varName)
{
    OptionData& var = this->GetOptionData(this->currentSub, varName);
    return var.data.boolean = !var.data.boolean;
}

int Menu::IntModify(const std::string& varName, OptionType type, int min, int max)
{
    OptionData& var = this->GetOptionData(this->currentSub, varName);

    if (this->toggled)
        var.data.integer++;
    else
        var.data.integer--;

    if (var.data.integer > max)
        return var.data.integer = min;
    if (var.data.integer < min)
        return var.data.integer = max;
}

OptionData& Menu::GetOptionData(Submenu sub, const std::string& key)
{
    return this->options.at(sub).at(key).var;
}

bool Menu::Ready()
{
    return GameData::Sys_Milliseconds() > timer;
}

void Menu::Wait(int ms)
{
    timer = GameData::Sys_Milliseconds() + ms;
}

void Menu::Execute()
{
    const char* title = "WaW Zombies DLL By E7ite";
    float menuCenterX = GameData::dc->screenDimensions[0] / 2
        / GameData::scrPlace->scaleVirtualToFull[0];
    float menuCenterY = GameData::dc->screenDimensions[1] / 2
        / GameData::scrPlace->scaleVirtualToFull[1];

    // Get x position of text aligned with a background and scaled for all resolutions
    float textWidth, textHeight;
    GameData::Font_s* fontPointer;
    float xAligned = AlignText(title, Fonts::normalFont, 0.3f,
        menuCenterX, ALIGN_CENTER, 1, 1, &fontPointer, &textWidth, &textHeight);

    // Get position and dimensions of all border and options
    float borderW = menuCenterX - 20;
    float borderH = textHeight * options[currentSub].size();
    float borderX = menuCenterX - borderW / 2;
    float borderY = menuCenterY - 98;
    float optionX = borderX + 4;
    float optionY = menuCenterY - 100;
    float optionH = UI_TextHeight(fontPointer, 0.3f);

    // Draw the title and the menu base
    optionY += RenderUITextWithBackground(title, xAligned, optionY, textWidth, 
        textHeight, Colors::blue, Colors::white, fontPointer, 0.3f);
    GameData::UI_FillRect(GameData::scrPlace, borderX, borderY, borderW, borderH, 0, 0,
        Colors::transparentBlack);
    GameData::UI_DrawRect(GameData::scrPlace, borderX, borderY, borderW, borderH,
        0, 0, 2, Colors::blue);

    // Draw all the options in the current sub menu
    for (auto& i : options[currentSub])
    {
        Colors::Color color = Colors::white;
        const Option& option = i.second;

        // Adjust options in menu based on mouse position and execute any callbacks
        if (this->MonitorMouse(i.second, borderX, optionY - 2, borderW, optionH + 2))
            color = Colors::blue;

        // Draw the additional visuals for array and boolean options
        switch (option.var.type)
        {
            case TYPE_BOOL:
                GameData::UI_FillRect(GameData::scrPlace, borderX + borderW - 12,
                    optionY - optionH + 2, 8, 8, 0, 0,
                    option.var.data.boolean ? Colors::blue : Colors::transparentBlack);
            break;
            case TYPE_INT:
                std::string data = std::to_string(option.var.data.integer);
                RenderUIText(data.data(),
                    AlignText(data.data(), Fonts::normalFont,
                        0.3f, borderX + borderW - 3, ALIGN_RIGHT, 1, 0),
                    optionY, 0.3f, color, fontPointer);
            break;
        }

        // Draw the text of the menu
        optionY += RenderUIText(i.first, optionX, optionY,
            0.3f, color, fontPointer);
    }
}

bool Menu::MonitorMouse(Option& opt, float optionX, float optionY,
    float optionW, float optionH)
{
    if (GameData::dc->cursorPos[0] > optionX
        && GameData::dc->cursorPos[0] < optionX + optionW
        && GameData::dc->cursorPos[1] > optionY - optionH
        && GameData::dc->cursorPos[1] < optionY)
    {
        if (this->Ready())
        {
            int delay = opt.var.type == TYPE_INT ? 100 : 200;
            if (GetAsyncKeyState(VK_LBUTTON) & 0x10000)
            {
                toggled = true;
                opt.callback();
                toggled = false;
                this->Wait(delay);
            }
            else if (GetAsyncKeyState(VK_RBUTTON) & 0x10000)
            {
                opt.callback();
                this->Wait(delay);
            }
        }
        return true;
    }
    return false;
}

void Menu::MonitorKeys()
{
    if (this->Ready())
    {
        if (GetAsyncKeyState(VK_HOME) && !this->open)
        {
            this->open = true;
            this->currentSub = MAIN_MENU;
            this->Wait(200);
        }
        if (GetAsyncKeyState(VK_BACK) & 0x10000)
        {
            this->CloseSub();
            this->Wait(200);
        }
    }
}

void RenderShader(float x, float y, float width, float height, float angle,
    const float* color, const char* material, int type)
{
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace, x, y, width, height,
        angle, color, GameData::Material_RegisterHandle(material, type));
}

void DrawFillRect(float x, float y, float width, float height,
    const Colors::Color& color, float rotation, int type)
{
    RenderShader(x, y, width, height, rotation, color, "white", type);
}

void DrawEmptyRect(float x, float y, float width, float height, float size,
    const Colors::Color& color, int type)
{
    RenderShader(x, y, width, size, 0, color, "white", type); //up
    RenderShader(x, y + height, width + (size - 1), size, 0,  //down
        color, "white", type);
    RenderShader(x, y, size, height + (size - 1), 0, color,   //left
        "white", type);
    RenderShader(x + width, y, size, height + size, 0, color, //right
        "white", type);
}

float AlignText(const char* text, const Fonts::Font& font, float scale, float initX,
    ScreenAlignment align, bool ui, bool bg, GameData::Font_s** fOut,
    float* wOut, float* hOut)
{
    GameData::Font_s* fontPointer;
    float width, height;
    float xAligned;

    // Align text based on if callee requested text alignment for background (bool bg)
    // and wants text aligned using routines for all screen resolutions (bool ui)
    if (bg && ui)
    {
        fontPointer = GameData::UI_GetFontHandle(GameData::scrPlace, font.index);
        width = UI_TextWidth(text, INT_MAX, fontPointer, scale) + 12.0f;
        height = UI_TextHeight(fontPointer, scale) + 2.0f;
    }
    else if (!bg && ui)
    {
        fontPointer = GameData::UI_GetFontHandle(GameData::scrPlace, font.index);
        width = UI_TextWidth(text, INT_MAX, fontPointer, scale);
        height = UI_TextHeight(fontPointer, scale) + 2.0f;
    }
    else if (bg && !ui)
    {
        fontPointer = GameData::R_RegisterFont(font.dir, 0);
        width = R_TextWidth(text, INT_MAX, fontPointer) * scale + 14.0f;
        height = R_TextHeight(fontPointer) * scale + 2.0f;
    }
    else
    {
        fontPointer = GameData::R_RegisterFont(font.dir, 0);
        width = R_TextWidth(text, INT_MAX, fontPointer) * scale;
        height = R_TextHeight(fontPointer) * scale + 2.0f;
    }

    switch (align)
    {
        case ALIGN_LEFT:
            xAligned = initX;
        break;
        case ALIGN_CENTER:
            xAligned = initX - width / 2;
        break;
        case ALIGN_RIGHT:
            xAligned = initX - width;
        break;
        default:
            xAligned = initX;
        break;
    }

    if (wOut)
        *wOut = width;
    if (hOut)
        *hOut = height;
    if (fOut)
        *fOut = fontPointer;
    return xAligned;
}

float RenderGameText(const char* text, float x, float y, float scale,
    const Colors::Color& color, GameData::Font_s* font, float rotation)
{
    GameData::CL_DrawTextPhysical(text, INT_MAX, font, x, y,
        scale, scale, rotation, color, 0);

    return GameData::R_TextHeight(font) * scale;
}

float RenderGameTextWithBackground(const char* text, float x, float y,
    float textW, float textH, const Colors::Color& borderColor, 
    const Colors::Color& textColor, GameData::Font_s* font, float scale)
{
    DrawFillRect(x, y - textH, textW - 2, textH + 2,
        Colors::transparentBlack, 0);
    DrawEmptyRect(x, y - textH, textW - 2, textH + 2, 2,
        borderColor);
    RenderGameText(text, x + 6, y, scale, textColor, font, 0);

    return textH + 2;
}

float RenderUIText(const std::string& text, float x, float y, float scale,
    const Colors::Color& color, GameData::Font_s* font)
{
    UI_DrawText(GameData::scrPlace, text.data(), INT_MAX, font, x, y,
        scale, 0.0f, color, 0);

    return UI_TextHeight(font, scale) + 2.0f;
}

float RenderUITextWithBackground(const char* text, float x, float y,
    float textW, float textH, const Colors::Color& borderColor, 
    const Colors::Color& textColor, GameData::Font_s* font, float scale)
{
    GameData::UI_FillRect(GameData::scrPlace, x, y - textH, textW - 2, textH + 2,
        0, 0, Colors::transparentBlack);
    GameData::UI_DrawRect(GameData::scrPlace, x, y - textH, textW - 2, textH + 2, 0, 0, 2, borderColor);
    RenderUIText(text, x + 6, y, scale, textColor, font);

    return textH + 2;
}

void WriteBytes(DWORD addr, const char* bytes, size_t len)
{
    DWORD curProtection;
    HANDLE curProcess = GetCurrentProcess();
    
    // Make virtual page address have read/write/exec privledges
    // and save old privledges to temporary
    VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

    WriteProcessMemory(curProcess, (LPVOID)addr, bytes, len, 0);
    
    // Restore old privledges from temporary to virtual page address
    VirtualProtect((LPVOID)addr, len, curProtection, 0);

    // Run this to ensure instruction cache doesn't contain the instruction
    // from the address you just modified, therefore running the same instruction
    // you just removed
    FlushInstructionCache(curProcess, (void*)addr, len);
}

void ReadBytes(DWORD addr, char* buf, size_t len)
{
    // Make virtual page address have read/write/exec privledges
    // and save old privledges to temporary
    DWORD curProtection;
    VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

    ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, buf, len, 0);

    // Restore old privledges from temporary to virtual page address
    VirtualProtect((LPVOID)addr, len, curProtection, 0);
} 
