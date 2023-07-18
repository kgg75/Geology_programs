#pragma once


//#include "StdAfx.h"
#include "Photo.h"


class PhotoPanel {
	private:
		IWICImagingFactory* gpIWICFactory_;
		//double  X, Y, x_scale, y_scale;

	public:
		std::vector <Photo> photos_vector;

		HWND hWnd_parent;
		int X_topleft, Y_topleft, X_bottomright, Y_bottomright, width, height;
		int frame_X_topleft, frame_Y_topleft, frame_X_bottomright, frame_Y_bottomright;
		RECT frame_rect, image_rect;
		Gdiplus::Rect imageRect;
		double minDepth, maxDepth;

		PhotoPanel();
		~PhotoPanel();
		void Create(HWND hwnd_parent, IWICImagingFactory* gpIWICFactory, int x_topleft, int y_topleft, int x_bottomright, int y_bottomright);
		HRESULT Add(WCHAR* szFileName);
		void Remove();
		void Draw(HDC hdc);
};