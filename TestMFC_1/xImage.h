#pragma once

//#define UINT unsigned int
//#define uint8_t unsigned char

enum class COLOR_ORDER {
	_unknown = -1,
	_RGB = 0,
	_BGR,
	_RGBA,
	_ARGB,
	_MASK,
	_BYTE,
	_FLOAT,
	_DOUBLE,
	_SHORT,
	_INT
};

enum class Color_Space {
	BGR = 0,
	HSL,
	HSV,
	YCbCr
};


class xImage
{
	int _Width;
	int _Height;
	int _RowBytes;
	int _Bpp;
	int _alignment = 4;
	uint8_t* _pDataGen;

	COLOR_ORDER _colorOrder;
	bool _isTopDown;
	Color_Space _currentColorSpace;
public:

	virtual ~xImage();

	explicit xImage(const int Width, const int Height, const int RowBytes, const int bpp, const COLOR_ORDER color_order, uint8_t* imageData);

	xImage();

	explicit xImage(const xImage& other);


	// Copy constctuctor Color convert current color order 
	explicit xImage(const xImage& other, COLOR_ORDER colorConvert);

	auto IsValid() const { return (_pDataGen != nullptr && _Width > 0 && _Height > 0 && _RowBytes >= _Width && _Bpp > 0); }

	auto SetData(uint8_t* inData) { _pDataGen = inData ? inData : nullptr; }
	auto SetWidth(int w) { _Width = w > 0 ? w : 0; }
	auto SetHeigh(int h) { _Height = h > 0 ? h : 0; }
	auto SetRowBytes(int rb) { _RowBytes = rb > 0 ? rb : 0; }
	auto SetBpp(int bpp) { _Bpp = bpp > 0 ? bpp : 0; }
	auto SetColorOrder(COLOR_ORDER corder) { _colorOrder = corder; }


	auto GetWidth() const { return _Width; }
	auto GetHeight() const { return _Height; }
	auto GetRowBytes() const { return _RowBytes; }
	auto GetBpp() const { return _Bpp; }
	auto GetOrder() const { return _colorOrder; }


	inline auto isTopDown() const { return _isTopDown; }

	auto GetData() const {
		return _pDataGen;
	}

	void allocateSpace(int width, int height, COLOR_ORDER colorOrder);

	void deAllocateSpace();
};

