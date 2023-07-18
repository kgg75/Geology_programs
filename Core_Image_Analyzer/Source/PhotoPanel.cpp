#include "StdAfx.h"
#include "PhotoPanel.h"
//#include "DemoApp.h"


PhotoPanel::PhotoPanel() {
}


PhotoPanel::~PhotoPanel() {
}


void PhotoPanel::Create(HWND hwnd_parent, IWICImagingFactory* gpIWICFactory, int x_topleft, int y_topleft, int x_bottomright, int y_bottomright) {
	hWnd_parent = hwnd_parent;
	gpIWICFactory_ = gpIWICFactory;
	X_topleft = x_topleft;
	Y_topleft = y_topleft;
	X_bottomright = x_bottomright;
	Y_bottomright = y_bottomright;
	frame_X_topleft = x_topleft;// -1;
	frame_Y_topleft = y_topleft;// -1;
	frame_X_bottomright = x_bottomright;// +1;
	frame_Y_bottomright = y_bottomright;// +1;
	frame_rect = RECT { X_topleft, Y_topleft, X_bottomright, Y_bottomright };
	image_rect = RECT { frame_X_topleft, frame_Y_topleft, frame_X_bottomright, frame_Y_bottomright };
	width = x_bottomright - x_topleft;
	height = y_bottomright - y_topleft;
	imageRect = Gdiplus::Rect(X_topleft + 1, Y_topleft + 1, width - 2, height - 2);
}


void PhotoPanel::Draw(HDC hdc) {
}


HRESULT PhotoPanel::Add(WCHAR* szFileName) {
    HRESULT hr = S_OK;
	Photo photo(gpIWICFactory_);

	hr = photo.Create(szFileName);
	if (SUCCEEDED(hr)) {
		int index = 0;
		while (szFileName[index++] != 0);
		szFileName[index - 3] = 'g';
		szFileName[index - 2] = 'w';
		hr = photo.SetCoords(szFileName);	// читаем jgw
	}

	photos_vector.push_back(photo);
   
	return hr;
}