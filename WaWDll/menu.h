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

struct Option
{
	const char *option;
	int *data;
	OptionType type;
	std::function<void()> callback;

	Option(const char *option, int *data,
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
	void Insert(int sub, const char *option, int *data,
		OptionType type, const std::function<void()> &func);
	void LoadSub(int sub);
	void CloseSub();
	void BoolModify(int *var);
	void IntModify(int *var, int min, int max);
	bool Ready();
	void Wait(int ms);
	bool MonitorMouse(const Option &opt, float optionX, float optionY,
		float optionW, float optionH);
	void MonitorKeys();
};

namespace Variables
{
	extern int enableAimbot;
	extern int aimKey;
	extern int aimType;
	extern int autoAim;
	extern int autoShoot;
	extern int noSpread;
	extern int noRecoil;

	extern int enemyESP;
	extern int friendlyESP;

	extern int serverInfo;

	extern int fov;	
	extern int steadyAim;
	extern int cheatsEnabled;
	extern int godMode;
	extern int infAmmo;
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