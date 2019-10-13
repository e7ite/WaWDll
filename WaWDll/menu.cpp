#include "menu.h"

std::vector<std::vector<Option>> Menu::options;
int Menu::open;
int Menu::toggled;
int Menu::currentSub;
int Menu::timer;

int Variables::enableAimbot;
int Variables::aimKey;
int Variables::aimType;
int Variables::autoAim;
int Variables::autoShoot;
int Variables::noSpread;
int Variables::noRecoil;
int Variables::enemyESP;
int Variables::friendlyESP;
int Variables::fov;
int Variables::steadyAim;
int Variables::serverInfo;

void Menu::Build()
{
	using namespace Variables;

	for (int sub = MAIN_MENU; sub <= HUD_MENU; sub++)
		options.push_back(std::vector<Option>());

	Insert(MAIN_MENU, "Aimbot Menu", nullptr,
		TYPE_SUB, std::bind(LoadSub, AIMBOT_MENU));
	Insert(MAIN_MENU, "ESP Menu", nullptr,
		TYPE_SUB, std::bind(LoadSub, ESP_MENU));
	Insert(MAIN_MENU, "HUD Menu", nullptr,
		TYPE_SUB, std::bind(LoadSub, HUD_MENU));
	Insert(MAIN_MENU, "Misc Menu", nullptr,
		TYPE_SUB, std::bind(LoadSub, MISC_MENU));

	Insert(ESP_MENU, "Enemy ESP", &enemyESP,
		TYPE_BOOL, std::bind(BoolModify, &enemyESP));
	Insert(ESP_MENU, "Friendly ESP", &friendlyESP,
		TYPE_BOOL, std::bind(BoolModify, &friendlyESP));

	Insert(HUD_MENU, "Server Info", &serverInfo,
		TYPE_BOOL, std::bind(BoolModify, &serverInfo));

	Insert(AIMBOT_MENU, "Enable Aimbot", &enableAimbot,
		TYPE_BOOL, std::bind(BoolModify, &enableAimbot));
	Insert(AIMBOT_MENU, "Aim Key", &aimKey,
		TYPE_INT, std::bind(IntModify, &aimKey, 0, 2));
	Insert(AIMBOT_MENU, "Aim Type", &aimType,
		TYPE_INT, std::bind(IntModify, &aimType, 0, 2));
	Insert(AIMBOT_MENU, "Auto Aim", &autoAim,
		TYPE_BOOL, std::bind(BoolModify, &autoAim));
	Insert(AIMBOT_MENU, "Auto Shoot", &autoShoot,
		TYPE_BOOL, std::bind(BoolModify, &autoShoot));
	Insert(AIMBOT_MENU, "No Recoil", &noRecoil,
		TYPE_BOOL, std::bind(BoolModify, &noRecoil));
	Insert(AIMBOT_MENU, "No Spread", &noSpread,
		TYPE_BOOL, std::bind(BoolModify, &noSpread));

	Insert(MISC_MENU, "FOV", &fov,
		TYPE_INT, std::bind(IntModify, &fov, 65, 125));
	Insert(MISC_MENU, "Super Steady Aim", &steadyAim,
		TYPE_BOOL, std::bind(BoolModify, &steadyAim));
}

void Menu::Insert(int sub, const char *option, int *data,
	OptionType type, const std::function<void()> &callback)
{
	options.at(sub).push_back(Option(option, data, type, callback));
}

void Menu::LoadSub(int sub)
{
	currentSub = sub;
}

void Menu::CloseSub()
{
	switch (currentSub)
	{
	case MAIN_MENU:
		open = false;
		break;
	case AIMBOT_MENU:
	case ESP_MENU:
	case HUD_MENU:
	case MISC_MENU:
		currentSub = MAIN_MENU;
		break;
	default:
		open = false;
		break;
	}
}

void Menu::BoolModify(int *var)
{
	*var = !*var;
}

