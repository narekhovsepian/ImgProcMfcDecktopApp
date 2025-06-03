#pragma once
#include <afxwin.h>
#include <utility>
#include <vector>
#include <memory>
#include "xImage.h"
#include "cMemoryDeviceContext.h"

#define SAFE_DELETE_OBJ(obj) {if(obj!=nullptr){delete obj;obj=nullptr;}}



template<typename _Ty1, typename _Ty2>
struct TCPoint {
	_Ty1 x;
	_Ty2 y;

	constexpr TCPoint() :
		x{},
		y{}
	{ }

	constexpr  TCPoint(const _Ty1 x, const _Ty2 y) :
		x{ x },
		y{ y }
	{ }

	constexpr  TCPoint(const CPoint p) :
		x{ (_Ty1)p.x },
		y{ (_Ty2)p.y }
	{ }

	constexpr TCPoint<_Ty1, _Ty2>(const TCPoint<_Ty1, _Ty2>& other) {
		x = other.x;
		y = other.y;
	}

	constexpr TCPoint<_Ty1, _Ty2>& operator=(const TCPoint<_Ty1, _Ty2>& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}

	TCPoint<_Ty1, _Ty2>& operator=(TCPoint<_Ty1, _Ty2>&& right) noexcept
	{

		x = std::forward<_Ty1>(right.x);
		y = std::forward<_Ty2>(right.y);
		return *this;
	}

};


class CImageViewer : public CStatic
{

	typedef TCPoint<float, float> fCPoint;
public:
	CImageViewer() = default;
	virtual ~CImageViewer();

	auto Initialize() -> bool;

	auto UpdatePreview() -> bool;
	auto DrawImage() -> bool const;

	auto SetOrignalTransformedImageDataRef(xImage* pImageOriginal, xImage* pImageTransformed) {
		_pOriginalImageRef = pImageOriginal;
		_pTransformedImageRef = pImageTransformed;
		_pCurrentImageRef = _pTransformedImageRef;
	}

	auto GetTransformedDMCRef() const { return _pTransformedImageDMC.get(); }
	auto GetOriginalDMCRef() const { return _pOriginalImageDMC.get(); }

	auto GetCurrentImageRef() const { return _pCurrentImageRef; }
	auto UpdatePos() -> void;

	auto GetCurrentPercent() const { return _fCurrentPercent; }

	void SetViewerRightPadding(int x) { _viewerRightPadding = x; }
	void SetViewerBottomPadding(int y) { _viewerBottomPadding = y; }
	int GetViewerRightPadding() const { return _viewerRightPadding; }
	int GetViewerBottomPadding() const { return _viewerBottomPadding; }
	auto _CalcCurrPercent(bool _zoomPercentcalc = true, bool _zoomPercentDownUp = true) -> void;
	//
	void SetZoomPercentAndIndex(int i) {
		if (i - 1 < _zoomPercent.size()) {
			_fOldPercent = _fCurrentPercent;
			_zoomPercentIndex = i;
			_fCurrentPercent = (float)_zoomPercent[i - 1];
		}
	}
	void SetCalcByPercentWidthHeight(bool calcbypercent) { _calcByPercentWidthHeight = calcbypercent; }
	std::size_t GetZoomPercentIndex() const { return _zoomPercentIndex; }
	auto GetXYPadingImage() { return CPoint{ _xPadding, _yPadding }; }
	//auto GetXYPadingImage() { return CPoint( _xPadding, _yPadding ); }
	void SetXYPadingImageBackup(const CPoint& pading) { _xyPadingbackup = pading; }
	auto GetXYPadingImageBackup() const { return _xyPadingbackup; }

	void SetXYMouseMoveStatus(bool ch) { _xyMouseMoveStatus = ch; }
	bool GetXYMouseMoveStatus()const { return _xyMouseMoveStatus; }
	void SetXYCordinateClickedDiferenceLastStatus(const CPoint& dif) { _xyCodinateClickedDiferenceLastStatus = dif; }
	auto GetXYCordinateClickedDiferenceLastStatus() const { return _xyCodinateClickedDiferenceLastStatus; }
	void SetXYMouseClickedCordinates(const CPoint& point) { _xyMouseClickedCordinates = point; }
	auto GetXYMouseClickedCordinates() { return _xyMouseClickedCordinates; }
	void SetXYMouseMoveCordinates(const CPoint& point) { _xyMouseMovesCordinates = point; }
	void SetMouseMoveImage(bool ch) { _mouseMoveImage = ch; }
	bool GetMouseMoveImage() const { return _mouseMoveImage; }
	void SetXYMouseClickedCordinatesBackup(CPoint& point) { _xyMouseClickedCordinatesBackup = point; }


	auto OriginalOrTransformedImageMouseRightClick(bool ch) {
		// ch == true originale Image  ref or ch == false transformaed image ref
		_pCurrentImageRef = ch ? _pOriginalImageRef : _pTransformedImageRef;
		_pCurrentImageDMCRef = ch ? _pOriginalImageDMC.get() : _pTransformedImageDMC.get();

	}

	void SetBitmapInfoRef(BITMAPINFO* info) { _bitmapInfoRef = info; }

	POINT GetZoomedWidthHeight() { return { _drawWidth,_drawHeight }; }

	void CalculateMouseWhellZoomFactor(short zDelta);

	const std::vector<int>_zoomPercent{ 10, 25, 50, 75, 100, 150, 200, 400 }; //{ 100, 110, 121, 133, 146, 161, 177, 194, 214, 235, 259 };

	 

protected:

	int _xPadding = 0;
	int _yPadding = 0;
	int _drawWidth = 0;
	int _drawHeight = 0;
	int _viewerRightPadding{};
	int _viewerBottomPadding{};
	float _fCurrentPercent = 0.0f;
	float _fOldPercent = 0.0f;
	float _fCenterXPadding{};
	float _fCenterYPadding{};
	bool _xyMouseMoveStatus{ false };
	// I 
	float _fPercentCalc = 0.0f;
	std::size_t _zoomPercentIndex{};
	bool _calcByPercentWidthHeight{};
	bool _mouseMoveImage{};

	CPoint _xyPadingbackup{ 0,0 };
	CPoint _xyMouseClickedCordinates{ 0,0 };
	CPoint _xyMouseClickedCordinatesBackup{ 0,0 };
	CPoint _xyCodinateClickedDiferenceLastStatus{ 0,0 };
	CPoint _xyMouseMovesCordinates{ 0,0 };

	xImage* _pOriginalImageRef = nullptr;
	xImage* _pTransformedImageRef = nullptr;
	xImage* _pCurrentImageRef = nullptr;

	cMemoryDeviceContext* _pCurrentImageDMCRef = nullptr;

	std::unique_ptr<cMemoryDeviceContext> _pOriginalImageDMC{ nullptr };
	std::unique_ptr<cMemoryDeviceContext> _pTransformedImageDMC{ nullptr };

	BITMAPINFO* _bitmapInfoRef{ nullptr };

	///////////////////
	bool _mouseWhell{ false };
	CPoint _xyMousePosition{ 0,0 };
	std::vector<int> whellpercent{ 100,110,121,133,146,161,177,194,214,235,259 };
	int _i{ 0 };

};

