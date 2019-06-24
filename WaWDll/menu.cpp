#include "menu.h"

__int8 Menu::open = false;
__int32 Menu::currentOption;
MenuName Menu::currentSub = MENU_CLOSED;
__int32 Menu::timer = 0;
std::map<MenuName,
	std::map<const char*, Menu::Option>> Menu::options;
std::map<const char*, dvar_s*> GameData::dvarGlob;
std::vector<QWORD> GameData::detours;

__int32 Variables::enableAimbot = 0;
__int32 Variables::aimRequired  = 0;
__int32 Variables::autoAim	    = 0;
__int32 Variables::aimTag       = 0;
__int32 Variables::autoShoot	= 0;
__int32 Variables::targetType   = 0;
__int32 Variables::noRecoil     = 0;
__int32 Variables::enableEsp    = 0;
__int32 Variables::boxType	    = 0;
__int32 Variables::zombieEsp	= 0;
__int32 Variables::teamEsp	    = 0;
__int32 Variables::enableCheats = 0;
__int32 Variables::fov          = 65;

Colors::Color Colors::white			   = { 255, 255, 255, 255 };
Colors::Color Colors::black			   = {   0,   0,   0, 255 };
Colors::Color Colors::red			   = { 255,   0,   0, 255 };
Colors::Color Colors::green            = {   0, 255,   0, 255 };
Colors::Color Colors::blue			   = {   0,   0, 255, 255 };
Colors::Color Colors::transparentBlack = {   0,   0,   0, 100 };

void MenuWait(int ms)
{
	Menu::timer = Sys_Milliseconds() + ms;
}

bool MenuReady()
{
	return Sys_Milliseconds() > Menu::timer;
}

void BuildMenu()
{
	Menu::options.insert(
		Submenu(
			MAIN_MENU, 
			OptionEntryHolder {
				OptionEntry(
					"Basic Mods",
					Menu::Option(MENU, nullptr, (int)BASIC_MENU)
				),
				OptionEntry(
					"Aimbot Menu", 
					Menu::Option(MENU, nullptr, (int)AIMBOT_MENU)
				),
				OptionEntry(
					"ESP Menu",
					Menu::Option(MENU, nullptr, (int)ESP_MENU)
				),
				OptionEntry(
					"HUD Menu",
					Menu::Option(MENU, nullptr, (int)HUD_MENU)
				)
			}
		)
	);
	Menu::options.insert(
		Submenu(
			BASIC_MENU,
			OptionEntryHolder {
				OptionEntry(
					"No Recoil",
					Menu::Option(TOGGLE, &Variables::noRecoil)
				),
				OptionEntry(
					"Unlock Cheat Commands",
					Menu::Option(TOGGLE, &Variables::enableCheats)
				),
				OptionEntry(
					"FOV",
					Menu::Option(INTEGER, &Variables::fov, -1, 65, 120)
				)
			}
		)
	);
	Menu::options.insert(
		Submenu(
			AIMBOT_MENU,
			OptionEntryHolder {
				OptionEntry(
					"Enable Aimbot",
					Menu::Option(TOGGLE, &Variables::enableAimbot)
				),
				OptionEntry(
					"Aim Required", 
					Menu::Option(TOGGLE, &Variables::aimRequired)
				),
				OptionEntry(
					"Bone Tag", 
					Menu::Option(INTEGER, &Variables::aimTag, -1, 0, 5)
				),
				OptionEntry(
					"Auto Shoot",
					Menu::Option(TOGGLE, &Variables::autoShoot)
				)
			}
		)
	);
	Menu::options.insert(
		Submenu(
			ESP_MENU,
			OptionEntryHolder {
				OptionEntry(
					"Enable ESP",
					Menu::Option(TOGGLE, &Variables::enableEsp)
				),
				OptionEntry(
					"Zombie ESP", 
					Menu::Option(TOGGLE, &Variables::zombieEsp)
				),
				OptionEntry(
					"Team ESP",
					Menu::Option(TOGGLE, &Variables::teamEsp)
				)
			}
		)
	);
}

std::function<void()> GetMenuFunction(bool *toggled, const Menu::Option &opt)
{
	switch (opt.type)
	{
	case MENU:
		return std::bind(LoadSub, (MenuName)opt.arg);
	case TOGGLE:
		return std::bind(VariableToggle, opt.var);
	case INTEGER:
		return std::bind(VariableModify, toggled, opt.var,
			opt.minimum, opt.maximum);
	}

	return std::function<void()>();
}

