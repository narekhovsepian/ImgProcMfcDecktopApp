#pragma once

///////////////////// mode = true ///////////////////////
// image is open		|	CreateCompatibleBitmap(W,H) image size changed |	Streatch	| Bitblt | n times call Delete Memory Context |
// Zoom + -				|	CreateCompatibleBitmap(W,H) image size changed |	Streatch	| Bitblt | n times call Delete Memory Context |											
// window resizng		|	CreateCompatibleBitmap(W,H) image size changed |	Streatch	| Bitblt | n times call Delete Memory Context |
// mouse move context	|				-----------                        |    --------    | BitBlt | __________________________________ |
// apply filter			|				-----------						   |	Streatch	| BitBlt | ---------------------------------- |


//////////////////// mode = false ////////////////////////
// image is open		|	CreateCompatibleBitmap(W,H) Viewer size changed |	Streatch	| Bitblt | 1 times call Allocate Memory Context |
// window resizng		|	CreateCompatibleBitmap(W,H) Viewer size changed	|	Streatch	| Bitblt | n times call Delete Memory Context 	|
// Zoom + -				|				-----------							|	Streatch	| Bitblt | ------------------------------------ |
// mouse move context	|				-----------							|   Streatch    | BitBlt | ------------------------------------ |
// apply filter			|				-----------							|	Streatch	| BitBlt | ------------------------------------ |


// image is open		| Call Function DeleteWindowsMemoryDC
// window resizng		| Call Function ResizingWindow										
// Zoom + -				| Call Function DeleteWindowsMemoryDC
// mouse move context	| Call Function UpdateMouseImagePos
// apply filter			| Call Function UpdateContext


/////////////// Drow mode /////////////////////////////////////

/////////////////// mode = true ////////////////////////////
//BOOL BitBlt(
//	[in] HDC   hdc,		| Viewer port HDC	
//	[in] int   x,		| _xpading 
//	[in] int   y,		| _ypading
//	[in] int   cx,		| _drawWidth
//	[in] int   cy,		| _drawHeight
//	[in] HDC   hdcSrc,	| image port HDC
//	[in] int   x1,		| 0
//	[in] int   y1,		| 0
//	[in] DWORD rop      | SRCCOPY
//);

/////////////////// mode = false ////////////////////////////
//BOOL BitBlt(
//	[in] HDC   hdc,		| Viewer port HDC	
//	[in] int   x,		| _xpading 
//	[in] int   y,		| _ypading
//	[in] int   cx,		| _drawWidth
//	[in] int   cy,		| _drawHeight
//	[in] HDC   hdcSrc,	| image port HDC
//	[in] int   x1,		| _xpading
//	[in] int   y1,		| _ypading
//	[in] DWORD rop      | SRCCOPY
//);


#define WidthMode 1820
#define HeightMode 1080
#define AreaMode (WidthMode * HeightMode)



// Read this paper 
// https://www.codeproject.com/Articles/224754/Guide-to-Win32-Memory-DC
class cMemoryDeviceContext
{
	// Ref to image Data
	uint8_t* _dataRef;

	// Ref ro image Info 
	BITMAPINFO* _bitmapInfoRef;

	// imaga Viewer SAFE HWND
	HWND _viewerHWND;

	//  Create Compatiblity DC
	HDC _hWindowMemoryHDC;

	// for Create Compatiblity DC
	HDC _hWindowDC;

	// Ref Bitmap Device memory
	HBITMAP _hBufferBitmapRef;

	HBITMAP _hOldBitmapRef;  // mono - chromatic 1x1 pixel bitmap.

	bool _mode{ true };

	bool _resizingWindow;

	bool _checkDeleteFunctionCall;

	bool _updateContext;

public:

	auto GetDeviceMemoryDC() const { return _hWindowMemoryHDC; }

	explicit cMemoryDeviceContext();

	explicit cMemoryDeviceContext(HWND viewerHWND, BITMAPINFO* bitmapInfo, uint8_t* data);

	explicit cMemoryDeviceContext(HWND viewerHWND, BITMAPINFO* bitmapInfo);

	virtual ~cMemoryDeviceContext();

	bool createWindowsMemoryDC(int width, int height, int xPading, int yPading);

	bool DeleteWindowsMemoryDC();

	void ResizingWindow();

	bool IsValidMemDC() const {
		return _hBufferBitmapRef;
	}

	void SetDataRef(uint8_t* data) { _dataRef = data; }

	void SetCurrentDrawProcessingMode(bool ch) {
		_mode = ch;
	}

	bool GetCurrentDrawProcessingMode() {
		return _mode;
	}

	bool CheckFunctionCall() {
		return _checkDeleteFunctionCall;
	}

	void UpdateMouseImagePos() {
		_checkDeleteFunctionCall = !_mode;
	}

	void UpdateContext() {
		_updateContext = true;
		_checkDeleteFunctionCall = true;
	}

	void setMode(int w, int h) {
		_mode = (w * h) > AreaMode ? false : true;
	}

};

