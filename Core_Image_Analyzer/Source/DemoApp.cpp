// DemoApp.cpp : Определяет точку входа для приложения.
//

#include "StdAfx.h"
#include "DemoApp.h"

#include "PhotoPanel.h"
//#include <afxctl.h>



#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int //image_width = 0,
    //image_height = 0,
    //image_UV_width = 0,
    //image_UV_height = 0,
    scroll_wnd_width     = 300,
    scroll_wnd_height    = 600;

SCROLLINFO si;

const RECT    draw_rect = {10, 10, 10 + 100 + scroll_wnd_width * 2 + 1, 10 + scroll_wnd_height };
//bool    changeScroll = false;


//const Gdiplus::RectF    rect_DL_dest { 10, 100, scroll_wnd_width, scroll_wnd_height };
//const Gdiplus::RectF    rect_UV_dest { 10, 100 + scroll_wnd_width, scroll_wnd_width, scroll_wnd_height };


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


const int MAX_PHOTO_WINDOWS = 2;
PhotoPanel photoPanel[MAX_PHOTO_WINDOWS];


HWND    hWnd_depth,
        hWnd_scrollbar;

IWICImagingFactory* gpIWICFactory;


//COleControlContainer coc;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // инициализация библиотеки GDI+
    ULONG_PTR m_gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);


    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEMOAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMOAPP));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // закрытие библиотеки GDI+
    Gdiplus::GdiplusShutdown(m_gdiplusToken);

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMOAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DEMOAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
 

   hWnd_scrollbar = CreateWindowEx(
       0,                     // no extended styles 
       L"SCROLLBAR",       // global string containing name of window class
       (PTSTR)NULL,             // global string containing title bar text 
       //WS_OVERLAPPEDWINDOW |
       WS_CHILD | WS_VISIBLE | WS_DISABLED |
       //SBS_HORZ | SBS_BOTTOMALIGN |
       SBS_VERT// | SBS_RIGHTALIGN
       //SBS_SIZEBOXBOTTOMRIGHTALIGN |
       //WS_VSCROLL
       ,
       //WS_HSCROLL | WS_VSCROLL, // window styles 
       100 + scroll_wnd_width + scroll_wnd_width + 4,  // CW_USEDEFAULT,         // default horizontal position 
       10,  // CW_USEDEFAULT,         // default vertical position 
       50,         // default width 
       scroll_wnd_height,  // CW_USEDEFAULT,         // default height 
       hWnd,           // no parent for overlapped windows 
       (HMENU)NULL,          // use the window class menu 
       hInstance,               // global instance handle  
       (PVOID)NULL           // pointer not needed 
   );

   /*hWnd_depth = CreateWindowEx(
       WS_EX_CLIENTEDGE,                     // no extended styles 
       szWindowClass,       // global string containing name of window class
       (PTSTR)NULL,             // global string containing title bar text 
       WS_CHILD | WS_VISIBLE, // window styles 
       10,         // default horizontal position 
       10,         // default vertical position 
       86,         // default width 
       scroll_wnd_height,         // default height 
       hWnd,           // no parent for overlapped windows 
       (HMENU)NULL,          // use the window class menu 
       hInstance,               // global instance handle  
       (PVOID)NULL           // pointer not needed 
   );*/

   // Создали нашу Factory, которая создает всё остальное, это обычный COM-объект
   CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&gpIWICFactory/*app.m_pIWICFactory*/);

   photoPanel[0].Create(hWnd, gpIWICFactory, 100, 10, 100 + scroll_wnd_width, 10 + scroll_wnd_height);
   photoPanel[1].Create(hWnd, gpIWICFactory, 100 + scroll_wnd_width/* + 2*/, 10, 100 + scroll_wnd_width * 2/* + 2*/, 10 + scroll_wnd_height);

   UpdateWindow(hWnd);

   return TRUE;
}


/******************************************************************
* Creates an open file dialog box and locate the image to decode. *
******************************************************************/

