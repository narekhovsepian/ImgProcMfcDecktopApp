#pragma once

#include "xImage.h"
#include <cmath>
#include <numbers>
#include <vector>
#include <fstream>
#include <thread>

namespace utility {


	inline unsigned char saturate_cast(float v) { int iv = (int)std::roundf(v); return (unsigned char)((unsigned)iv <= UCHAR_MAX ? iv : iv > 0 ? UCHAR_MAX : 0); }
	inline unsigned char saturate_cast(double v) { int iv = (int)std::round(v); return (unsigned char)((unsigned)iv <= UCHAR_MAX ? iv : iv > 0 ? UCHAR_MAX : 0); }
	inline unsigned char saturate_cast(int v) { return (unsigned char)((unsigned)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }

	static const double atan2_p1 = 0.9997878412794807f * (double)(180 / std::numbers::pi);
	static const double atan2_p3 = -0.3258083974640975f * (double)(180 / std::numbers::pi);
	static const double atan2_p5 = 0.1555786518463281f * (double)(180 / std::numbers::pi);
	static const double atan2_p7 = -0.04432655554792128f * (double)(180 / std::numbers::pi);

	static inline double atan_f64(double y, double x);

	void ConvertToGray8_1Ch(xImage* Indata, xImage* outData);
	bool ConvertToGrayMultiThreadByLayer3Ch8_2_8Ch1(xImage* Indata, xImage* outData);
	void helpConvertToGrayMultiThreadByLayer8_1Ch(uint8_t* InData, uint8_t* outData,
		const int& rowBytesSrc, const int& rowBytesDst,
		const int& width, const int& lowHeight, const int& highHeight);

	// Split image in 3 channels outB outG outR
	bool Split(xImage* srcBGR, xImage* outB = nullptr, xImage* outG = nullptr, xImage* outR = nullptr);

	// Merge 3 colors out BGR image
	bool Merge(xImage* srcB, xImage* srcG, xImage* srcR, xImage* dstBGR);
	bool MergeMultiThreadByLayer8_1Ch_3Ch(xImage* srcB, xImage* srcG, xImage* srcR, xImage* dstBGR);
	void helpMergeMultiThreadByLayer8_1Ch_3Ch(uint8_t* srcB, uint8_t* srcG, uint8_t* srcR, uint8_t* dstBGR, const int& rowBytesSrc,
		const int& rowBytesDst, const int& width, const int& lowHeight, const int& highHeight);


	bool addWeighted1Ch(xImage* src1, xImage* src2, xImage* dst, float alpha, float beta, float gamma);
	void helpaddWeightedMultiThreadByLayer32int_1Ch_Out32double_1Ch(const uint8_t* src1,const uint8_t* src2, uint8_t* dst,
		const int& rowBytesSrc, const int& rowBytesDst, const int& width, const int& lowHeigh, const int& highHeight,
		const float& alpha, const float& beta, const float& gamma);
	bool addWeightedMultiThreadByLayer32int_1Ch_Out32double_1Ch(xImage* src1, xImage* src2, xImage* dst, 
		const float& alpha, const float& beta, const float& gamma);


	// Convert float32 to unsigned char
	// @param src type 32 bit float
	// @param dst type 8 bit uchar
	bool ConvertTo32F28U1Ch(xImage* src, xImage* dst);
	bool ConvertToMultiThreadByLayer32F28U1Ch(xImage* src, xImage* dst);
	void helpConvertToMultiThreadByLayer32F28U1Ch(const uint8_t* pSrc, uint8_t* pDst, const int rowBytesSrc, const int rowBytesDst,
		const int width, const int low, const int high);
	// Convert double 64 to unsigned char
	// @param src type 64 bit double 
	// @param dst type 8 bit uchar 
	bool ConvertTo64F28U1Ch(xImage* src, xImage* dst);

	// Convert int 64 to unsigned char
	// @param input src type 64 bit int 
	// @param output dst type 8 bit uchar
	bool ConvertTo64Int28U1Ch(xImage* src, xImage* dst);

	// convert short 16 to unsgined char
	// @param input src type 16 bit short
	// @param output dst type 8 bit uchar
	bool ConvertTo16S28U1Ch(xImage* src, xImage* dst);

	/// <summary>
	///  For Canny edge detection
	/// </summary>
	/// <param name="magnitude">float 32f</param>
	/// <param name="angle">float 32f</param>
	/// <param name="nonMaxSupp">float 32f</param>
	void nonMaximumSupression32F1ChAdv(xImage* magnitude, xImage* angle, xImage* nonMaxSupp);

	/// <summary>
	///  For Canny edge detection
	/// </summary>
	/// <param name="magnitude">float 32f</param>
	/// <param name="angle">float 32f</param>
	/// <param name="nonMaxSupp">float 32f</param>
	void nonMaximumSupression32F1Ch(xImage* magnitude, xImage* angle, xImage* nonMaxSupp);

	/// <summary>
	/// For Canny Edge Detection and Sobel
	/// </summary>
	/// <param name="src1">float 32F</param>
	/// <param name="src2">float 32F</param>
	/// <param name="dst">float 32F</param>
	void magnitude32F1Ch(xImage* src1, xImage* src2, xImage* dst);
	void helpMagnitudeMultiThreadByLayer32float_1Ch_Out32float_1Ch(const uint8_t* pSrc1,const uint8_t* pSrc2, uint8_t* pDst, const int& rowBytesSrc1,
		const int& rowBytesSrc2, const int& width, const int& lowHeight, const int& highHeight);
	bool magnitudeMultiThreadByLayer32float_1Ch_Out32float_1Ch(xImage* src1, xImage* src2, xImage* dst);


	/// <summary>
	/// For Canny edge detectiom
	/// </summary>
	/// <param name="src">float 32f</param>
	/// <param name="dst">uchar 8</param>
	/// <param name="minThrRatio">float 0.06 ratio</param>
	/// <param name="MaxThrRatio">float 0.09 ratio</param>
	void doubleThreshold1Ch(xImage* src, xImage* dst, float minThrRatio = 0.05, float MaxThrRatio = 0.09);

	/// <summary>
	/// For Canny Edge DeteCtion
	/// </summary>
	/// <param name="gardX">float 32F</param>
	/// <param name="gradY">float 32F</param>
	/// <param name="angle">float 32F</param>
	/// <returns></returns>
	bool angleFormedGradients32F1Ch(xImage* gardX, xImage* gradY, xImage* angle);


	// For CannY Edge Detection
	void edgeTrackingByHysteresis(xImage* src);

	bool range01(xImage* src, xImage* dst);

	bool range0255(xImage* src, xImage* dst);

	template<typename _Ty>
	bool writeData2File(xImage* src);

	void console();

	bool dataEqual(xImage* pInImage, xImage* pOutImage);


}



