#include "pch.h"
#include "ImageViewer.h"
#include "resource.h"
#include <cmath>



auto CImageViewer::UpdatePreview() -> bool {

	if (_pOriginalImageRef != nullptr || _pTransformedImageRef != nullptr) {
		CRect rectViewer;
		this->GetWindowRect(&rectViewer);
		this->GetParent()->ScreenToClient(&rectViewer);

		float widthfactor{};
		float heightfactor{};
		int originalimagewidth = _pOriginalImageRef->GetWidth();
		int originalimageheight = _pOriginalImageRef->GetHeight();
		int rectViewerHeight = rectViewer.Height();
		int rectViewerWidth = rectViewer.Width();



		if (_mouseMoveImage == false) {
			if (_calcByPercentWidthHeight == false) {

				if ((rectViewerWidth < originalimagewidth) || (rectViewerHeight < originalimageheight)) {

					widthfactor = rectViewerWidth / (float)originalimagewidth;
					heightfactor = rectViewerHeight / (float)originalimageheight;

#if 0
					float fFactor = min(rectViewerWidth, rectViewerHeight) / (float)max(originalimageheight, originalimagewidth);
#else
					float fFactor = min(widthfactor, heightfactor);
#endif

					_drawWidth = int(originalimagewidth * fFactor);
					_drawHeight = int(originalimageheight * fFactor);

					_xPadding = ((rectViewerWidth - _drawWidth) / 2);
					_yPadding = ((rectViewerHeight - _drawHeight) / 2);

					_xyPadingbackup = { _xPadding,_yPadding };

					//TRACE(L"\ndrawWidth %d drawHeight %d xpading %d ypading %d\n", _drawWidth, _drawHeight, _xPadding, _yPadding);

				}
				else {		//if (rectViewer.Width() > originalimagewidth && rectViewer.Height() > originalimageheight) {

					_xPadding = (rectViewerWidth - originalimagewidth) / 2;
					_yPadding = (rectViewerHeight - originalimageheight) / 2;
					_drawWidth = originalimagewidth;
					_drawHeight = originalimageheight;
					_xyPadingbackup = { _xPadding,_yPadding };

				}
				_CalcCurrPercent();
			}
			else {

				CPoint tmpPading = _xyPadingbackup;

				float oldXpadding = (float)_xPadding;
				float oldYpadding = (float)_yPadding;


				_drawWidth = int((originalimagewidth * _fCurrentPercent) / 100);
				_drawHeight = int((originalimageheight * _fCurrentPercent) / 100);


				float newcenterXpadding = ((-1 * _drawWidth + rectViewerWidth) / 2.0f);
				float newcenterYPadding = ((-1 * _drawHeight + rectViewerHeight) / 2.0f);

				_xyPadingbackup = { (int)newcenterXpadding,(int)newcenterYPadding };


				float zoomDelta = (_fCurrentPercent / (_fOldPercent == 0 ? 1 : _fOldPercent));

				_xPadding = (int)newcenterXpadding != 0 ? int(zoomDelta * (oldXpadding - tmpPading.x) + newcenterXpadding) : 0;
				_yPadding = (int)newcenterYPadding != 0 ? int(zoomDelta * (oldYpadding - tmpPading.y) + newcenterYPadding) : 0;


				if (_drawWidth > rectViewerWidth)
					_xPadding = _xPadding > 0 ? 0 : (std::abs(_xPadding) > 2 * std::abs(newcenterXpadding)) ? int(2 * newcenterXpadding) : _xPadding;
				else
					_xPadding = (int)newcenterXpadding;

				if (_drawHeight > rectViewerHeight)
					_yPadding = _yPadding > 0 ? 0 : (std::abs(_yPadding) > 2 * std::abs(newcenterYPadding)) ? int(2 * newcenterYPadding) : _yPadding;
				else
					_yPadding = (int)newcenterYPadding;
			
				return true;
			}
		}
		else {

			int xDifference{};
			int yDifference{};
			int xNewPading{};
			int yNewPading{};
			CPoint _xyClickedDifLaststatus = _xyCodinateClickedDiferenceLastStatus;

			xDifference = _xyMouseMovesCordinates.x - _xyMouseClickedCordinatesBackup.x;
			yDifference = _xyMouseMovesCordinates.y - _xyMouseClickedCordinatesBackup.y;

			_xyCodinateClickedDiferenceLastStatus = { xDifference,yDifference };

			xDifference -= _xyClickedDifLaststatus.x;
			yDifference -= _xyClickedDifLaststatus.y;

			xNewPading = xDifference + _xPadding;
			yNewPading = yDifference + _yPadding;

			if (xNewPading > 0)
			{
				xNewPading = 0;
			}
			if (yNewPading > 0)
			{
				yNewPading = 0;
			}
			////////////////////////////////////////////////////////////////////////////////
			if ((_xyPadingbackup.x < 0) && (xNewPading < (_xyPadingbackup.x * 2 - 1))) {
				xNewPading = _xyPadingbackup.x * 2 - 1;
			}


			if ((_xyPadingbackup.y < 0) && (yNewPading < (_xyPadingbackup.y * 2 - 1))) {
				yNewPading = _xyPadingbackup.y * 2 - 1;
			}

			/////////////////////////////////////////////////////////////////////////////////

			if (_drawHeight < rectViewerHeight) {
				yNewPading = _yPadding;
			}

			if (_drawWidth < rectViewerWidth) {
				xNewPading = _xPadding;
			}

			///////////////////////////////////////////////////////////////////////////////

			_xPadding = xNewPading;
			_yPadding = yNewPading;
			TRACE(L"\n Mouse drawWidth %d drawHeight %d xpading %d ypading %d\n", _drawWidth, _drawHeight, _xPadding, _yPadding);

		}
		return true;

	}
	return false;
}