void HandleControls()
{
	if (MenuReady())
	{
		if (GetAsyncKeyState(VK_HOME) && !Menu::open)
		{
			Menu::open = true; 
			Menu::currentSub = MAIN_MENU;
			MenuWait(200);
		}
		if (GetAsyncKeyState(VK_BACK) & 0x10000)
		{
			CloseSub();
			MenuWait(200);
		}
		if (Menu::open)
			if (InGame())
			{
				if (GetAsyncKeyState(VK_NEXT) & 0x10000)
				{
					++Menu::currentOption;

					if (Menu::currentOption 
							>= (int)Menu::options[Menu::currentSub].size())
						Menu::currentOption = 0;

					MenuWait(200);
				}
				if (GetAsyncKeyState(VK_PRIOR) & 0x10000)
				{
					--Menu::currentOption;

					if (Menu::currentOption < 0)
						Menu::currentOption
							= Menu::options[Menu::currentSub].size() - 1;

					MenuWait(200);
				}
				for (const auto &i : Menu::options[Menu::currentSub])
					if (GetAsyncKeyState(VK_OEM_PLUS) & 0x10000
							|| GetAsyncKeyState(VK_OEM_MINUS) & 0x10000)
						{
							bool toggled 
								= GetAsyncKeyState(VK_OEM_PLUS) & 0x10000;

							GetMenuFunction(&toggled, i.second)();
							MenuWait(200);

							break;
						}
			}
	}
}

void CloseSub()
{
	switch (Menu::currentSub)
	{
	case BASIC_MENU:
	case AIMBOT_MENU:
	case ESP_MENU:
	case HUD_MENU:
		Menu::currentSub = MAIN_MENU;
		break;
	case MAIN_MENU:
		Menu::currentSub = MENU_CLOSED;
		Menu::open = false;
	}
}

bool MonitorMouse(bool *toggled,
	const std::function<void()> &func, int optPos,
	float optionX, float optionY, float optionW, float optionH,
	int &opt)
{
	if (dc->cursorPos[0] > optionX  && dc->cursorPos[0] < optionW
		&& dc->cursorPos[1] > optionY - (optionH - optionY)
		&& dc->cursorPos[1] < optionY)
	{
		opt = optPos;

		if (MenuReady())
			if (GetAsyncKeyState(VK_LBUTTON) & 0x10000)
			{
				*toggled = false;
				func();
				MenuWait(200);

				return false;
			}
			else if (GetAsyncKeyState(VK_RBUTTON) & 0x10000)
			{
				*toggled = true;
				func();
				MenuWait(200);

				return false;
			}
	}

	return true;
}

void ExecuteMenu()
{
	HandleControls();

	if (Menu::open)
		RunMenu();

	if (InGame())
		DrawHuds();
}

void VariableToggle(__int32 *var)
{
	*var = !(*var);
}

void VariableModify(bool *increment, __int32 *var,
	__int32 min, __int32 max)
{
	if (*increment)
		++(*var);
	else 
		--(*var);

	if (*var > max)
		(*var) = min;
	if (*var < min)
		(*var) = max;
}

void RunMenu()
{
	float y = 0, height = 0;
	float menuCenterX = dc->screenDimensions[0] / 2
		/ scrPlace->scaleVirtualToFull[0];
	float menuCenterY = dc->screenDimensions[1] / 2
		/ scrPlace->scaleVirtualToFull[1];

	height += RenderTextWithBackground("Zombies DLL By E7ite", 
				menuCenterX,
				menuCenterY - 200,
				0.5f, ALIGN_CENTER, Colors::blue, 
				Colors::white, normalFont, 0);

	float menuHeight = 0;
	for (const auto i : Menu::options[Menu::currentSub])
		menuHeight += CalculateMenuHeight(menuHeight, 0.3f, normalFont);

	DrawFillRect(menuCenterX - 135, 
		height - CalculateMenuHeight(menuHeight, 0.3f, normalFont),
		265.f, menuHeight, Colors::transparentBlack, 0.0f);
	DrawEmptyRect(menuCenterX - 135,
		height - CalculateMenuHeight(menuHeight, 0.3f, normalFont),
		265.f, menuHeight, 2, Colors::blue);

	for (OptionIterator i = Menu::options[Menu::currentSub].begin();
		i != Menu::options[Menu::currentSub].end();
		++i)
	{
		bool toggled;
		float xBound;
		float textHeight;
		float prevH = height;
		int dist    = std::distance(
			Menu::options[Menu::currentSub].begin(), i
		);

		height += RenderText(i->first,
			menuCenterX - 128,
			height, 0.3f, ALIGN_LEFT,
			dist == Menu::currentOption
			? Colors::blue : Colors::white,
			normalFont, 0.0f, 0, &xBound, &textHeight);

		switch (i->second.type)
		{
		case TOGGLE:
			DrawFillRect(menuCenterX + 115, prevH - textHeight + 4, 
				textHeight - 6, textHeight - 6,
				*i->second.var ? Colors::blue : Colors::black, 0);
			break;
		case INTEGER:
			RenderText(std::to_string(*i->second.var).data(),
				menuCenterX + 115, prevH, 0.3f, ALIGN_RIGHT,
				dist == Menu::currentOption 
				? Colors::blue : Colors::white,
				normalFont, 0.0f);
			break;
		}

		if (!MonitorMouse(&toggled, 
				GetMenuFunction(&toggled, i->second), dist,
				xBound + 25.f, prevH, xBound + 220.f, height,
				Menu::currentOption))
			break;

	}
}

