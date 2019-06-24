#pragma once

#include "structures.h"

using QWORD = unsigned __int64;

enum MenuName
{
	MENU_CLOSED,
	MAIN_MENU,
	BASIC_MENU,
	AIMBOT_MENU,
	ESP_MENU,
	HUD_MENU
};

enum ScreenAlignment
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

enum Fonts
{
	smallFont = 3,
	normalFont = 1,
	consoleFont = 5,
	objectiveFont = 6,
};

enum OptionVer : char
{
	MENU,
	TOGGLE,
	INTEGER,
};

namespace Colors
{
	struct Color
	{
		float r, g, b, a;
		
		Color() : r(0.0F), g(0.0f), b(0.0f), a(1.0f) {}
		Color(float r, float g, float b, float a)
			: r(r / 255), g(g / 255), b(b / 255), a(a / 255) {}

		operator float*() { return reinterpret_cast<float*>(this); }
	};

	extern Color white;
	extern Color black;
	extern Color red;
	extern Color green;
	extern Color blue;
	extern Color transparentBlack;
}

namespace Menu
{
	struct Option
	{
		OptionVer type;
		__int32 *var;
		__int32 arg;
		__int32 minimum, maximum;

		Option(OptionVer type, __int32 *var = nullptr,
			__int32 arg = -1,  __int32 min = -1, __int32 max = -1)
			: type(type), var(var), arg(arg), minimum(min), maximum(max) {}
	};

	extern __int8 open;
	extern __int32 currentOption;
	extern MenuName currentSub;
	extern __int32 timer;
	extern std::map<MenuName,
		std::map<const char*, Option>> options;
};

namespace GameData
{
	extern std::map<const char*, dvar_s*> dvarGlob;
	extern std::vector<QWORD> detours;
};

namespace Variables
{
	extern __int32 enableAimbot;
	extern __int32 aimRequired;
	extern __int32 aimTag;
	extern __int32 autoAim;
	extern __int32 autoShoot;
	extern __int32 targetType;
	extern __int32 noRecoil;
	extern __int32 enableEsp;
	extern __int32 boxType;
	extern __int32 zombieEsp;
	extern __int32 teamEsp;
	extern __int32 enableCheats;
	extern __int32 fov;
}

using OptionIterator	= std::map<const char*, Menu::Option>::iterator;
using OptionEntryHolder = std::map<const char*, Menu::Option>;
using OptionEntry		= std::pair<const char*, Menu::Option>;
using Submenu			= std::pair<MenuName,
							std::map<const char*, Menu::Option>>;

void MenuWait(int ms);
std::function<void()> GetMenuFunction(bool *toggled, const Menu::Option &opt);
bool MenuReady();
void BuildMenu();
void HandleControls();
void CloseSub();
void ExecuteMenu();
void RunMenu();
void DrawHuds();
void VariableToggle(__int32 *var);
void VariableModify(bool *increment, __int32 *var,
	__int32 min, __int32 max);
bool MonitorMouse(bool *toggled,
	const std::function<void()> &func, int optPos,
	float optionX, float optionY, float optionW, float optionH,
	int &opt);
void LoadSub(MenuName menu);
void RenderShader(float x, float y, float width, float height, float angle,
	const float *color, const char *material, int type = 0);
float CalculateMenuHeight(float y, float scale,
	Fonts fontName);
float RenderText(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *color,
	Fonts fontName, float rotation, int fontType = 0,
	float *wOut = nullptr, float *hOut = nullptr);
void DrawFillRect(float x, float y, float width, float height,
	const float *color, float rotation, int type = 0);
void DrawEmptyRect(float x, float y, float width, float height, float size,
	const float *color, int type = 0);
float RenderTextWithBackground(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *borderColor, const float *textColor,
	Fonts fontName, int fontType = 0, float *wOut = nullptr, float *hOut = nullptr);
bool InGame();

void WriteBytes(DWORD addr, const char *bytes, size_t len);

template <typename T>
T ReadBytes(DWORD addr)
{
	T type;
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)addr, &type, sizeof T, 0);

	return type;
}