CImageViewer::~CImageViewer()
{


}

auto CImageViewer::Initialize() -> bool
{
	// Create Context for transformed and original image 
	_pOriginalImageDMC = std::make_unique<cMemoryDeviceContext>(GetSafeHwnd(), _bitmapInfoRef);

	_pTransformedImageDMC = std::make_unique<cMemoryDeviceContext>(GetSafeHwnd(), _bitmapInfoRef);

	_pCurrentImageDMCRef = _pTransformedImageDMC.get();
	///////////////////////

	return true;
}


auto CImageViewer::DrawImage() -> bool const
{
	HWND imageViewerHWND = GetSafeHwnd();
	HDC hWindowDC = ::GetWindowDC(imageViewerHWND);

	int LxPading{};
	int LyPading{};

	int rbitBlt{};

	if (_pTransformedImageDMC->CheckFunctionCall()) {
		_pTransformedImageDMC->createWindowsMemoryDC(_drawWidth, _drawHeight, (int)_xPadding, (int)_yPadding);

		TRACE("\nCall TRANSformed\n");
	}


	if (_pOriginalImageDMC->CheckFunctionCall()) {
		_pOriginalImageDMC->createWindowsMemoryDC(_drawWidth, _drawHeight, (int)_xPadding, (int)_yPadding);

		TRACE("\nCall Original\n");
	}

	if (_pOriginalImageDMC->GetCurrentDrawProcessingMode()) {
		LyPading = 0;
		LxPading = 0;
	}
	else {
		LxPading = (int)_xPadding;
		LyPading = (int)_yPadding;
	}


	//::SetStretchBltMode(hWindowDC, COLORONCOLOR);

	rbitBlt = ::BitBlt(hWindowDC, (int)_xPadding, (int)_yPadding, _drawWidth, _drawHeight, _pCurrentImageDMCRef->GetDeviceMemoryDC(), LxPading, LyPading, SRCCOPY);


	if (rbitBlt == 0) {
		TRACE("Cannot draw BitBlt\n");
		return 0;
	}

	return true;
}

auto CImageViewer::UpdatePos() -> void {

	RECT dlgRect, viewRect;

	this->GetParent()->GetWindowRect(&dlgRect);
	this->GetWindowRect(&viewRect);

	this->GetParent()->ScreenToClient(&dlgRect);
	this->GetParent()->ScreenToClient(&viewRect);


	int rDiff = dlgRect.right - viewRect.right;
	int bDiff = dlgRect.bottom - viewRect.bottom;
	viewRect.right = viewRect.right + rDiff - _viewerRightPadding;
	viewRect.bottom = viewRect.bottom + bDiff - _viewerBottomPadding;

	this->MoveWindow(&viewRect);
	this->GetParent()->RedrawWindow();
}




auto CImageViewer::_CalcCurrPercent(bool zoomPercentCalc, bool zoomPercentDownUp) -> void
{
	if (_pOriginalImageRef != nullptr)
	{
		if (zoomPercentCalc)
		{
			_fOldPercent = _fCurrentPercent;
			_fCurrentPercent = (_drawWidth * 100.0f / _pOriginalImageRef->GetWidth());
			_zoomPercentIndex = 0;

		}
		else {

			if (zoomPercentDownUp) {
				for (std::size_t i{}; i != _zoomPercent.size(); ++i)
				{
					if ((_fCurrentPercent < _zoomPercent[i])) {
						_zoomPercentIndex = i;
						break;
					}
				}
			}
			else {
				for (std::size_t i = _zoomPercent.size() - 1; i != -1; --i)
				{
					if ((_fCurrentPercent > _zoomPercent[i])) {
						_zoomPercentIndex = i;
						break;
					}
				}
			}

			//_zoomPercentIndex = _zoomPercent.size() == _zoomPercentIndex ? _zoomPercentIndex - 1 : _zoomPercentIndex;
			_fOldPercent = _fCurrentPercent;
			_fCurrentPercent = float(_zoomPercent[_zoomPercentIndex]);

		}

	}
}
