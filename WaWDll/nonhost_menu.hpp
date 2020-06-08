#pragma once

#include "game_entities.hpp"

// Forward declared functions
void RenderESP();

namespace GameData
{
    enum scriptInstance_t;

#pragma pack(push, 1)
    struct rectDef_s
    {
        float x;                                        // 0x00
        float y;                                        // 0x04
        float w;                                        // 0x08
        float h;                                        // 0x0C
        int   horzAlign;                                // 0x10
        int   vertAlign;                                // 0x14
    }; // Size = 0x18

    union DvarValue
    {
        bool             enabled;                       // 0x01
        int              integer;                       // 0x04
        unsigned int     unsignedInt;                   // 0x04
        __int64          integer64;                     // 0x08
        unsigned __int64 unsignedInt64;                 // 0x08
        float            value;                         // 0x04
        float            vector[4];                     // 0x10
        const char      *string;                        // 0x04
        char             color[4];                      // 0x04
    }; // Size = 0x10

    struct dvar_s
    {
        const char  *name;                              // 0x00
        const char  *description;                       // 0x04
        unsigned int flags;                             // 0x08
        char         pad00[0x4];                        // 0x0C
        DvarValue    current;                           // 0x10
        DvarValue    latched;                           // 0x20
        DvarValue    reset;                             // 0x30
        DvarValue    saved;                             // 0x40
        char         pad01[0x8];                        // 0x50
        dvar_s      *next;                              // 0x58
    }; // Size = 0x5C

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
    
    struct ScreenPlacement
    {
        float scaleVirtualToReal[2];                    // 0x00
        float scaleVirtualToFull[2];                    // 0x08
        float scaleRealToVirtual[2];                    // 0x10
        float virtualViewableMin[2];                    // 0x18
        float virtualViewableMax[2];                    // 0x20
        float virtualTweakableMin[2];                   // 0x28
        float virtualTweakableMax[2];                   // 0x30
        float realViewportBase[2];                      // 0x38
        float realViewportSize[2];                      // 0x40
        float realViewportMid[2];                       // 0x48
        float realViewableMin[2];                       // 0x50
        float realViewableMax[2];                       // 0x58
        float realTweakableMin[2];                      // 0x60
        float realTweakableMax[2];                      // 0x68
        float subScreen[2];                             // 0x70
    }; // Size = 0x78
#pragma pack(pop)

    enum
    {
        InitializeCriticalSection_a = 0x7EB148,
        EnterCriticalSection_a      = 0x7EB138,
        LeaveCriticalSection_a      = 0x7EB134,
        R_RegisterFont_a            = 0x6E8D84,
        Material_RegisterHandle_a   = 0x6E9C00,
        CG_DrawRotatedPic_a         = 0x43E570,
        Cmd_ExecuteSingleCommmand_a = 0x594ED0,
        Dvar_FindVar_a              = 0x5EDE30,
        CL_DrawTextPhysical_a       = 0x6F5F10,
        UI_TextWidth_a              = 0x5B5EC0,
        CG_GameMessage_a            = 0x661910,
        UI_GetFontHandle_a          = 0x5B6400,
        UI_DrawText_a               = 0x5B5FB0,
        SL_FindString_a             = 0x68DA90,
        CG_DrawRotatedPicPhysical_a = 0x43E3C0,
        R_TextWidth_a               = 0x6E8DA0,
        DrawSketchPicGun_a          = 0x42CC30,
        Key_StringToKeynum_a        = 0x477540,
        ScrPlace_ApplyRect_a        = 0x47A450,
        UI_DrawRect_a               = 0x5B5BD0,
        UI_FillRect_a               = 0x5B08E0,
        MessageBoxA_a               = 0x7EB33C,
        timeGetTime_a               = 0x7EB39C,
        Cbuf_AddText_a              = 0x594200,
        Com_Error_a                 = 0x59AC50,
        va_a                        = 0x5F6D80,
        Menu_PaintAll_a             = 0x5CA9A2,
        TopLevelExceptionFilter_a   = 0x5FF510,
        Menu_HandleMouseMove_a      = 0x5C9B10,
        CL_WritePacket_a            = 0x63EA80,
        CL_KeyEvent_a               = 0x4780F0,
        CL_MouseMove_a              = 0x63DE70,
        IN_MouseEvent_a             = 0x5FA5F0,
        Com_Printf_a                = 0x59A2C0,
    };

    extern int             *cl_connectionState;
    extern ScreenPlacement *scrPlace;
    extern UiContext       *uiDC;
    extern KeyState        *keys;
    extern HWND            *hwnd;