bool LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName) {
    pszFileName[0] = L'\0';

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    //ofn.lpstrFilter = L"All Image Files\0"              L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico\0"
    //    L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
    //    L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
    //    L"Graphics Interchange Format\0"  L"*.gif\0"
    //    L"Portable Network Graphics\0"    L"*.png\0"
    //    L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
    //    L"Tiff File\0"                    L"*.tif\0"
    //    L"Icon\0"                         L"*.ico\0"
    //    L"All Files\0"                    L"*.*\0"
    //    L"\0";
    ofn.lpstrFilter = L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0";
    ofn.lpstrFile = pszFileName;
    ofn.nMaxFile = cchFileName;
    ofn.lpstrTitle = L"Открыть фото керна";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

    // Display the Open dialog box. 
    return GetOpenFileName(&ofn);
}


//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
        case WM_COMMAND: {
                int wmId = LOWORD(wParam);
                // Разобрать выбор в меню:
                switch (wmId) {
                    case IDM_OPEN: {
                            if (!photoPanel[0].photos_vector.empty()) {
                                MessageBox(hWnd, L"Sorry, I can upload only one photo right now.", L"Warning", NULL);
                                break;
                            }
                            HRESULT hr;
                            WCHAR szFileName[MAX_PATH];
                            if (!LocateImageFile(hWnd, szFileName, MAX_PATH))
                                break;
                            hr = photoPanel[0].Add(szFileName);

                            int index = 0;
                            while (szFileName[index++] != 0);

                            // загружаем снимок в УФ
                            szFileName[index - 5] = '_';
                            szFileName[index - 4] = 'u';
                            szFileName[index - 3] = 'f';
                            szFileName[index - 2] = '.';
                            szFileName[index - 1] = 'j';
                            szFileName[index] = 'p';
                            szFileName[index + 1] = 'g';
                            szFileName[index + 2] = 0;

                            hr = photoPanel[1].Add(szFileName);

                            EnableWindow(hWnd_scrollbar, true);
                            si.cbSize = sizeof(SCROLLINFO);
                            si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
                            si.nMin = 0;
                            //si.nMax = image_height - scroll_wnd_height;    // 1176
                            si.nMax = photoPanel[0].photos_vector[0].height;    // по первому (в дневном свете) снимку
                            si.nPos = 0;
                            //si.nPage = scroll_wnd_height < image_height ? (scroll_wnd_height * si.nMax) / (double)image_height : 0;
                            si.nPage = scroll_wnd_height;
                            int iResult = SetScrollInfo(hWnd_scrollbar, SB_CTL, &si, true);
                            InvalidateRect(hWnd, &draw_rect, true);
                            UpdateWindow(hWnd);
                        }
                        break;
                    case IDM_ABOUT:
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                        break;
                    case IDM_EXIT:
                        DestroyWindow(hWnd);
                        break;
                    default:
                        return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        case WM_VSCROLL:{
                int old_pos = si.nPos;
                switch (LOWORD(wParam)) {
                    case SB_TOP:    // Прокручивается до верхнего левого угла.
                        si.nPos = si.nMin;
                         break; 
                    case SB_BOTTOM: // Прокручивается до нижнего правого угла.
                        si.nPos = si.nMax - si.nPage;
                        break;
                    case SB_LINEUP: // Прокручивает одну линию вверх.
                        si.nPos--;
                        break;
                    case SB_LINEDOWN:   // Прокручивает одну строку вниз.
                        si.nPos++;
                        break;
                    case SB_PAGEUP: // Прокручивает одну страницу вверх.
                        si.nPos -= si.nPage;
                        break;
                    case SB_PAGEDOWN:  // Прокручивает одну страницу вниз.
                        si.nPos += si.nPage;
                        break;
                    case SB_THUMBPOSITION:  // Пользователь перетаскивал поле прокрутки (большой палец) и отпустил кнопку мыши.HIWORD указывает положение поля прокрутки в конце операции перетаскивания.
                        si.nPos = HIWORD(wParam);
                        break;
                    case SB_THUMBTRACK: //  Пользователь перетаскивает полосу прокрутки.Это сообщение отправляется повторно, пока пользователь не отпустит кнопку мыши.HIWORD указывает положение, в которое перетаскивается поле прокрутки.
                        si.fMask = SIF_TRACKPOS;
                        GetScrollInfo(hWnd_scrollbar, SB_CTL, &si);
                        si.nPos = si.nTrackPos;   //  si.nTrackPos может быть на 1 больше si.nMax (bug?)
                        break;
                    case SB_ENDSCROLL:  // Заканчивается прокрутка.
                        break;
                    default:
                        ;//changeScroll = false;
                }

                if (old_pos != si.nPos) {
                    if (si.nPos > (si.nMax - (int)si.nPage))
                        si.nPos = si.nMax - si.nPage;
                    else if (si.nPos < si.nMin)
                        si.nPos = si.nMin;

                    si.fMask = SIF_POS;
                    SetScrollInfo(hWnd_scrollbar, SB_CTL, &si, true);

                    InvalidateRect(hWnd, &draw_rect, true);
                    UpdateWindow(hWnd);
                }
            }
            break;
        case WM_PAINT: {
                PAINTSTRUCT ps;
                ps.fErase = false;
                HDC hdc;
                RECT R;
                hdc = BeginPaint(hWnd, &ps);
                DrawEdge(hdc, &photoPanel[0].frame_rect, EDGE_SUNKEN, BF_RECT/*BF_ADJUST*/);
                DrawEdge(hdc, &photoPanel[1].frame_rect, EDGE_SUNKEN, BF_RECT/*BF_ADJUST*/);

                if (!photoPanel[0].photos_vector.empty()) {
                    Gdiplus::Graphics graphic0(hdc);
                     
                    if (photoPanel[0].photos_vector[0].m_pGdiPlusBitmap != nullptr) {
                        graphic0.DrawImage(photoPanel[0].photos_vector[0].m_pGdiPlusBitmap,
                            photoPanel[0].imageRect,
                            0,
                            si.nPos,
                            scroll_wnd_width - 2,
                            scroll_wnd_height - 2,
                            Gdiplus::UnitPixel
                        );
                    }

                    if (photoPanel[1].photos_vector[0].m_pGdiPlusBitmap != nullptr) {
                        double ratio = photoPanel[0].photos_vector[0].x_scale / photoPanel[1].photos_vector[0].x_scale;
                        int new_w = (int)((scroll_wnd_width - 2) * ratio);
                        int new_h = (int)((scroll_wnd_height - 2) * ratio);

                        graphic0.DrawImage(photoPanel[1].photos_vector[0].m_pGdiPlusBitmap,
                            photoPanel[1].imageRect,
                            0,
                            (int)((photoPanel[1].photos_vector[0].height - new_h/*scroll_wnd_height*/) * ((double)si.nPos / (double)(photoPanel[0].photos_vector[0].height - scroll_wnd_height))),
                            new_w,
                            new_h,
                            Gdiplus::UnitPixel
                        );
                    }
                    
                    HFONT font = CreateFontA(-12, 0, 0, 0, 700, 0, 0, 0, RUSSIAN_CHARSET, OUT_TT_PRECIS, 0, 0, VARIABLE_PITCH, "Arial");

                    SelectObject(hdc, font);
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, 0x000000);
                
                    std::string str;
                    R = { 10, 10, 80, 10 };
                    str = std::to_string(photoPanel[0].photos_vector[0].Y_top + photoPanel[0].photos_vector[0].y_scale * si.nPos);
                    DrawTextA(hdc, (LPCSTR)str.c_str(), -1, &R, DT_NOCLIP);
                    
                    int h = DrawTextA(hdc, (LPCSTR)str.c_str(), -1, &R, DT_CALCRECT);   // высота подписи
                    str = std::to_string(photoPanel[0].photos_vector[0].Y_top + photoPanel[0].photos_vector[0].y_scale * (si.nPos + si.nPage / 2)); // середина
                    R.top = 10 + (scroll_wnd_height - h) / 2;
                    R.bottom = 10 + (scroll_wnd_height + h) / 2;
                    DrawTextA(hdc, (LPCSTR)str.c_str(), -1, &R, DT_NOCLIP);

                    str = std::to_string(photoPanel[0].photos_vector[0].Y_top + photoPanel[0].photos_vector[0].y_scale * (si.nPos + si.nPage));
                    R.bottom = 10 + scroll_wnd_height;
                    R.top = R.bottom - h;
                    DrawTextA(hdc, (LPCSTR)str.c_str(), -1, &R, DT_NOCLIP);

                    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
                    SelectObject(hdc, pen);
                    MoveToEx(hdc, 100, 10 + scroll_wnd_height / 2, NULL);
                    LineTo(hdc, 100 + scroll_wnd_width * 2, 10 + scroll_wnd_height / 2);
                }

                EndPaint(hWnd, &ps);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

