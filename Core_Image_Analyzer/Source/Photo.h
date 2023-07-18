#pragma once

//#include "DemoApp.h"
//#include "StdAfx.h"

class Photo {
private:
	std::wstring _fileName;

	//HWND m_hwnd;

	IWICImagingFactory* m_pIWICFactory;
	//ID2D1Factory* m_pDirect2dFactory;
	//ID2D1HwndRenderTarget* m_pRenderTarget;
	//ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	//ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	IWICBitmapSource* m_pOriginalBitmapSource;

	BYTE* m_pbBuffer;
public:
	UINT width = 0, height = 0;
	double  X_left, X_right, Y_top, Y_bottom, x_scale, y_scale;
	Gdiplus::Bitmap* m_pGdiPlusBitmap;
	HRESULT Create(WCHAR* szFileName);
	HRESULT SetCoords(WCHAR* szFileName);
	Photo(IWICImagingFactory* pIWICFactory);
	~Photo();
};