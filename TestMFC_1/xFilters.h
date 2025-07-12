#pragma once
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <chrono>
#include <numbers>
#include "xImage.h"
#include "utility.h"
#include <thread>
#include <mutex>

//#define CL_TARGET_OPENCL_VERSION 300
#define CL_TARGET_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#include <CL/cl2.hpp>
//#include <CL/opencl.hpp>
#include <CL/cl.h>




class xImage;

namespace filter {
	enum class FilterNames {
		_None = 0,
		_Grayscale,
		_BlackAndWhite,
		_OnlyRed,
		_OnlyGreen,
		_OnlyBlue,
		_GraycaleNegative,
		_B_W_Negative,
		_Negative,
		_Contrast,
		_Brightness,
		_Gamma,
		_GaussianBlurClassic,
		_GaussianBlurSeperableConvolutionOpencvKernel,
		_RecursiveGaussianBlurVliet,
		_HSVColorSpace,
		_HSLColorSpace,
		_YCbCrColorSpace,
		_YUVColorSpace,
		_SobelEdgeDetection,
		_PrewittEdgeDetection,
		_CannyEdgeDetection,
		_test
	};

	const static std::vector<std::wstring> StrNames{
		_T("None"),
		_T("Grayscale"),
		_T("Black And White"),
		_T("Only Red"),
		_T("Only Green"),
		_T("Only Blue"),
		_T("Graycale Negative"),
		_T("Black and White Negative"),
		_T("Negative"),
		_T("Contrast"),
		_T("Brightness"),
		_T("Gamma"),
		_T("GaussianBlur Classic"),
		_T("GaussianBlurSep.Conv.(Op.cvK)"),
		_T("RecursiveGaussianBlurVliet"),
		_T("HSV Color Space"),
		_T("HSL Color Space"),
		_T("YCbCr Color Space"),
		_T("YUV Color Space"),
		_T("Sobel Edge Detection"),
		_T("Prewitt Edge Detection"),
		_T("Canny Edge Detection"),
		_T("test")
	};


	//////////////
	inline int ConvertPercentToValue255(int x) { return (255 * x / 100); }
	inline float ConvertPercentToValue4(int x) { return x == 0 ? 1 : x < 0 ? ((100 + x) / 100.0f) : 1 + (3.0f * x) / 100.0f; }
	inline float Divide100(int x) { return x / 100.0f; }

	/////////////

	// Gaussian blur classic									min value = positive primen number, max = 2 * 100 + (min - 2) //  infiniti
	// Recursive Gaussian blur Vliet							min = 0.5 and max = infiniti positive nunmber
	// Gaussian Blur Seperable Convolution with Opencv Kernel   min value = positive primen number, max = 2 * 100 + (min - 2) //  infiniti
	// Black and white											min = 0 ans max = 255 positive number 
	// _B_W_Negative											min = 0 max = 255 positive number 
	// Contrast													min = 0 max = 255 positive number
	// Brightness												min = no value max = 255 positive number  -50 to 50 points
	// Gamma													min = 100 for divide [0.99.... 0.1] points value max = 4 - 1 [1....4] positive number no input number


	// sliderValue current point
	// divide points with min and max 
	// max no 0 
	inline auto linspace(int sliderValue, int points, float min, float max) {
		return min + ((sliderValue - 1) * (max - min) / (points - 1));
	}

	inline auto calculateSliderValueForCurrentFilter(const FilterNames name, int sliderValue, float min = 0, float max = 0)
	{


		// Gaazan kod
		//return name == FilterNames::_GaussianBlurClassic || name == FilterNames::_GaussianBlurSeperableConvolutionOpencvKernel
		//	? (int)min % 2 == 0 ? sliderValue == 1 ? ++min : ++min + 2 * (sliderValue - 1) : min + 2 * (sliderValue - 1)
		//	: name == FilterNames::_RecursiveGaussianBlurVliet || name == FilterNames::_BlackAndWhite || name == FilterNames::_B_W_Negative || name == FilterNames::_Contrast
		//	? min + ((sliderValue - 1) * (max - min) / (100 - 1)) /* linspace */
		//	: name == FilterNames::_Brightness ? (max * sliderValue / 100)
		//	: name == FilterNames::_Gamma
		//	? sliderValue == 0 ? 1 : sliderValue < 0 ? ((100 + sliderValue) / 100.0f) : 1 + (3.0f * sliderValue) / 100.0f
		//	: 0;


		if (name == FilterNames::_GaussianBlurClassic || name == FilterNames::_GaussianBlurSeperableConvolutionOpencvKernel) {

			/*if ((int)min % 2 == 0) {
				++min;
			}
			return sliderValue == 1 ? min : min + 2 * (sliderValue - 1);*/

			/////////////////////////////////////////////////////////////////
			int curr = int(min + ((sliderValue - 1) * (max - min) / (100 - 1))); /* linspace */
			return curr % 2 != 0 ? (float)curr : ((float)curr + 1.0f);

		}
		else if (name == FilterNames::_RecursiveGaussianBlurVliet || name == FilterNames::_BlackAndWhite ||
			name == FilterNames::_B_W_Negative || name == FilterNames::_Contrast)
		{
			return (min + ((sliderValue - 1) * (max - min) / (100 - 1))); /* linspace */
		}
		else if (name == FilterNames::_Brightness) {
			return (max * sliderValue / 100);
		}
		else if (name == FilterNames::_Gamma) {
			return sliderValue == 0 ? 1 : sliderValue < 0 ? ((100 + sliderValue) / 100.0f) : 1 + (3.0f * sliderValue) / 100.0f;
		}
		else {
			return 0.f;
		}
	}


