#pragma once

#include "structures.hpp"

namespace GameData
{
    struct UiContext
    {
        int   contentIndex;                             // 0x00
        float bias;                                     // 0x04
        int   realTime;                                 // 0x08
        int   frameTime;                                // 0x0C
        float cursorPos[2];                             // 0x10
        int   isCursorVisible;                          // 0x18
        int   screenDimensions[2];                      // 0x1C
        float screenAspect;                             // 0x24
        float fps;                                      // 0x28
    }; // Size = 0x2C

    struct Font_s
    {
        const char      *fontName;                      // 0x00
        int              pixelHeight;                   // 0x04
        int              glyphCount;                    // 0x08
        struct Material *material;                      // 0x0C
        struct Material *glowMaterial;                  // 0x10
        struct Glyph    *glyphs;                        // 0x14
    }; // Size = 0x18

    struct KeyState
    {
        int         down;                               // 0x00
        int         repeats;                            // 0x04
        const char *binding;                            // 0x08
        const char *binding2;                           // 0x0C
    }; // Size = 0x10

    extern Font_s *(__cdecl *R_RegisterFont)(const char *font, int imageTrac);
    extern void *(__cdecl *Material_RegisterHandle)(const char *materialName,
        int imageTrac);
    extern void (__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
        float width, float height, float angle, const float *color, void *material);
    extern void (__cdecl *CL_DrawTextPhysicalInternal)(const char *text, int maxChars,
        void *font, float x, float y, float xScale, float yScale, float rotation,
        int style);
    extern int (__cdecl *UI_TextWidthInternal)(const char *text, int maxChars,
        void *font, float scale);
    extern void (__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length);

    Font_s *UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum);
    float UI_TextWidth(const char *text, int maxChars,
        Font_s *font, float scale);
    float UI_TextHeight(Font_s *font, float scale);
    void UI_FillRect(ScreenPlacement *scrPlace, float x, float y, float width,
        float height, int horzAlign, int veryAlign, const float *color);
    void UI_DrawRect(ScreenPlacement *scrPlace, float x, float y, float width,
        float height, int horzAlign, int vertAlign, float size, const float *color);
    void UI_DrawText(ScreenPlacement *scrPlace, const char *text,
        int maxChars, void *font, float x, float y, float scale,
        float angle, const float *color, int style);
    void CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
        float width, float height, float angle, const float *color, void *material);
    float R_NormalizedTextScale(Font_s *font, float scale);
    void CL_DrawTextPhysical(const char *text, int maxChars,
        void *font, float x, float y, float xScale, float yScale,
        float rotation, const float *color, int style);
    void ScrPlace_ApplyRect(ScreenPlacement *scrPlace,
        float *x, float *y, float *w, float *h, int horzAlign, int vertAlign);
    float R_TextWidth(const char *text, int maxChars, Font_s *font);
    float R_TextHeight(Font_s *font);
    void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
        const float *color, struct Material *material, int ratio);
    int Key_StringToKeynum(const char *name);
    bool Key_IsDown(const char *bind);

    extern UiContext       *dc;
    extern ScreenPlacement *scrPlace;
    extern KeyState        *keys;
}

enum ScreenAlignment
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

enum Submenu
{
    MAIN_MENU,
    MISC_MENU,
    AIMBOT_MENU,
    ESP_MENU,
    HUD_MENU
};

enum OptionType
{
    TYPE_SUB   = 0,
    TYPE_INT   = 1,
    TYPE_FLOAT = 2,
    TYPE_BOOL  = 4,
    TYPE_VOID  = 8
};

// The data stored inside each option
struct OptionData
{
    union Data
    {
        bool  boolean;
        int   integer;
        float rational;

        Data()               : integer(0) {}
        Data(bool boolean)   : boolean(boolean) {}
        Data(int integer)    : integer(integer) {}
        Data(float rational) : rational(rational) {}
    };

    Data       data;
    OptionType type;

    OptionData(OptionType type = TYPE_INT);
};

// The basic structure each menu option contains
struct Option
{
    // The name of the option
    std::string name;
    // The callback to handle this option correctly
    std::function<void()> callback;
    // The actual data this variable uses
    OptionData var;

