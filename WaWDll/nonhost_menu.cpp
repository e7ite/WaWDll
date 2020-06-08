#include "stdafx.hpp"
#include "nonhost_menu.hpp"

namespace GameData
{
    int             *cl_connectionState = (int *)0x305842C;
    UiContext       *uiDC               = (UiContext *)0x208E920;
    ScreenPlacement *scrPlace           = (ScreenPlacement *)0x957360;
    KeyState        *keys               = (KeyState *)0x951C44;
    HWND            *hwnd               = (HWND *)0x22C1BE4;

    int (__stdcall *MessageBoxA)(HWND hWnd, LPCSTR lpText,
        LPCSTR lpCaption, UINT uType)
        = *(int (__stdcall **)(HWND, LPCSTR, LPCSTR, UINT))MessageBoxA_a;
    DWORD (__stdcall *timeGetTime)() = *(DWORD (__stdcall **)())timeGetTime_a;
    void (__stdcall *InitializeCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void (__stdcall **)(LPCRITICAL_SECTION))InitializeCriticalSection_a;
    void (__stdcall *EnterCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void (__stdcall **)(LPCRITICAL_SECTION))EnterCriticalSection_a;
    void (__stdcall *LeaveCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void (__stdcall **)(LPCRITICAL_SECTION))LeaveCriticalSection_a;
    dvar_s *(__cdecl *Dvar_FindVar)(const char *dvarName)
        = (dvar_s *(*)(const char *))Dvar_FindVar_a;
    Font_s *(__cdecl *R_RegisterFont)(const char *font, int imageTrac)
        = (Font_s *(*)(const char *, int))R_RegisterFont_a;
    void *(__cdecl *Material_RegisterHandle)(const char *materialName, int imageTrac)
        = (void *(*)(const char *, int))Material_RegisterHandle_a;
    void (__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
        float width, float height, float angle, const float *color, void *material)
        = (void (*)(ScreenPlacement *, float, float,
            float, float, float, const float*, void*))CG_DrawRotatedPic_a;
    void (__cdecl *R_AddCmdDrawStretchPicInternal)(const char *text, int maxChars,
        void *font, float x, float y, float xScale, float yScale, float rotation, int style)
        = (void (__cdecl *)(const char *, int, void *, float, float, 
            float, float, float, int))CL_DrawTextPhysical_a;
    int (__cdecl *UI_TextWidthInternal)(const char *text, int maxChars,
        void *font, float scale)
        = (int (*)(const char *, int, void *, float))UI_TextWidth_a;
    char *(__cdecl *va)(const char *fmt, ...) 
        = (char *(__cdecl *)(const char *, ...))va_a;
    void (__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length)
        = (void (__cdecl *)(int, const char *, int))CG_GameMessage_a;
    void (__cdecl *Com_Error)(int code, const char *fmt, ...)
        = (void (__cdecl *)(int, const char *, ...))Com_Error_a;

    void __usercall Cbuf_AddText(const char *cmd)
    {
        DWORD addr = Cbuf_AddText_a;
        __asm
        {
            mov         ecx, 0
            mov         eax, cmd
            call        addr
        }
    }

    unsigned int Sys_Milliseconds()
    {
        return GameData::timeGetTime() - *(int *)0x22BEC34;
    }
    
    bool IN_IsForegroundWindow()
    {
        return *(bool *)(0x229A0D4);
    }

    const char *SL_ConvertToString(int stringValue)
    {
        std::cout << "newjdias\n";
        const char **gScrMemTreePub = (const char **)0x3702390;
        return *gScrMemTreePub + ((stringValue * 2 + stringValue) * 4) + 4;
    }

    unsigned short __usercall SL_FindString(scriptInstance_t inst, const char *tagname)
    {
        unsigned short result;
        unsigned int len = strlen(tagname) + 1;
        DWORD addr = SL_FindString_a;
        __asm
        {
            mov         eax, inst
            push        len
            push        tagname
            call        addr
            add         esp, 8
            mov         result, ax
        }
        return result;
    }