void DrawHuds()
{
	std::stringstream tmp;
	Colors::Color color;
	float menuCenterX = dc->screenDimensions[0] / 2
		/ scrPlace->scaleVirtualToFull[0];
	float menuCenterY = dc->screenDimensions[1] / 2
		/ scrPlace->scaleVirtualToFull[1];

	tmp << "Health: "
		<< cgameGlob->predictedPlayerState.health << " / "
		<< cgameGlob->predictedPlayerState.maxHealth;
	color[0] = (255.f - (cgameGlob->predictedPlayerState.health 
		* (255.f / cgameGlob->predictedPlayerState.maxHealth))) / 255.f;
	color[1] = (cgameGlob->predictedPlayerState.health 
		* (255.f / cgameGlob->predictedPlayerState.maxHealth)) / 255.f;

	DrawFillRect(menuCenterX - 75.f, menuCenterY + 200.f, 
		cgameGlob->predictedPlayerState.health 
			* (150.f / cgameGlob->predictedPlayerState.maxHealth),
		15.f, color, 0);
	DrawEmptyRect(menuCenterX - 75, menuCenterY + 200.f, 150.f, 15.f,
		2, Colors::black, 0);
	RenderText(tmp.str().data(), menuCenterX, menuCenterY + 202, 0.2f,
		ALIGN_CENTER, Colors::white, normalFont, 0);
}

void LoadSub(MenuName menu)
{
	Menu::currentSub = menu;
}

void RenderShader(float x, float y, float width, float height, float angle,
	const float *color, const char *material, int type)
{
	CG_DrawRotatedPic(scrPlace, x, y, width, height, angle, color, 
		 Material_RegisterHandle(material, type));
}

float CalculateMenuHeight(float y, float scale,
	Fonts fontName)
{
	return UI_TextHeight(UI_GetFontHandle(scrPlace, fontName), scale) + 2;
}

float RenderText(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *color,
	Fonts fontName, float rotation, int fontType,
	float *wOut, float *hOut)
{
	Font_s *font     = UI_GetFontHandle(scrPlace, fontName);
	float textWidth  = UI_TextWidth(text, INT_MAX, font, scale);
	float textHeight = UI_TextHeight(font, scale) + 2;
	float xAligned;

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = x;
		break;
	case ALIGN_CENTER:
		xAligned = x - textWidth / 2;
		break;
	case ALIGN_RIGHT:
		xAligned = x - textWidth;
		break;
	default:
		xAligned = x;
		break;
	}

	if (wOut != nullptr)
		*wOut = xAligned;
	if (hOut != nullptr)
		*hOut = textHeight;

	UI_DrawText(scrPlace, text, INT_MAX, font, xAligned, y,
		scale, rotation, color, fontType);

	return textHeight;
}

float RenderTextWithBackground(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *borderColor, const float *textColor,
	Fonts fontName, int fontType, float *wOut, float *hOut)
{
	Font_s *font     = UI_GetFontHandle(scrPlace, fontName);
	float textWidth  = UI_TextWidth(text, INT_MAX, font, scale) + 12;
	float textHeight = UI_TextHeight(font, scale) + 2;
	float xAligned;

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = x;
		break;
	case ALIGN_CENTER:
		xAligned = x - textWidth / 2;
		break;
	case ALIGN_RIGHT:
		xAligned = x - textWidth;
		break;
	default:
		xAligned = x;
		break;
	}

	if (wOut != nullptr)
		*wOut = xAligned;
	if (hOut != nullptr)
		*hOut = textHeight;

	DrawFillRect(xAligned, y - textHeight, textWidth - 2, textHeight + 2,
		Colors::transparentBlack, 0, fontType);
	DrawEmptyRect(xAligned, y - textHeight, textWidth - 2, textHeight + 2, 2,
		borderColor, fontType);
	RenderText(text, xAligned + 6, y, scale, ALIGN_LEFT, textColor,
		fontName, 0, fontType);

	return y + textHeight + 2;
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

void WriteBytes(DWORD addr, const char *bytes, size_t len)
{
	DWORD curProtection;
	VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)addr, bytes, len, 0);

	VirtualProtect((LPVOID)addr, len, curProtection, nullptr);
}

void InsertDvar(const char *dvarName)
{
	GameData::dvarGlob.insert(
		std::pair<const char*, dvar_s*>(dvarName, Dvar_FindVar(dvarName))
	);
}

bool InGame()
{
	return GameData::dvarGlob["cl_ingame"] && *(int*)cl_connectionState >= 9;
}