void Menu::IntModify(int *var, int min, int max)
{
	if (toggled)
		++(*var);
	else
		--(*var);

	if (*var > max)
		*var = min;
	if (*var < min)
		*var = max;
}

bool Menu::Ready()
{
	return Sys_Milliseconds() > timer;
}

void Menu::Wait(int ms)
{
	timer = Sys_Milliseconds() + ms;
}

void Menu::Execute()
{
	float menuCenterX = dc->screenDimensions[0] / 2
		/ scrPlace->scaleVirtualToFull[0];
	float menuCenterY = dc->screenDimensions[1] / 2
		/ scrPlace->scaleVirtualToFull[1];
	float optionY = menuCenterY - 100;

	float textWidth, textHeight;
	Font_s *fontPointer;
	float xAligned = AlignText("WaW Zombies DLL By E7ite", GameData::normalFont, 0.3f,
		menuCenterX, ALIGN_CENTER, 1, 1, &fontPointer, &textWidth, &textHeight);
	optionY += RenderUITextWithBackground("WaW Zombies DLL By E7ite",
		xAligned, optionY, textWidth, textHeight, Colors::blue,
		Colors::white, fontPointer, 0.3f);

	float borderW = menuCenterX - 20,
		borderH = textHeight * options[currentSub].size(),
		borderX = menuCenterX - borderW / 2, borderY = menuCenterY - 98;
	UI_FillRect(scrPlace, borderX, borderY, borderW, borderH, 0, 0,
		Colors::transparentBlack);
	UI_DrawRect(scrPlace, borderX, borderY, borderW, borderH, 
		0, 0, 2, Colors::blue);

	float optionX = borderX + 4,
		optionH = (float)UI_TextHeight(fontPointer, 0.3f);
	for (const auto &i : options[currentSub])
	{
		Colors::Color color = Colors::white;
		if (MonitorMouse(i, borderX, optionY - 2, borderW, optionH + 2))
			color = Colors::blue;

		switch (i.type)
		{
		case TYPE_BOOL:
			UI_FillRect(scrPlace, borderX + borderW - 12,
				optionY - optionH + 2, 8, 8, 0, 0,
				*i.data ? Colors::blue : Colors::transparentBlack);
			break;
		case TYPE_INT:
			std::string data = std::to_string(*i.data);
			RenderUIText(data.data(),
				AlignText(data.data(), GameData::normalFont,
					0.3f, borderX + borderW - 3, ALIGN_RIGHT, 1, 0),
				optionY, 0.3f, color, fontPointer);
			break;
		}

		optionY += RenderUIText(i.option, optionX,
			optionY, 0.3f, color, fontPointer);
	}
}

bool Menu::MonitorMouse(const Option &opt, float optionX, float optionY,
	float optionW, float optionH)
{
	if (dc->cursorPos[0] > optionX
		&& dc->cursorPos[0] < optionX + optionW
		&& dc->cursorPos[1] > optionY - optionH
		&& dc->cursorPos[1] < optionY)
	{
		if (Ready())
			if (GetAsyncKeyState(VK_LBUTTON) & 0x10000)
			{
				toggled = true;
				opt.callback();
				toggled = false;
				Wait(100);
			}
			else if (GetAsyncKeyState(VK_RBUTTON) & 0x10000)
			{
				opt.callback();
				Wait(100);
			}

		return true;
	}

	return false;
}

void Menu::MonitorKeys()
{
	if (Ready())
	{
		if (GetAsyncKeyState(VK_HOME) && !open)
		{
			open = true;
			currentSub = MAIN_MENU;
			Wait(200);
		}
		if (GetAsyncKeyState(VK_BACK) & 0x10000)
		{
			CloseSub();
			Wait(200);
		}
	}
}

void RenderShader(float x, float y, float width, float height, float angle,
	const float *color, const char *material, int type)
{
	CG_DrawRotatedPicPhysical(scrPlace, x, y, width, height, angle, color,
		Material_RegisterHandle(material, type));
}

