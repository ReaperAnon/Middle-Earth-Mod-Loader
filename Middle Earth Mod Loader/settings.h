#pragma once

#include "framework.h"

namespace Settings
{
	extern CSimpleIni	iniFile;
	extern std::string	iniFileName;

	extern float		fontScale;

	extern bool			saveWhenClosing;
	extern bool			saveOnDefault;
	extern bool			showAllMods;
	extern bool			debugState;
	extern int			logLimit;

	void ReadSettings();

	void SetDefaultSettings();

	void SaveSettings();

	void SavePlugins();
}