    Font_s *__usercall UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum)
    {
        DWORD addr = UI_GetFontHandle_a;
        Font_s *result;
        __asm
        {
            mov         ecx, scrPlace
            mov         eax, fontEnum
            fldz
            sub         esp, 4
            fstp        [esp]
            call        addr
            add         esp, 4
            mov         result, eax
        }
        return result;
    }

    float __usercall UI_TextWidth(const char *text, int maxChars,
        Font_s *font, float scale)
    {
        float result;
        __asm
        {
            push        maxChars
            push        text
            mov         eax, font
            movss       xmm0, scale
            call        UI_TextWidthInternal
            add         esp, 8
            cvtsi2ss    xmm0, eax
            movss       result, xmm0
        }
        return result;
    }

    float UI_TextHeight(Font_s *font, float scale)
    {
        return static_cast<float>(
            floor(font->pixelHeight * R_NormalizedTextScale(font, scale) + 0.5));
    }

    void __usercall UI_DrawRect(ScreenPlacement *scrPlace, float x, float y, float width,
        float height, int horzAlign, int vertAlign, float thickness, const float *color)
    {
        DWORD addr = UI_DrawRect_a;
        __asm
        {
            mov         eax, color
            mov         ecx, horzAlign
            sub         esp, 4
            fld         thickness
            fstp        dword ptr [esp]
            push        vertAlign
            sub         esp, 10h
            fld         height
            fstp        dword ptr [esp + 0Ch]
            fld         width
            fstp        dword ptr [esp + 8]
            fld         y
            fstp        dword ptr [esp + 4]
            fld         x
            fstp        dword ptr [esp]
            push        scrPlace
            call        addr
            add         esp, 1Ch
        }
    }

    void __usercall UI_FillRect(ScreenPlacement *scrPlace, float x, float y, float width,
        float height, int horzAlign, int vertAlign, const float *color)
    {
        DWORD addr = UI_FillRect_a;
        __asm
        {
            sub         esp, 1Ch
            mov         ecx, scrPlace
            mov         edx, color
            mov         dword ptr [esp + 18h], edx
            mov         edx, vertAlign
            mov         dword ptr [esp + 14h], edx
            mov         edx, horzAlign
            mov         dword ptr [esp + 10h], edx
            fld         height
            fstp        dword ptr [esp + 0Ch]
            fld         width
            fstp        dword ptr [esp + 8]
            fld         y
            fstp        dword ptr [esp + 4]
            fld         x
            fstp        dword ptr [esp]
            call        addr
            add         esp, 1Ch
        }
    }

    void __usercall UI_DrawText(ScreenPlacement *scrPlace, const char *text,
        int maxChars, void *font, float x, float y, float scale,
        float angle, const float *color, int style)
    {
        DWORD addr = UI_DrawText_a;
        __asm
        {
            mov         eax, 0
            mov         ecx, 0
            push        style
            push        color
            push        scale
            push        y
            push        x
            push        font
            push        maxChars
            push        text
            push        scrPlace
            call        addr
            add         esp, 24h
        }
    }

    void __usercall R_AddCmdDrawStretchPic(const char *text, int maxChars,
        void *font, float x, float y, float xScale, float yScale,
        float rotation, const float *color, int style)
    {
        __asm
        {
            push        style
            sub         esp, 14h
            fld         rotation
            fstp        [esp + 10h]
            fld         yScale
            fstp        [esp + 0Ch]
            fld         xScale
            fstp        [esp + 08h]
            fld         y
            fstp        [esp + 04h]
            fld         x
            fstp        [esp]
            push        font
            push        maxChars
            push        text
            mov         ecx, color
            call        R_AddCmdDrawStretchPicInternal
            add         esp, 24h
        }
    }

    void __usercall ScrPlace_ApplyRect(ScreenPlacement *scrPlace,
        float *x, float *y, float *w, float *h, int horzAlign, int vertAlign)
    {
        DWORD addr = ScrPlace_ApplyRect_a;
        __asm
        {
            mov         edx, x
            mov         ecx, w
            mov         edi, y
            mov         esi, h
            push        vertAlign
            push        horzAlign
            push        scrPlace
            call        addr
            add         esp, 0Ch
        }
    }

    void __usercall CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
        float width, float height, float angle, const float *color, void *material)
    {
        DWORD addr = CG_DrawRotatedPicPhysical_a;
        __asm
        {
            push        material
            push        color
            sub         esp, 10h
            fld         x
            fstp        [esp]
            fld         y
            fstp        [esp + 4]
            fld         width
            fstp        [esp + 8]
            fld         height
            fstp        [esp + 0Ch]
            movss       xmm0, angle
            mov         edx, scrPlace
            call        addr
            add         esp, 18h
        }
    }

    int __usercall Key_StringToKeynum(const char *name)
    {
        DWORD result;
        DWORD addr = Key_StringToKeynum_a;
        __asm
        {
            mov         edi, name
            call        addr
            mov         result, eax
        }
        return result;
    }

    bool Key_IsDown(const char *bind)
    {
        for (__int16 i = 0; i < 256; i++)
            if (keys[i].binding)
                if (!strcmp(keys[i].binding, bind))
                    if (keys[i].down)
                        return true;
        return false;
    }

    float R_NormalizedTextScale(Font_s *font, float scale)
    {
        return scale * 48.0f / (float)font->pixelHeight;
    }

    float R_TextHeight(Font_s *font)
    {
        return static_cast<float>(font->pixelHeight);
    }

    float __usercall R_TextWidth(const char *text, int maxChars, Font_s *font)
    {
        DWORD addr = R_TextWidth_a;
        float result;
        __asm
        {
            mov         eax, text
            push        font
            push        maxChars
            call        addr
            add         esp, 8
            cvtsi2ss    xmm0, eax
            movss       result, xmm0
        }
        return result;
    }

    void __usercall DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
        const float *color, Material *material, int ratio)
    {
        DWORD addr = DrawSketchPicGun_a;
        __asm
        {
            mov         eax, rect
            push        ratio
            push        material
            push        color
            push        scrPlace
            call        addr
            add         esp, 10h
        }
    }

    LONG (__stdcall *TopLevelExceptionFilter)(struct _EXCEPTION_POINTERS *ExceptionInfo)
        = (LONG (__stdcall *)(_EXCEPTION_POINTERS *))TopLevelExceptionFilter_a;
    void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS *ExceptionInfo)
    {
        PDWORD_PTR currESP =
            (PDWORD_PTR)ExceptionInfo->ContextRecord->Esp;
        PDWORD_PTR currEIP =
            (PDWORD_PTR)ExceptionInfo->ContextRecord->Eip;

        printf("EAX: %p\n", (void *)ExceptionInfo->ContextRecord->Eax);
        printf("EBX: %p\n", (void *)ExceptionInfo->ContextRecord->Ebx);
        printf("ECX: %p\n", (void *)ExceptionInfo->ContextRecord->Ecx);
        printf("EDX: %p\n", (void *)ExceptionInfo->ContextRecord->Edx);
        printf("EDI: %p\n", (void *)ExceptionInfo->ContextRecord->Edi);
        printf("ESI: %p\n", (void *)ExceptionInfo->ContextRecord->Esi);
        printf("EBP: %p\n", (void *)ExceptionInfo->ContextRecord->Ebp);
        printf("ESP: %p\n", (void *)currESP);
        printf("EIP: %p\n", (void *)currEIP);

        printf("\nSTACK VIEW:\n");
        for (int i = 0; i < 8; i++)
        {
            if (i)
                ++currESP;
            printf("%p: %p ", (void *)currESP, (void *)*currESP);
            for (int j = 0; j < 3; j++)
                printf("%p ", (void *)*(++currESP));
            printf("\n");
        }

        if (CopyTextToClipboard(GameData::va("%p", currEIP)))
            printf("\nInstruction pointer copied to clipboard\n");
        else
            printf("\nProblem copying instruction pointer to clipboard\n");
    }

    void __usercall *Menu_PaintAll = (void __usercall *)Menu_PaintAll_a;
    void __declspec(naked) Menu_PaintAllDetourInvoke(UiContext *dc)
    {
        __asm
        {
            push        esi
            call        Menu_PaintAllDetour
            add         esp, 4
            pop         edi
            pop         ebp
            pop         ebx
            add         esp, 410h
            ret
        }
    }
    void Menu_PaintAllDetour(UiContext *dc)
    {
        Menu &menu = Menu::Instance();
        EnterCriticalSection(&menu.critSection);

        if (IN_IsForegroundWindow())
            menu.MonitorKeys();

        if (menu.open)
            menu.Execute();

        RenderESP();

        /*
            To call a GSC method, you must reverse yyparse() in ScriptParse()
            as this function fills the sval_u nodes which consitiute the arguments.
            In VM_Execute(), the game uses the gScrCompilePub[inst].func_table to 
            get the functions to call. This table is populated during EmitCall during
            the call to ScriptCompile().

            TODO figure out how to access all the GSC functions using in VM_Execute()
            Steps I currently know:
            1. It is converted func_name from sval_u into a string using SL_ConvertToString
            2. it uses GetFunction() to get the function pointer
            3. It uses AddFunction to add it to gScrCompilePub[inst].func_table 
            4. The function is called somehow in VM_Execute. Don't know yet
        */
       
        LeaveCriticalSection(&menu.critSection);
    }
    
    int (__cdecl *Menu_HandleMouseMove)(ScreenPlacement *scrPlace, void *menu)
        = (int (__cdecl *)(ScreenPlacement *, void *))Menu_HandleMouseMove_a;
    int Menu_HandleMouseMoveDetour(ScreenPlacement *scrPlace, void *item)
    {
        Menu &menu = Menu::Instance();
        //GameData::EnterCriticalSection(&menu.critSection);

        if (!menu.open)
        { 
           // GameData::LeaveCriticalSection(&menu.critSection);
            return GameData::Menu_HandleMouseMove(scrPlace, item);
        }
    
        //GameData::LeaveCriticalSection(&menu.critSection);
        return 0;
    }

    void (__cdecl *CL_KeyEvent)(int localClientNum, int value, int down,
        unsigned int time) = (void(*)(int, int, int, unsigned int))CL_KeyEvent_a;
    void CL_KeyEventDetour(int localClientNum, int key, int down, int time)
    {
        Menu &menu = Menu::Instance();
        GameData::EnterCriticalSection(&menu.critSection);

        OptionData &aimKey = menu.GetOptionData(AIMBOT_MENU, "Aim Key");
        OptionData &autoShoot = menu.GetOptionData(AIMBOT_MENU, "Auto Shoot");

        if (InGame() && GameData::keys[key].binding
            && !*(int *)0x208E938
            && (aimKey.data.integer != 1 || !strcmp(GameData::keys[key].binding, "+attack"))
            && autoShoot.data.boolean)
            return;

        GameData::LeaveCriticalSection(&menu.critSection);
        return GameData::CL_KeyEvent(localClientNum, key, down, time);
    }

    void __usercall *Cbuf_AddTextHook = (void __usercall *)Cbuf_AddText_a;
    void __declspec(naked) Cbuf_AddTextDetourInvoke(const char *text,
        int localClientNum)
    {
        __asm
        {
            push        eax
            push        ecx
            push        ecx
            push        eax
            call        Cbuf_AddTextDetour
            add         esp, 8h
            cmp         al, 0
            pop         ecx
            pop         eax
            jz          LABEL_1
            push        ebp
            push        esi
            push        edi
            push        22990F8h
            push        594208h
            ret
    LABEL_1:
            pop         edx
            jmp         edx
        }
    }
    bool Cbuf_AddTextDetour(const char *text, int localClientNum)
    {
        return true;
    }

    void (__cdecl *IN_MouseEvent)(int mstate) = (void (__cdecl *)(int))IN_MouseEvent_a;
    void IN_MouseEventDetour(int mstate)
    {
        Menu &menu = Menu::Instance();
        //GameData::EnterCriticalSection(&menu.critSection);

        if (!menu.open)
        {
            GameData::LeaveCriticalSection(&menu.critSection);
            return GameData::IN_MouseEvent(mstate);
        }

       // GameData::LeaveCriticalSection(&menu.critSection);
    }

    int (__cdecl *Com_Printf)(int channel, const char *format, ...)
        = (int (__cdecl *)(int, const char *, ...))Com_Printf_a;
    int Com_PrintfDetour(int channel, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);

        char printBuffer[1024];
        vsnprintf(printBuffer, 1024, format, ap);
        printf("%s\n", printBuffer);

        va_end(ap);
        return 0;
    }
}

