#include "pch.h"
#include "cMemoryDeviceContext.h"


bool cMemoryDeviceContext::DeleteWindowsMemoryDC() {

	if ((_hBufferBitmapRef != nullptr && _mode) || _resizingWindow) {

		int rDO{};

		auto x = ::SelectObject(_hWindowMemoryHDC, _hOldBitmapRef);

		rDO = ::DeleteObject(_hBufferBitmapRef);

		_hBufferBitmapRef = nullptr;

		if (!rDO) {
			TRACE(L"Cannot Delte Object\n");
			return false;
		}

	}
	_checkDeleteFunctionCall = true;
	return true;
}


bool cMemoryDeviceContext::createWindowsMemoryDC(int Width, int Height, int xPading, int yPading)
{
	if (Width > 0 && Height > 0 && _bitmapInfoRef != nullptr && _dataRef != nullptr)
	{
		int LWidth{};
		int LHeight{};
		int LxPading{};
		int LyPading{};

		if ((((bool(_hBufferBitmapRef) & !_mode) ^ true) && !_updateContext) || _resizingWindow) {

			if (_mode == false) {
				CRect rect;
				::GetWindowRect(_viewerHWND, &rect);

				LWidth = rect.Width();
				LHeight = rect.Height();
			}
			else {
				LWidth = Width;
				LHeight = Height;
			}

			_hBufferBitmapRef = ::CreateCompatibleBitmap(_hWindowDC, LWidth, LHeight);
			_hOldBitmapRef = (HBITMAP)::SelectObject(_hWindowMemoryHDC, _hBufferBitmapRef);

			// For Resizing window
			_resizingWindow = false;
		}

		if (_mode)
		{
			LxPading = 0;
			LyPading = 0;
		}
		else {
			LxPading = xPading;
			LyPading = yPading;
		}

		// HALFTONE  is correct show image 
		::SetStretchBltMode(_hWindowMemoryHDC, HALFTONE); //COLORONCOLOR

		int rStrV = ::StretchDIBits(_hWindowMemoryHDC, LxPading, LyPading,
			Width, Height,
			0, 0,
			_bitmapInfoRef->bmiHeader.biWidth, -1 * _bitmapInfoRef->bmiHeader.biHeight,
			_dataRef,
			_bitmapInfoRef,
			DIB_RGB_COLORS,
			SRCCOPY);

		if (rStrV == 0) {

			TRACE(L"Cannot Stretch Image\n\n");
			return false;
		}
		_updateContext = false;
		_checkDeleteFunctionCall = false;
		return true;

	}
	return false;
}

void cMemoryDeviceContext::ResizingWindow() {

	_resizingWindow = true;
	DeleteWindowsMemoryDC();
}


cMemoryDeviceContext::~cMemoryDeviceContext() {

	if (_hBufferBitmapRef != nullptr) {

		auto x = ::SelectObject(_hWindowMemoryHDC, _hOldBitmapRef);

		if (::DeleteDC(_hWindowMemoryHDC) == 0) {
			TRACE(L"Cannot Delete Device Memory Context");

		}

		if (::DeleteObject(_hBufferBitmapRef) == 0) {
			TRACE(L"Cannot Delete Buffer Bitmap\n");
		}
	}
	else {
		if (::DeleteDC(_hWindowMemoryHDC) == 0) {
			TRACE(L"Cannot Delete Device Memory Context");
		}
	}

	_dataRef = nullptr;

}

cMemoryDeviceContext::cMemoryDeviceContext()
	: _viewerHWND{ nullptr },
	_hWindowMemoryHDC{ nullptr },
	_bitmapInfoRef{ nullptr },
	_dataRef{ nullptr },
	_hWindowDC{ nullptr },
	_hBufferBitmapRef{ nullptr },
	_hOldBitmapRef{ nullptr },
	_checkDeleteFunctionCall{ false },
	_resizingWindow{ false },
	_updateContext{ false }

{ }


cMemoryDeviceContext::cMemoryDeviceContext(HWND viewerHWND, BITMAPINFO* bitmapInfo, uint8_t* data)
	: _viewerHWND{ viewerHWND },
	_bitmapInfoRef{ bitmapInfo },
	_dataRef{ data },
	_hWindowMemoryHDC{ nullptr },
	_hWindowDC{ nullptr },
	_hBufferBitmapRef{ nullptr },
	_hOldBitmapRef{ nullptr },
	_checkDeleteFunctionCall{ false },
	_resizingWindow{ false },
	_updateContext{ false }
{
	_hWindowDC = ::GetWindowDC(_viewerHWND);

	_hWindowMemoryHDC = ::CreateCompatibleDC(_hWindowDC);
}

cMemoryDeviceContext::cMemoryDeviceContext(HWND viewerHWND, BITMAPINFO* bitmapInfo)
	: _viewerHWND{ viewerHWND },
	_bitmapInfoRef{ bitmapInfo },
	_dataRef{ nullptr },
	_hWindowMemoryHDC{ nullptr },
	_hWindowDC{ nullptr },
	_hBufferBitmapRef{ nullptr },
	_hOldBitmapRef{ nullptr },
	_checkDeleteFunctionCall{ false },
	_resizingWindow{ false },
	_updateContext{ false }
{
	_hWindowDC = ::GetWindowDC(_viewerHWND);

	_hWindowMemoryHDC = ::CreateCompatibleDC(_hWindowDC);

	//_allocateCompatibleBitmap();
}