	inline uint8_t truncate(int x) { return x > 255 ? 255 : x < 0 ? 0 : (uint8_t)x; }
	inline uint8_t truncate(float x) { return x > 255 ? 255 : x < 0 ? 0 : (uint8_t)x; }
	inline float truncatef(float x) { return x > 255 ? 255 : x < 0 ? 0 : x; }



	static void HSVLSlider(double& outC1, double& outC2, double& outC3, int sIn1, int sIn2, int sIn3) {
		outC1 = (outC1 + sIn1);
		outC2 = (outC2 * (1.0 + sIn2 / 100.0));
		outC3 = (outC3 * (1.0 + sIn3 / 100.0));
	}
	static void YCbCrYUVLSlider(double& outC1, double& outC2, double& outC3, int sIn1, int sIn2, int sIn3) {
		outC1 = outC1 + sIn1;
		outC2 = outC2 + sIn2;
		outC3 = outC3 + sIn3;
	}

	class xFilters
	{
	public:

		static bool Grayscale(xImage* pInImage, xImage* pOutImage);
		///
		static bool GrayscaleMultiThread(xImage* pInImage, xImage* pOutImage);
	private:
		inline static void helpFunctionGrayscaleMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
		///
	public:
		static bool BlackAndWhite(xImage* pInImage, xImage* pOutImage, float fThreshold = 128);
		static bool BlackAndWhiteMultiThread(xImage* pInImage, xImage* pOutImage, float fThreshold = 128);
	private:
		inline static void helpFunctionBlackAndWhiteMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight, float fThreshold);
		//
	public:
		static bool OnlyRed(xImage* pInImage, xImage* pOutImage);
		static bool OnlyRedMultiThread(xImage* pInImage, xImage* pOutImage);
	private:
		inline static void helpFunctionOnlyRedMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
		//
	public:
		static bool OnlyGreen(xImage* pInImage, xImage* pOutImage);
		static bool OnlyGreenMultiThread(xImage* pInImage, xImage* pOutImage);
	private:
		inline static void helpFunctionOnlyGreenMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool OnlyBlue(xImage* pInImage, xImage* pOutImage);
		static bool OnlyBlueMultiThread(xImage* pInImage, xImage* pOutImage);
	private:
		inline static void helpFunctionOnlyBlueMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool GraycaleNegative(xImage* pInImage, xImage* pOutImage);
		static bool GraycaleNegativeMultiThread(xImage* pInImage, xImage* pOutImage);
	private:
		inline static void helpFunctionGraycaleNegativeMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool BlackAndWhiteNegative(xImage* pInImage, xImage* pOutImage, const float fThreshold = 128);
		static bool BlackAndWhiteNegativeMultiThread(xImage* pInImage, xImage* pOutImage, const float fThreshold = 128);
	private:
		inline static void helpFunctionBlackAndWhiteNegativeMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight, const float fThreshold = 128);
	public:
		//
		static bool Negative(xImage* pInImage, xImage* pOutImage);
		static bool NegativeMultiThread(xImage* pInImage, xImage* pOutImage);
	private:
		inline static void helpFunctionNegativeMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool Contrast(xImage* pInImage, xImage* pOutImage, float iContrast);
		static bool ContrastMultiThread(xImage* pInImage, xImage* pOutImage, float iContrast);
	private:
		inline static void helpFunctionContrastMultiThread(uint8_t* pInData, uint8_t* pOutData, const std::vector<uint8_t>& lut, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool Brightness(xImage* pInImage, xImage* pOutImage, int ibrightness);
		static bool BrightnessMultiThread(xImage* pInImage, xImage* pOutImage, int ibrightness);
	private:
		inline static void helpFunctionBrightnessMultiThread(uint8_t* pInData, uint8_t* pOutData, const std::vector<uint8_t>& lut, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool Gamma(xImage* pInImage, xImage* pOutImage, float fGamma);
		static bool GammaMultiThread(xImage* pInImage, xImage* pOutImage, float fGamma);
	private:
		inline static void helpFunctionGammaMultiThread(uint8_t* pInData, uint8_t* pOutData, const std::vector<uint8_t>& lut, const int rowBytes, const int Width, const int lowHeight, const int highHeight);
	public:
		//
		static bool GaussianBlurRealClassic(xImage* pInImage, xImage* pOutImage, int iKernelSize);
		static bool GaussianBlurSeperableConvolution(xImage* pInImage, xImage* pOutImage, int kernelSize, float sigmaX = 0, float sigmaY = 0);
		static float* GenereateGaussianKernelOpencvVersion(int kernelsize, float sigma);
		static float** GenerateGaussianKernelClassic(int iSizeKernel);
		static void GenerateGaussianKernelClassic(std::vector<std::vector<float>>& kernel, int iSizeKernel);

		static bool RecursiveGaussianBlurVliet(xImage* pInImage, xImage* pOutImage, float sigma);

		static bool CopyOriginalToTransformedImage(xImage* pInImage, xImage* pOutImage);


		/// Color Space //////////////////////////////

		static void HSV2RGB(double& hue, double& saturation, double& value, uint8_t& b, uint8_t& g, uint8_t& r);
		static void RGB2HSV(uint8_t& b, uint8_t& g, uint8_t& r, double& hue, double& saturation, double& value);
		static void HSL2RGB(double& hue, double& saturation, double& luminance, uint8_t& b, uint8_t& g, uint8_t& r);
		static void RGB2HSL(uint8_t& b, uint8_t& g, uint8_t& r, double& hue, double& saturation, double& luminance);
		static void YCbCr2RGB(double& Y, double& Cb, double& Cr, uint8_t& b, uint8_t& g, uint8_t& r);
		static void RGB2YCbCr(uint8_t& b, uint8_t& g, uint8_t& r, double& Y, double& Cb, double& Cr);
		static void RGB2YUV(uint8_t& b, uint8_t& g, uint8_t& r, double& Y, double& U, double& V);
		static void YUV2RGB(double& Y, double& U, double& V, uint8_t& b, uint8_t& g, uint8_t& r);


		static void GeenericColorSpace2BGR(xImage* pInImage, xImage* pOutImage, void(*GenFunIn)(uint8_t&, uint8_t&, uint8_t&, double&, double&, double&),
			void(*GenFunOut)(double&, double&, double&, uint8_t&, uint8_t&, uint8_t&),
			void(*GenFunSlider)(double&, double&, double&, int, int, int),
			int C1, int C2, int C3);
		//////////////////////////////////////////////////

		//////////////// Convolution /////////////////////

		static void Convolution1Ch(xImage* pInImage, xImage* pOutImage, std::vector < std::vector <float>>& kernel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pInImage">byte</param>
		/// <param name="pOutImage">float</param>
		/// <param name="kernelX">int</param>
		/// <param name="kernelY">int</param>
		static void SepConvolution1D(xImage* pInImage, xImage* pOutImage, const std::vector<int>& kernelX, const std::vector<int>& kernelY);

		/// <summary>
		///
		/// </summary>
		/// <param name="pInImage">double 64f</param>
		/// <param name="pOutImage">double 64f</param>
		/// <param name="kernelX">int</param>
		/// <param name="kernelY">int</param>
		static void SepConvolution64F1D(xImage* pInImage, xImage* pOutImage, const std::vector<int>& kernelX, const std::vector<int>& kernelY);


		///////// Edge Detection /////////////////////////
		static void getSobelKernels(std::vector<int>& _kx, std::vector<int>& _ky, int dx, int dy, int _ksize);

		static void getPrewittKernel(std::vector<int>& kX, std::vector<int>& kY, int kSize);

		static void Sobel(xImage* pInImage, xImage* pOutImage, int kernelSize);

		static void Prewitt(xImage* pInImage, xImage* pOutImage, int kernelSize);


		static void Canny(xImage* pInImage, xImage* pOutImage);


		////////////// Add OpenCL support for GPU ///////////////////////////////////

		static bool GenericOpenclRunFile(xImage* pInImage, xImage* pOutImage, cl::Context& context,
			const std::string& inputFileCl, const std::string& functionName, const float genericValue = 0.0f);

	};

}