std::unordered_map<const char *, GameData::dvar_s *> dvars;

Fonts::Font Fonts::normalFont = { 1, "fonts/normalFont" };

Colors::Color Colors::white            = { 255.0f, 255.0f, 255.0f, 255.0f };
Colors::Color Colors::black            = {   0.0f,   0.0f,   0.0f, 255.0f };
Colors::Color Colors::red              = { 255.0f,   0.0f,   0.0f, 255.0f };
Colors::Color Colors::green            = {   0.0f, 255.0f,   0.0f, 255.0f };
Colors::Color Colors::blue             = {   0.0f,   0.0f, 255.0f, 255.0f };
Colors::Color Colors::transparentBlack = {   0.0f,   0.0f,   0.0f, 100.0f };

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
        []() 
        {
            Menu::Instance().BoolModify("No Spread"); 
        });

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

    if (InsertDvar("cl_ingame")
        && InsertDvar("cg_fov")
        && InsertDvar("perk_weapSpreadMultiplier")
        && InsertDvar("sv_cheats")
        && InsertDvar("player_sustainAmmo"))
        this->GetOptionData(MISC_MENU, "FOV").data.integer = 65;
    else
        GameData::Com_Error(0, "Dvars failed to load\n");
}

void Menu::Insert(int sub, const char *option, OptionType type, 
    std::function<void()> &&callback)
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

