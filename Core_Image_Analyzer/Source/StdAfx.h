// header.h: включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows

// Файлы заголовков Windows
#include <windows.h>
// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h>

#include <string>
#include <vector>

#include <fstream>

// Windows Header Files:
//#include <windows.h>

// C RunTime Header Files:
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
#include <wchar.h>
#include <math.h>


#include <wincodec.h>

//#include <WICViewerGDIPlus.h>

//#include <wincodecsdk.h>
//#include <windows.graphics.imaging.interop.h>


#include <d2d1.h>
//#include <d2d1helper.h>
//#include <dwrite.h>
//#include <wincodec.h>


#include <gdiplus.h>
#include <gdiplustypes.h>
//#include <gdiplusheaders.h>

#pragma comment(lib, "Gdiplus.lib") 
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")


template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}