    /**
     * @brief Constructor of an option
     * @param option The option name which is displayed on menu
     * @param type The type of option. Menu displays them differently based
     *              on the type specified.Values defined in enum OptionType
     * @param callback The function to perform to when the option is clicked in
     *                 by the mouse throguh Menu::MonitorMouse
    **/
    Option(const char *option, OptionType type, std::function<void()> &&callback)
        : name(option), var(OptionData(type)), callback(callback) {}
};

struct Menu
{
    // Data structure where all options in menu are stored
    std::vector<std::unordered_map<std::string, Option>> options;
    // Used to synchronize detours in menu
    CRITICAL_SECTION                                     critSection;
    // If menu is open
    bool                                                 open;
    // Used to discern whether a decrease/increase was called in Menu::IntModify
    bool                                                 toggled;
    // Current submenu
    Submenu                                              currentSub;
    // Used to delay the speed of activing an option again
    unsigned int                                         timer;

    /**
     * @brief Initializes menu and inserts all options into it
    **/
    Menu();
    static Menu &Instance() { static Menu menu; return menu; }

    /**
     * @brief Displays the menu and monitors for input
    **/
    void Execute();

    /**
     * @brief Wrapper to insert an option into the menu
     * @param sub The sub menu to insert into. Values defined in enum Submenu
     * @param option The option name which is displayed on menu
     * @param type The type of option. Menu displays them differenntly based
     *             on the type specified. Values defined in enum OptionType
     * @param callback The function to perform to when the option is clicked in
     *                 by the mouse throguh Menu::MonitorMouse
    **/
    void Insert(int sub, const char *option, OptionType type, 
        std::function<void()> &&callback);

    /**
     * @brief Handles where to go when user exits a submenu
    **/
    void CloseSub();

    /**
     * @brief Sets the menu timer which causes a wait to process input
     * @param ms The amount of time in millisecodns to stall the menu
    **/
    void Wait(int ms);

    /**
     * @brief Returns if the menu has waited any duration set by Menu::Wait
     * @return If menu is ready to process input again
    **/
    bool Ready();

    /**
     * @brief Monitors mouse position/clicks and adjusts menu accordingly to it
     * @param opt The current option to selected for input
     * @param optionX The current option x-position
     * @param optionY The current option y-position
     * @param optionW THe current option width
     * @param optionH The current option height
     * @return if the mouse is currently over the option passed in as argument
    **/
    bool MonitorMouse(Option &opt, float optionX, float optionY,
        float optionW, float optionH);

    /**
     * @brief Monitors keyboard input and adjusts menu accordingly to it
    **/
    void MonitorKeys();

    /**
     * @brief Switches the submenu currently being displayed
     * @param sub The submenu to change to from enum Submenu
    **/
    void LoadSub(Submenu sub);

    /**
     * @brief Inverts the value of a varName boolean variable 
     * @param varName The name of the variable being changed
     * @return The result of the variable after the change
    **/
    bool BoolModify(const std::string &varName);

    /**
     * @brief Increments/Decrements the value of a varName integer variable
     * @param varName The name of the variable being changed
     * @return The result of the variable after the change
    **/
    int IntModify(const std::string &varName, OptionType type, int min, int max);

    /**
     * @brief Returns a reference to an option stored in Menu::options
     * @param sub The submenu to get the variable from
     * @param varName The name of the variable to get
    **/
    OptionData &GetOptionData(Submenu sub, const std::string &varName);
};

/**
 * @brief Draws a filled rectangle to the screen
 * @param x X-position on screen to place top-left of rectangle
 * @param y Y-position on screen to place top-left of rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the argument using the Colors::Color structure
 * @param rotation What angle to rotate the rectangle to.
 * @param type Material type to the game to draw the rectangle. I don't really
 *             know how the game handles this at the moment.
**/
void DrawFillRect(float x, float y, float width, float height,
    const Colors::Color &color, float rotation, int type = 0);

/**
 * @brief Draws an empty rectangle to the screen
 * @param x X-position on screen to place top-left of rectangle
 * @param y Y-position on screen to place top-left of rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the argument using the Colors::Color structure
 * @param rotation What angle to rotate the rectangle to.
 * @param type Material type to the game to draw the rectangle. I don't really
 *             know how the game handles this at the moment.
**/
void DrawEmptyRect(float x, float y, float width, float height, float size,
    const Colors::Color &color, int type = 0);