void DrawFillRect(float x, float y, float width, float height,
	const float *color, float rotation, int type)
{
	RenderShader(x, y, width, height, rotation, color, "white", type);
}

void DrawEmptyRect(float x, float y, float width, float height, float size,
	const float *color, int type)
{
	RenderShader(x, y, width, size, 0, color, "white", type); //up
	RenderShader(x, y + height, width + (size - 1), size, 0,  //down
		color, "white", type);
	RenderShader(x, y, size, height + (size - 1), 0, color,   //left
		"white", type);
	RenderShader(x + width, y, size, height + size, 0, color, //right
		"white", type);
}

float AlignText(const char *text, const GameData::Font &font, float scale, float initX,
	ScreenAlignment align, bool ui, bool bg, Font_s **fOut, float *wOut, float *hOut)
{
	Font_s *fontPointer;
	float width, height;
	float xAligned;

	if (bg && ui)
	{
		fontPointer = UI_GetFontHandle(scrPlace, font.index);
		width = (float)UI_TextWidth(text, INT_MAX, fontPointer, scale) + 12.0f;
		height = (float)UI_TextHeight(fontPointer, scale) + 2.0f;
	}
	else if (!bg && ui)
	{
		fontPointer = UI_GetFontHandle(scrPlace, font.index);
		width = (float)UI_TextWidth(text, INT_MAX, fontPointer, scale);
		height = (float)UI_TextHeight(fontPointer, scale) + 2.0f;
	}
	else if (bg && !ui)
	{
		fontPointer = R_RegisterFont(font.dir, 0);
		width = (float)R_TextWidth(text, INT_MAX, fontPointer) * scale + 14.0f;
		height = (float)R_TextHeight(fontPointer) * scale + 2.0f;
	}
	else
	{
		fontPointer = R_RegisterFont(font.dir, 0);
		width = (float)R_TextWidth(text, INT_MAX, fontPointer) * scale;
		height = (float)R_TextHeight(fontPointer) * scale + 2.0f;
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
	const float *color, Font_s *font, float rotation)
{
	CL_DrawTextPhysical(text, INT_MAX, font, x, y,
		scale, scale, rotation, color, 0);

	return R_TextHeight(font) * scale;
}

float RenderGameTextWithBackground(const char *text, float x, float y,
	float textW, float textH, const float *borderColor, const float *textColor,
	Font_s *font, float scale)
{
	DrawFillRect(x, y - textH, textW - 2, textH + 2,
		Colors::transparentBlack, 0);
	DrawEmptyRect(x, y - textH, textW - 2, textH + 2, 2,
		borderColor);
	RenderGameText(text, x + 6, y, scale, textColor, font, 0);

	return textH + 2;
}

float RenderUIText(const char *text, float x, float y, float scale,
	const float *color, Font_s *font)
{
	UI_DrawText(scrPlace, text, INT_MAX, font, x, y,
		scale, 0.0f, color, 0);

	return UI_TextHeight(font, scale) + 2.0f;
}

float RenderUITextWithBackground(const char *text, float x, float y,
	float textW, float textH, const float *borderColor, const float *textColor,
	Font_s *font, float scale)
{
	UI_FillRect(scrPlace, x, y - textH, textW - 2, textH + 2,
		0, 0, Colors::transparentBlack);
	UI_DrawRect(scrPlace, x, y - textH, textW - 2, textH + 2, 0, 0, 2, borderColor);
	RenderUIText(text, x + 6, y, scale, textColor, font);

	return textH + 2;
}

void WriteBytes(DWORD addr, const char *bytes, size_t len)
{
	DWORD curProtection;
	VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)addr, bytes, len, 0);

	VirtualProtect((LPVOID)addr, len, curProtection, 0);
}

void ReadBytes(DWORD addr, char *buf, size_t len)
{
	DWORD curProtection;
	VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, buf, len, 0);

	VirtualProtect((LPVOID)addr, len, curProtection, 0);
}