    // Game's imported library
    extern int (__stdcall *MessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
    extern DWORD (__stdcall *timeGetTime)();
    extern void (__stdcall *InitializeCriticalSection)(LPCRITICAL_SECTION lpCriticalSection);
    extern void (__stdcall *EnterCriticalSection)(LPCRITICAL_SECTION lpCriticalSection);
    extern void (__stdcall *LeaveCriticalSection)(LPCRITICAL_SECTION lpCriticalSection);

    // Command
    extern void (__cdecl *Cmd_ExecuteSingleCommand)(int localClientNum,
        int controllerIndex, const char *text);

    void __usercall Cbuf_AddText(const char *cmd);

    // Dvars 
    extern dvar_s *(__cdecl *Dvar_FindVar)(const char *dvarName);

    // Rendering functions
    extern Font_s *(__cdecl *R_RegisterFont)(const char *font, int imageTrac);
    extern void *(__cdecl *Material_RegisterHandle)(const char *materialName,
        int imageTrac);
    extern void (__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
        float width, float height, float angle, const float *color, void *material);
    extern void (__cdecl *R_AddCmdDrawStretchPicInternal)(const char *text, int maxChars,
        void *font, float x, float y, float xScale, float yScale, float rotation,
        int style);
    extern int (__cdecl *UI_TextWidthInternal)(const char *text, int maxChars,
        void *font, float scale);
    extern void (__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length);

    Font_s *__usercall UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum);
    float __usercall UI_TextWidth(const char *text, int maxChars,
        Font_s *font, float scale);
    float UI_TextHeight(Font_s *font, float scale);
    void __usercall UI_DrawRect(ScreenPlacement *scrPlace, float x, float y, float width,
        float height, int horzAlign, int vertAlign, float thickness, const float *color);
    void __usercall UI_FillRect(ScreenPlacement *scrPlace, float x, float y, float width,
        float height, int horzAlign, int vertAlign, const float *color);
    void __usercall UI_DrawText(ScreenPlacement *scrPlace, const char *text,
        int maxChars, void *font, float x, float y, float scale,
        float angle, const float *color, int style);
    void __usercall R_AddCmdDrawStretchPic(const char *text, int maxChars,
        void *font, float x, float y, float xScale, float yScale,
        float rotation, const float *color, int style);
    void __usercall ScrPlace_ApplyRect(ScreenPlacement *scrPlace,
        float *x, float *y, float *w, float *h, int horzAlign, int vertAlign);
    void __usercall CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
        float width, float height, float angle, const float *color, void *material);
    int __usercall Key_StringToKeynum(const char *name);
    bool Key_IsDown(const char *bind);
    float R_NormalizedTextScale(Font_s *font, float scale);
    float R_TextHeight(Font_s *font);
    float __usercall R_TextWidth(const char *text, int maxChars, Font_s *font);
    void __usercall DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
        const float *color, Material *material, int ratio);

    // Game Status
    bool IN_IsForegroundWindow();
    unsigned int Sys_Milliseconds();

    // Miscellaneous
    unsigned short SL_FindString(scriptInstance_t inst, const char *tagname);
    const char *SL_ConvertToString(int stringValue);
    extern void (__cdecl *Com_Error)(int code, const char *fmt, ...);
    extern char *(__cdecl *va)(const char *fmt, ...);

    // Catches game exceptions and prints a stack trace and register values
    extern LONG (__stdcall *TopLevelExceptionFilter)(struct _EXCEPTION_POINTERS *ExceptionInfo);
    void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS *ExceptionInfo);

    // Detours for non-host menu drawing 
    extern void __usercall* Menu_PaintAll;
    void Menu_PaintAllDetourInvoke(UiContext *dc);
    void Menu_PaintAllDetour(UiContext *dc);

    // Disables mouse movement if menu is open
    extern int (__cdecl *Menu_HandleMouseMove)(ScreenPlacement *scrPlace, void *menu);
    int Menu_HandleMouseMoveDetour(GameData::ScreenPlacement *scrPlace, void *item);

    // Disables shooting if aimbot autoshoot is enabled
    extern void (__cdecl *CL_KeyEvent)(int localClientNum, int value,
        int down, unsigned int time);
    void CL_KeyEventDetour(int localClientNum, int key, int down, int time);
    
    // Used to disable restriction of fire rate on semi auto weapons
    extern void __usercall *Cbuf_AddTextHook;
    void Cbuf_AddTextDetourInvoke(const char *text, int localClientNum);    
    bool Cbuf_AddTextDetour(const char *text, int localClientNum);

    // Used to disable mouse clicking when menu is open
    extern void (__cdecl *IN_MouseEvent)(int mstate);
    void IN_MouseEventDetour(int mstate);
    
    // Used to output the games debugging statements into stdout
    extern int ( __cdecl *Com_Printf)(int channel, const char *format, ...);
    int Com_PrintfDetour(int channel, const char *format, ...);
}

enum ScreenAlignment : char
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

enum Submenu : char
{
    MAIN_MENU,
    MISC_MENU,
    AIMBOT_MENU,
    ESP_MENU,
    HUD_MENU
};

enum OptionType : char
{
    TYPE_SUB,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_VOID
};

namespace Colors
{
    struct Color
    {
        float r, g, b, a;

        Color() : r(0.0F), g(0.0f), b(0.0f), a(1.0f) {}
        Color(float r, float g, float b, float a)
            : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

        operator float *() { return (float *)this; }
        operator const float *() const { return (const float *)this; }
    };

    extern Color white;
    extern Color black;
    extern Color red;
    extern Color green;
    extern Color blue;
    extern Color transparentBlack;
}

namespace Fonts
{
    struct Font
    {
        int         index;
        const char *dir;
    };

    extern Font normalFont;
}

// Local storage of the used game dvar structure
extern std::unordered_map<const char *, GameData::dvar_s *> dvars;

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
 * @brief Copys a string into the clipboard
 * @param text The string to copy
 * @return If the text was successfully copied to clipboard return true else false
**/
bool CopyTextToClipboard(const std::string &text);

/**
 * @brief Copys an address in hex into the clipboard
 * @param address The address tto print
 * @return If the text was successfully copied to clipboard return true else false
**/
bool CopyAddressToClipboard(void *address);

/**
 * @brief Outputs an error message and exits the program
 * @return A formatted string with the error code
 *
 * Call when a Win32 API function fails. This will get the error it wrote in a 
 * formatted string and output it using the game's error function
**/
std::string FormatError(DWORD lastError);

/**
 * @brief Inserts a game dvar structure pointer into local dvars map
 * @param dvarName The dvar name to insert
 * @param dvar A pointer to the dvar if we already have it
 * @return If the dvar was found and inserted, return true. else false
**/
bool InsertDvar(const char *dvarName, GameData::dvar_s *dvar = nullptr);

/**
 * @brief if we are currently in game
 * @return If we are in game return true else false
**/
bool InGame();

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