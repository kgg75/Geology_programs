#include "StdAfx.h"
#include "Photo.h"

Photo::Photo(IWICImagingFactory* pIWICFactory) : m_pIWICFactory(pIWICFactory) {
}

Photo::~Photo() {
}

HRESULT Photo::Create(WCHAR* szFileName) {
    HRESULT hr = S_OK;
    _fileName = (std::wstring)szFileName; //TODO

    //WCHAR szFileName[MAX_PATH];

    // Step 1: Create the open dialog box and locate the image file
    //if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
    {
        // Step 2: Decode the source image to IWICBitmapSource

        // TODO #1 - open UV-image
        // функции работы с именем файла

        // TODO #2 - open jgw-files for both

        // Create a decoder
        IWICBitmapDecoder* pDecoder = NULL;

        //IWICImagingFactory*  pIF;

        hr = m_pIWICFactory->CreateDecoderFromFilename(
            szFileName,                      // Image to be decoded
            NULL,                            // Do not prefer a particular vendor
            GENERIC_READ,                    // Desired read access to the file
            WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
            &pDecoder                        // Pointer to the decoder
        );

        // Retrieve the first frame of the image from the decoder
        IWICBitmapFrameDecode* pFrame = NULL;

        if (SUCCEEDED(hr)) {
            hr = pDecoder->GetFrame(0, &pFrame);
        }

        // Retrieve IWICBitmapSource from the frame
        // m_pOriginalBitmapSource contains the original bitmap and acts as an intermediate
        if (SUCCEEDED(hr)) {
            SafeRelease(&m_pOriginalBitmapSource);
            hr = pFrame->QueryInterface(
                IID_IWICBitmapSource,
                reinterpret_cast<void**>(&m_pOriginalBitmapSource));
        }

        // Step 3: Scale the original IWICBitmapSource to the client rect size and convert the pixel format
        IWICBitmapSource* pToRenderBitmapSource = NULL;

        if (SUCCEEDED(hr)) {
            //hr = ConvertBitmapSource(hWnd, &pToRenderBitmapSource);

            pToRenderBitmapSource = NULL;

            // Get the client Rect - зачем ??
            //RECT rcClient;
            //hr = GetClientRect(hWnd, &rcClient) ? S_OK : E_FAIL;

            if (SUCCEEDED(hr)) {
                // Format convert the bitmap into 32bppBGR, a convenient pixel format for GDI+ rendering 
                if (SUCCEEDED(hr)) {
                    IWICFormatConverter* pConverter = NULL;

                    hr = m_pIWICFactory->CreateFormatConverter(&pConverter);

                    // Format convert to 32bppBGR 
                    if (SUCCEEDED(hr)) {
                        hr = pConverter->Initialize(
                            //pScaler,                         // Input bitmap to convert
                            m_pOriginalBitmapSource,    // вместо pScaler
                            GUID_WICPixelFormat32bppBGR,     // Destination pixel format
                            WICBitmapDitherTypeNone,         // Specified dither patterm
                            NULL,                            // Specify a particular palette 
                            0.f,                             // Alpha threshold
                            WICBitmapPaletteTypeCustom       // Palette translation type
                        );

                        // Store the converted bitmap as ppToRenderBitmapSource 
                        if (SUCCEEDED(hr)) {
                            hr = pConverter->QueryInterface(IID_PPV_ARGS(&pToRenderBitmapSource));
                        }
                    }

                    SafeRelease(&pConverter);
                }

                //SafeRelease(&pScaler);
            }

        }

        // Step 4: Create a DIB from the converted IWICBitmapSource
        if (SUCCEEDED(hr)) {
            //hr = CreateDIBSectionFromBitmapSource(pToRenderBitmapSource);

            // Check BitmapSource format
            WICPixelFormatGUID pixelFormat;
            hr = pToRenderBitmapSource->GetPixelFormat(&pixelFormat);

            if (SUCCEEDED(hr)) {
                hr = (pixelFormat == GUID_WICPixelFormat32bppBGR) ? S_OK : E_FAIL;
            }

            if (SUCCEEDED(hr)) {
                hr = pToRenderBitmapSource->GetSize(&width, &height);
            }

            UINT cbStride = 0;
            if (SUCCEEDED(hr)) {
                // Size of a scan line represented in bytes: 4 bytes each pixel
                hr = UIntMult(width, sizeof(Gdiplus::ARGB), &cbStride);
            }

            UINT cbBufferSize = 0;
            if (SUCCEEDED(hr)) {
                // Size of the image, represented in bytes
                hr = UIntMult(cbStride, height, &cbBufferSize);
            }

            if (SUCCEEDED(hr)) {
                // Make sure to free previously allocated buffer and bitmap
                //DeleteBufferAndBmp();
                delete m_pGdiPlusBitmap;
                m_pGdiPlusBitmap = NULL;

                delete[] m_pbBuffer;
                m_pbBuffer = NULL;


                m_pbBuffer = new BYTE[cbBufferSize];

                hr = (m_pbBuffer) ? S_OK : E_FAIL;

                if (SUCCEEDED(hr)) {
                    WICRect wic_rect = { 0, 0, width, height };

                    // Extract the image into the GDI+ Bitmap
                    hr = pToRenderBitmapSource->CopyPixels(
                        &wic_rect,
                        cbStride,
                        cbBufferSize,
                        m_pbBuffer
                    );

                    if (SUCCEEDED(hr)) {
                        m_pGdiPlusBitmap = new Gdiplus::Bitmap(
                            width,
                            height,
                            cbStride,
                            PixelFormat32bppRGB,
                            m_pbBuffer
                        );

                        hr = m_pGdiPlusBitmap ? S_OK : E_FAIL;

                        if (SUCCEEDED(hr)) {
                            if (m_pGdiPlusBitmap->GetLastStatus() != Gdiplus::Ok) {
                                hr = E_FAIL;
                            }
                        }
                    }

                    if (FAILED(hr)) {
                        //DeleteBufferAndBmp();
                        delete m_pGdiPlusBitmap;
                        m_pGdiPlusBitmap = NULL;

                        delete[] m_pbBuffer;
                        m_pbBuffer = NULL;
                    }
                }
            }
        }
        SafeRelease(&pToRenderBitmapSource);
        SafeRelease(&pDecoder);
        SafeRelease(&pFrame);
    }

    return hr;
}

HRESULT Photo::SetCoords(WCHAR* szFileName) {
    HRESULT hr = S_OK; 
    int index = 0;

    std::fstream fs;
    fs.open(szFileName, std::ios::in);  // читаем jgw
    if (fs.is_open()) {
        fs >> x_scale;
        fs >> y_scale;  // пустое чтение; параметр не используется
        fs >> y_scale;  // пустое чтение; параметр не используется
        fs >> y_scale;
        fs >> X_left;
        fs >> Y_top;
        fs.close();
        X_right = X_left + width * x_scale;
        Y_bottom = Y_top + height * y_scale;
    }
    else {
        MessageBox(NULL, L"Ошибка при чтении файла координат.", L"Error", NULL);
        hr = E_FAIL;
    }

    return hr;
}