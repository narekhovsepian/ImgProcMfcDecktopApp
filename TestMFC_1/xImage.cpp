#include "pch.h"

#include "xDefines.h"
#include "xImage.h"
#include <memory>



xImage::xImage() :
	_Width{ 0 },
	_Height{ 0 },
	_RowBytes{ 0 },
	_Bpp{ 0 },
	_colorOrder{ COLOR_ORDER::_unknown },
	_isTopDown{ false },
	_pDataGen{ nullptr }
{ }


xImage::xImage(const int Width, const int Height, const int RowBytes, const int bpp, const COLOR_ORDER color_order, uint8_t* imageData) :
	_Width{ Width },
	_Height{ Height },
	_Bpp{ bpp },
	_colorOrder{ color_order },
	_pDataGen{ nullptr },
	_isTopDown{ false }
{

	if (Width > 0 && Height > 0 && RowBytes >= Width && bpp > 0 && imageData != nullptr) {


		_RowBytes = RowBytes;
		_pDataGen = new uint8_t[Height * _RowBytes]{};
		std::memcpy(_pDataGen, imageData, Height * _RowBytes);

	}

}


xImage::~xImage() {
	SAFE_DELETE_PTR(_pDataGen);
}

xImage::xImage(const xImage& other) :
	_pDataGen{ nullptr }
{
	if (this != &other && other.IsValid()) {

		_Width = other._Width;
		_Height = other._Height;
		_RowBytes = other._RowBytes;
		_Bpp = other._Bpp;
		_colorOrder = other._colorOrder;
		_isTopDown = other._isTopDown;

		_pDataGen = new uint8_t[_Height * _RowBytes]{};

		std::memcpy(_pDataGen, other._pDataGen, (_Height * _RowBytes));
	}
}



void xImage::allocateSpace(int width, int height, COLOR_ORDER colorOrder) {

	if (width <= 0 || height <= 0 || colorOrder == COLOR_ORDER::_unknown) {
		return;
	}

	_colorOrder = colorOrder;
	_Width = width;
	_Height = height;

	if (colorOrder == COLOR_ORDER::_BGR || colorOrder == COLOR_ORDER::_RGB) {
		_Bpp = 24;
	}
	else if (colorOrder == COLOR_ORDER::_BYTE) {
		_Bpp = 8;
	}
	else if (colorOrder == COLOR_ORDER::_FLOAT) {
		_Bpp = 32;
	}
	else if (colorOrder == COLOR_ORDER::_INT) {
		_Bpp = 32;
	}
	else if (colorOrder == COLOR_ORDER::_DOUBLE) {
		_Bpp = 64;
	}
	else if (colorOrder == COLOR_ORDER::_SHORT) {
		_Bpp = 16;
	}

	int padding = (_alignment - ((width * _Bpp / 8) % _alignment)) % _alignment;
	_RowBytes = width * (_Bpp / 8) + padding;

	_pDataGen = new uint8_t[_Height * _RowBytes];
}



void xImage::deAllocateSpace() {

	if (IsValid()) {

		SAFE_DELETE_PTR(_pDataGen);
		_Width = 0;
		_Height = 0;
		_RowBytes = 0;
		_Bpp = 0;
		_isTopDown = false;
		_colorOrder = COLOR_ORDER::_unknown;
	}
}