bool Menu::BoolModify(const std::string &varName)
{
    OptionData &var = this->GetOptionData(this->currentSub, varName);
    return var.data.boolean = !var.data.boolean;
}

int Menu::IntModify(const std::string &varName, OptionType type, int min, int max)
{
    OptionData &var = this->GetOptionData(this->currentSub, varName);

    if (this->toggled)
        var.data.integer++;
    else
        var.data.integer--;

    if (var.data.integer > max)
        var.data.integer = min;
    if (var.data.integer < min)
        var.data.integer = max;

    return var.data.integer;
}

OptionData &Menu::GetOptionData(Submenu sub, const std::string &key)
{
    return this->options.at(sub).at(key).var;
}

bool Menu::Ready()
{
    return GameData::Sys_Milliseconds() > this->timer;
}

void Menu::Wait(int ms)
{
    this->timer = GameData::Sys_Milliseconds() + ms;
}

void Menu::Execute()
{
    const char *title = "WaW Zombies DLL By E7ite";
    float menuCenterX = GameData::uiDC->screenDimensions[0] / 2
        / GameData::scrPlace->scaleVirtualToFull[0];
    float menuCenterY = GameData::uiDC->screenDimensions[1] / 2
        / GameData::scrPlace->scaleVirtualToFull[1];

    // Get x position of text aligned with a background and scaled for all resolutions
    float textWidth, textHeight;
    GameData::Font_s *fontPointer;
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
    for (auto &i : options[currentSub])
    {
        Colors::Color color = Colors::white;
        const Option &option = i.second;

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

bool Menu::MonitorMouse(Option &opt, float optionX, float optionY,
    float optionW, float optionH)
{
    if (GameData::uiDC->cursorPos[0] > optionX
        && GameData::uiDC->cursorPos[0] < optionX + optionW
        && GameData::uiDC->cursorPos[1] > optionY - optionH
        && GameData::uiDC->cursorPos[1] < optionY)
    {
        if (this->Ready())
        {
            int delay = opt.var.type == TYPE_INT ? 100 : 200;
            if (GetAsyncKeyState(VK_LBUTTON) & 0x10000)
            {
                this->toggled = true;
                opt.callback();
                this->toggled = false;
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
    const float *color, const char *material, int type)
{
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace, x, y, width, height,
        angle, color, GameData::Material_RegisterHandle(material, type));
}

void DrawFillRect(float x, float y, float width, float height,
    const Colors::Color &color, float rotation, int type)
{
    RenderShader(x, y, width, height, rotation, color, "white", type);
}

void DrawEmptyRect(float x, float y, float width, float height, float size,
    const Colors::Color &color, int type)
{
    RenderShader(x, y, width, size, 0, color, "white", type); //up
    RenderShader(x, y + height, width + (size - 1), size, 0,  //down
        color, "white", type);
    RenderShader(x, y, size, height + (size - 1), 0, color,   //left
        "white", type);
    RenderShader(x + width, y, size, height + size, 0, color, //right
        "white", type);
}

float AlignText(const char *text, const Fonts::Font &font, float scale, float initX,
    ScreenAlignment align, bool ui, bool bg, GameData::Font_s **fOut,
    float *wOut, float *hOut)
{
    GameData::Font_s *fontPointer;
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

float RenderGameText(const char *text, float x, float y, float scale,
    const Colors::Color &color, GameData::Font_s *font, float rotation)
{
    GameData::R_AddCmdDrawStretchPic(text, INT_MAX, font, x, y,
        scale, scale, rotation, color, 0);

    return GameData::R_TextHeight(font) * scale;
}

float RenderGameTextWithBackground(const char *text, float x, float y,
    float textW, float textH, const Colors::Color &borderColor, 
    const Colors::Color &textColor, GameData::Font_s *font, float scale)
{
    DrawFillRect(x, y - textH, textW - 2, textH + 2,
        Colors::transparentBlack, 0);
    DrawEmptyRect(x, y - textH, textW - 2, textH + 2, 2,
        borderColor);
    RenderGameText(text, x + 6, y, scale, textColor, font, 0);

    return textH + 2;
}

float RenderUIText(const std::string &text, float x, float y, float scale,
    const Colors::Color &color, GameData::Font_s *font)
{
    UI_DrawText(GameData::scrPlace, text.data(), INT_MAX, font, x, y,
        scale, 0.0f, color, 0);

    return UI_TextHeight(font, scale) + 2.0f;
}

float RenderUITextWithBackground(const char *text, float x, float y,
    float textW, float textH, const Colors::Color &borderColor, 
    const Colors::Color &textColor, GameData::Font_s *font, float scale)
{
    GameData::UI_FillRect(GameData::scrPlace, x, y - textH, textW - 2, textH + 2,
        0, 0, Colors::transparentBlack);
    GameData::UI_DrawRect(GameData::scrPlace, x, y - textH, textW - 2, textH + 2, 0, 0, 2, borderColor);
    RenderUIText(text, x + 6, y, scale, textColor, font);

    return textH + 2;
}

void WriteBytes(DWORD addr, const char *bytes, size_t len)
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

void ReadBytes(DWORD addr, char *buf, size_t len)
{
    // Make virtual page address have read/write/exec privledges
    // and save old privledges to temporary
    DWORD curProtection;
    VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

    ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, buf, len, 0);

    // Restore old privledges from temporary to virtual page address
    VirtualProtect((LPVOID)addr, len, curProtection, 0);
}

bool InsertDvar(const char *dvarName, GameData::dvar_s *dvar)
{
    GameData::dvar_s *pdvar = dvar ? dvar : GameData::Dvar_FindVar(dvarName);
    if (!pdvar)
        return false;

    dvars.insert(std::pair<const char *, GameData::dvar_s *>(dvarName, pdvar));
    return true;
}

bool InGame()
{
    return dvars.at("cl_ingame")->current.enabled
        && *GameData::cl_connectionState >= 9;
}

bool CopyTextToClipboard(const std::string &text)
{
    if (!OpenClipboard(*GameData::hwnd))
        return false;
    if (!EmptyClipboard())
        return false;

    size_t len = text.size() + 1;
    bool state = false;
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hg)
    {
        memcpy(static_cast<LPSTR>(GlobalLock(hg)), text.c_str(), len);
        GlobalUnlock(hg);
        state = SetClipboardData(CF_TEXT, hg);
    }

    CloseClipboard();
    GlobalFree(hg);
    return state;
}

bool CopyAddressToClipboard(void *address)
{
    std::stringstream str;
    str << std::hex << address;
    return CopyTextToClipboard(str.str());
}

std::string FormatError(DWORD lastError)
{
    LPSTR message;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message, 0, NULL);
    return message;
}