/**
 * @brief Returns the starting x-position of where to draw text in order to 
 *        match arguments specified
 * @param text The string which will be displayed.
 * @param font Which font to use. Specified in the preset Fonts namespace
 * @param scale How much the text will be scaled by
 * @param initX The x-position where the text would like to place the text
 * @param align How to align the text. Specified in the ScreenAlignment enum
 * @param ui If the text being drawn will scale for all resolutions
 * @param bg If the text will be drawn with a background using other functions below
 * @param fOut Optional address to save the address of the Font_s being used
 * @param wOut Optional address to save the width of the text being drawn
 * @param fOut Optional address to save the height of the text being drawn
 * @return The final x-position of where to draw the text matching properties specified
**/
float AlignText(const char *text, const Fonts::Font &font, float scale, float initX,
    ScreenAlignment align, bool ui, bool bg, GameData::Font_s **fOut = nullptr,
    float *wOut = nullptr, float *hOut = nullptr);

/**
 * @brief Renders text which does not scale for all resolutions
 * @param text The string to draw
 * @param x X-position where to draw text
 * @param y Y-position where to draw text
 * @param scale Amount to scale text
 * @param color The color of the argument using the Colors::Color structure
 * @param font Which font to use. Specified in the preset Fonts namespace
 * @param rotation How much in degrees to rotate the text
 * @return The y-position of the bottom of the text.
 *
 * This function only exists because it will be used when drawing names above
 * players or other entities. Scaling for all screen resolutions here does not
 * work well with the UI functions
**/
float RenderGameText(const char *text, float x, float y, float scale,
    const Colors::Color &color, GameData::Font_s *font, float rotation);

/**
 * @brief Renders text which does not scale for all resolutions with a background
 * @param text The string to draw
 * @param x X-position where to draw text
 * @param textW The width of the text
 * @param textH The height of the text
 * @param color The color of the argument using the Colors::Color structure
 * @param font Which font to use. Specified in the preset Fonts namespace
 * @param rotation How much in degrees to rotate the text
 * @return The y-position of the bottom of the text.
 * 
 * This function only exists because it will be used when drawing names above
 * players or other entities. Scaling for all screen resolutions here does not
 * work well with the UI functions
**/
float RenderGameTextWithBackground(const char *text, float x, float y,
    float textW, float textH, const Colors::Color &borderColor, const Colors::Color &textColor,
    GameData::Font_s *font, float scale);

/**
 * @brief Renders text which does scale for all resolutions
 * @param text The string to draw
 * @param x X-position where to draw text
 * @param y Y-position where to draw text
 * @param scale Amount to scale text
 * @param color The color of the argument using the Colors::Color structure
 * @param font Which font to use. Specified in the preset Fonts namespace
 * @param rotation How much in degrees to rotate the text
 * @return The y-position of the bottom of the text.
 *
 * The use of this function is important while drawing the menu or other HUDs
 * as this will look the same among all resolutions available.
**/
float RenderUIText(const std::string &text, float x, float y, float scale,
    const Colors::Color &color, GameData::Font_s *font);

/**
 * @brief Renders text which does scale for all resolutions with a background
 * @param text The string to draw
 * @param x X-position where to draw text
 * @param textW The width of the text
 * @param textH The height of the text
 * @param color The color of the argument using the Colors::Color structure
 * @param font Which font to use. Specified in the preset Fonts namespace
 * @param rotation How much in degrees to rotate the text
 * @return The y-position of the bottom of the text.
 *
 * The use of this function is important while drawing the menu or other HUDs
 * as this will look the same among all resolutions available.
**/
float RenderUITextWithBackground(const char *text, float x, float y,
    float textW, float textH, const Colors::Color &borderColor, 
    const Colors::Color &textColor, GameData::Font_s *font, float scale);

/**
 * @brief Facilitates writing memory to any section in the executable 
 * @param addr The address of the function to write to
 * @param bytes The data to write to this address
 * @param len The amount of bytes to write from the argument bytes
**/
void WriteBytes(DWORD addr, const char *bytes, size_t len);

/**
 * @brief Facilitates reading memory from any section in the executable
 * @param addr The address of the function to read from
 * @param buf Where to store the memory read from the address
 * @param len The amount of bytes to read from the address
**/
void ReadBytes(DWORD addr, char *buf, size_t len); 
