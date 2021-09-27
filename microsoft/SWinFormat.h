#pragma once

#include "global.h"

namespace WinApi
{
	QString FormatMemProtect(DWORD value);
	QString FormatMemState(DWORD value);
	QString FormatMemType(DWORD value);
}
