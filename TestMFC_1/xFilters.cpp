#include "pch.h"


#include "xFilters.h"

//int filter::ConvertPercentToValue255(int x) { return (255 * x / 100); }

// pInImage->IsValid()
bool filter::xFilters::Grayscale(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex = 0, outIndex = 0;

		for (int y = 0; y < imageH; y++)
		{
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x = 0; x < imageW; x++)
			{
				uint8_t red = 0, green = 0, blue = 0;
				uint16_t gray = 0;
				red = inImageData[inIndex + 2];
				green = inImageData[inIndex + 1];
				blue = inImageData[inIndex + 0];

				gray = (red + green + blue) / 3;

				outImageData[outIndex + 2] = uint8_t(gray);
				outImageData[outIndex + 1] = uint8_t(gray);
				outImageData[outIndex + 0] = uint8_t(gray);

				inIndex += 3;
				outIndex += 3;
			}
		}
		return true;
	}
	return false;
}


void filter::xFilters::helpFunctionGrayscaleMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight) {
	for (int y{ lowHeight }; y < highHeight; ++y) {

		auto currRowIn = (uint8_t*)(pInData + (y * rowBytes));
		auto currRowOut = (uint8_t*)(pOutData + (y * rowBytes));
		for (int x{}, i{}; x < Width; ++x, i += 3) {
			currRowOut[i] = (currRowIn[i] + currRowIn[i + 1] + currRowIn[i + 2]) / 3;
			currRowOut[i + 1] = currRowOut[i];
			currRowOut[i + 2] = currRowOut[i];
		}

	}
}

bool filter::xFilters::GrayscaleMultiThread(xImage* pInImage, xImage* pOutImage) {

	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		std::mutex mt;
		const int imageH = pInImage->GetHeight();
		const int imageW = pInImage->GetWidth();

		const int rowBytes = pInImage->GetRowBytes();

		auto pInImageData = pInImage->GetData();
		auto pOutImageData = pOutImage->GetData();

		/*auto tFun = [&](uint8_t* pInData, uint8_t* pOutData, const int lowHeight, const int highHeight) {

			std::lock_guard<std::mutex>{mt};
			for (int y{ lowHeight }; y < highHeight; ++y) {

				auto currRowIn = (uint8_t*)(pInData + (y * rowBytes));
				auto currRowOut = (uint8_t*)(pOutData + (y * rowBytes));
				for (int x{}, i{}; x < imageW; ++x, i += 3) {
					currRowOut[i] = (currRowIn[i] + currRowIn[i + 1] + currRowIn[i + 2]) / 3;
					currRowOut[i + 1] = currRowOut[i];
					currRowOut[i + 2] = currRowOut[i];
				}

			}
			};*/

		std::vector<std::thread> th;

		int n = std::thread::hardware_concurrency();

		int H = imageH / n;

		int mn = imageH % n;


		int low{};
		int high{ H };

		for (int i{}; i < n; ++i) {
			if (i != 0) {
				low += H;
				high += (i == (n - 1) ? (mn + H) : H);
			}
			th.emplace_back(filter::xFilters::helpFunctionGrayscaleMultiThread, pInImageData, pOutImageData, rowBytes, imageW, low, high);
		}

		for (int i{}; i < n; ++i) {
			th[i].join();
		}


		return true;
	}

	return false;
}



bool filter::xFilters::BlackAndWhite(xImage* pInImage, xImage* pOutImage, float fThreshold/*=128*/) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex = 0, outIndex = 0;

		for (int y = 0; y < imageH; y++)
		{
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x = 0; x < imageW; x++)
			{
				uint8_t red = 0, green = 0, blue = 0;
				uint16_t gray = 0;
				red = inImageData[inIndex + 2];
				green = inImageData[inIndex + 1];
				blue = inImageData[inIndex + 0];

				gray = (red + green + blue) / 3;

				if (gray < fThreshold)
					gray = 0;
				else
					gray = 0xff;

				outImageData[outIndex + 2] = uint8_t(gray);
				outImageData[outIndex + 1] = uint8_t(gray);
				outImageData[outIndex + 0] = uint8_t(gray);

				inIndex += 3;
				outIndex += 3;
			}
		}
		return true;
	}
	return false;
}


void filter::xFilters::helpFunctionBlackAndWhiteMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight, float fThreshold) {

	for (int y{ lowHeight }; y < highHeight; ++y) {

		auto currRowIn = (uint8_t*)(pInData + (y * rowBytes));
		auto currRowOut = (uint8_t*)(pOutData + (y * rowBytes));
		for (int x{}, i{}; x < Width; ++x, i += 3) {

			int gray = (currRowIn[i] + currRowIn[i + 1] + currRowIn[i + 2]) / 3;

			if (gray < fThreshold)
				gray = 0;
			else
				gray = 0xff;

			currRowOut[i] = gray;
			currRowOut[i + 1] = currRowOut[i];
			currRowOut[i + 2] = currRowOut[i];
		}

	}
}

bool filter::xFilters::BlackAndWhiteMultiThread(xImage* pInImage, xImage* pOutImage, float fThreshold) {

	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const auto imageH = pInImage->GetHeight();
		const auto imageW = pInImage->GetWidth();
		const auto rowBytes = pInImage->GetRowBytes();

		auto inData = pInImage->GetData();
		auto outData = pOutImage->GetData();

		int n = std::thread::hardware_concurrency();

		int H = imageH / n;
		int mn = imageH % n;

		int low{};
		int high{ H };


		std::vector<std::thread> th;

		for (int i{}; i < n; ++i) {
			if (i != 0) {
				low += H;
				high += (i == n - 1 ? H + mn : H);
			}

			th.emplace_back(helpFunctionBlackAndWhiteMultiThread, inData, outData, rowBytes, imageW, low, high, fThreshold);
		}

		for (int i{}; i < n; ++i) {
			th[i].join();
		}


		return true;
	}
	return false;
}

bool filter::xFilters::OnlyRed(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();


		int inIndex = 0, outIndex = 0;

		for (int y = 0; y < imageH; y++)
		{
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x = 0; x < imageW; x++)
			{
				outImageData[outIndex + 2] = inImageData[inIndex + 2];
				outImageData[outIndex + 1] = 0;
				outImageData[outIndex + 0] = 0;

				inIndex += 3;
				outIndex += 3;
			}
		}
		return true;
	}
	return false;
}


void filter::xFilters::helpFunctionOnlyRedMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight)
{
	for (int y{ lowHeight }; y < highHeight; ++y) {

		auto pInDataCurrRow = (uint8_t*)(pInData + (y * rowBytes));
		auto pOutDataCurrRow = (uint8_t*)(pOutData + (y * rowBytes));
		for (int x{}, index{}; x < Width; ++x, index += 3) {

			pOutDataCurrRow[index + 2] = pInDataCurrRow[index + 2];
			pOutDataCurrRow[index + 1] = 0;
			pOutDataCurrRow[index] = 0;

		}
	}
}

bool filter::xFilters::OnlyRedMultiThread(xImage* pInImage, xImage* pOutImage) {
	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const int imageW = pInImage->GetWidth();
		const int imageH = pInImage->GetHeight();

		const int rowBytes = pInImage->GetRowBytes();

		auto pInData = pInImage->GetData();
		auto pOutData = pOutImage->GetData();

		const int thSize = std::thread::hardware_concurrency();

		int n = imageH / thSize;
		int m = imageH % thSize;

		std::vector<std::jthread> threads;

		int low{};
		int high{ n };

		for (int i{}; i < thSize; ++i) {

			if (i != 0) {
				low += n;
				high += (i == thSize - 1 ? n + m : n);
			}

			threads.emplace_back(helpFunctionOnlyRedMultiThread, pInData, pOutData, rowBytes, imageW, low, high);
		}

		return true;
	}

	return false;
}







bool filter::xFilters::OnlyGreen(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex = 0, outIndex = 0;

		for (int y = 0; y < imageH; y++)
		{
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x = 0; x < imageW; x++)
			{
				uint8_t green = 0;

				green = inImageData[inIndex + 1];

				outImageData[outIndex + 2] = 0;
				outImageData[outIndex + 1] = green;
				outImageData[outIndex + 0] = 0;

				inIndex += 3;
				outIndex += 3;
			}
		}

		return true;
	}
	return false;
}

void filter::xFilters::helpFunctionOnlyGreenMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight)
{
	for (int y{ lowHeight }; y < highHeight; ++y) {
		auto pInDataCurrRow = (pInData + (y * rowBytes));
		auto pOutDataCurrRow = (pOutData + (y * rowBytes));
		for (int x{}, index{}; x < Width; ++x, index += 3) {
			pOutDataCurrRow[index + 2] = 0;
			pOutDataCurrRow[index + 1] = pOutDataCurrRow[index + 1];
			pOutDataCurrRow[index] = 0;
		}
	}
}

bool filter::xFilters::OnlyGreenMultiThread(xImage* pInImage, xImage* pOutImage)
{
	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const int imageW = pInImage->GetWidth();
		const int imageH = pInImage->GetHeight();
		const int rowBytes = pInImage->GetRowBytes();

		uint8_t* pInData = pInImage->GetData();
		uint8_t* pOutData = pOutImage->GetData();

		const int thSize = std::thread::hardware_concurrency();

		const int n = imageH / thSize;
		const int m = imageH % thSize;

		int low{};
		int high{ n };

		std::vector<std::jthread> threads;

		for (int i{}; i < thSize; ++i) {
			if (i != 0) {
				low += n;
				high += (i == thSize - 1 ? n + m : n);
			}
			threads.emplace_back(helpFunctionOnlyGreenMultiThread, pInData, pOutData, rowBytes, imageW, low, high);
		}

		return true;
	}
	return false;
}


bool filter::xFilters::OnlyBlue(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex = 0, outIndex = 0;

		for (int y = 0; y < imageH; y++)
		{
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x = 0; x < imageW; x++)
			{
				outImageData[outIndex + 2] = 0;
				outImageData[outIndex + 1] = 0;
				outImageData[outIndex + 0] = inImageData[inIndex + 0];

				inIndex += 3;
				outIndex += 3;
			}
		}
		return true;
	}
	return false;
}

void filter::xFilters::helpFunctionOnlyBlueMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight)
{
	for (int y{ lowHeight }; y < highHeight; ++y) {
		auto pInDataCurrRow = (pInData + (y * rowBytes));
		auto pOutDataCurrRow = (pOutData + (y * rowBytes));
		for (int x{}, index{}; x < Width; ++x, index += 3) {
			pOutDataCurrRow[index] = pInDataCurrRow[index];
			pOutDataCurrRow[index + 1] = 0;
			pOutDataCurrRow[index + 2] = 0;
		}
	}

}

bool filter::xFilters::OnlyBlueMultiThread(xImage* pInImage, xImage* pOutImage)
{
	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const int imageW = pInImage->GetWidth();
		const int imageH = pInImage->GetHeight();
		const int rowBytes = pInImage->GetRowBytes();

		uint8_t* pInData = pInImage->GetData();
		uint8_t* pOutData = pOutImage->GetData();

		const int thSize = std::thread::hardware_concurrency();

		const int n = imageH / thSize;
		const int m = imageH % thSize;

		int low{};
		int high{ n };

		std::vector<std::jthread> threads;

		for (int i{}; i < thSize; ++i) {
			if (i != 0) {
				low += n;
				high += (i == thSize - 1 ? m + n : n);
			}
			threads.emplace_back(helpFunctionOnlyBlueMultiThread, pInData, pOutData, rowBytes, imageW, low, high);
		}

		return true;
	}
	return false;
}





bool filter::xFilters::GraycaleNegative(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex{}, outIndex{};

		for (int y{}; y < imageH; y++) {
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x{}; x < imageW; x++) {

				uint8_t red = 0, green = 0, blue = 0, dif = 0;
				uint16_t gray = 0;
				blue = inImageData[inIndex + 0];
				green = inImageData[inIndex + 1];
				red = inImageData[inIndex + 2];

				gray = (blue + green + red) / 3;

				dif = 255 - (uint8_t)gray;


				outImageData[outIndex + 0] = dif;
				outImageData[outIndex + 1] = dif;
				outImageData[outIndex + 2] = dif;


				inIndex += 3;
				outIndex += 3;
			}
		}
		return true;
	}
	return false;
}



void filter::xFilters::helpFunctionGraycaleNegativeMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight) {

	for (int y{ lowHeight }; y < highHeight; ++y) {
		auto pInDataCurrRow = (pInData + (y * rowBytes));
		auto pOutDataCurrRow = (pOutData + (y * rowBytes));
		for (int x{}, index{}; x < Width; ++x, index += 3) {
			pOutDataCurrRow[index] = 255 - pInDataCurrRow[index];
			pOutDataCurrRow[index + 1] = 255 - pInDataCurrRow[index + 1];
			pOutDataCurrRow[index + 2] = 255 - pInDataCurrRow[index + 1];
		}
	}

}

bool filter::xFilters::GraycaleNegativeMultiThread(xImage* pInImage, xImage* pOutImage) {

	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const int imageW = pInImage->GetWidth();
		const int imageH = pOutImage->GetHeight();
		const int rowBytes = pOutImage->GetRowBytes();

		uint8_t* pInData = pInImage->GetData();
		uint8_t* pOutData = pOutImage->GetData();

		const int thSize = std::thread::hardware_concurrency();

		const int n = imageH / thSize;
		const int m = imageH % thSize;

		int low{};
		int high{ n };

		std::vector<std::jthread> threads;

		for (int i{}; i < thSize; ++i) {
			if (i != 0) {
				low += n;
				high += (i == thSize - 1 ? m + n : n);
			}
			threads.emplace_back(helpFunctionGraycaleNegativeMultiThread, pInData, pOutData, rowBytes, imageW, low, high);
		}

		return true;
	}
	return false;
}



bool filter::xFilters::BlackAndWhiteNegative(xImage* pInImage, xImage* pOutImage, float fThreshold) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex{}, outIndex{};

		for (int y{}; y < imageH; y++) {
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x{}; x < imageW; x++) {
				uint8_t blue = 0, green = 0, red = 0;
				uint16_t gray = 0;

				blue = inImageData[inIndex + 0];
				green = inImageData[inIndex + 1];
				red = inImageData[inIndex + 2];

				gray = (blue + green + red) / 2;

				if (gray > fThreshold)
					gray = 0;
				else gray = 0xff;

				outImageData[outIndex + 0] = (uint8_t)gray;
				outImageData[outIndex + 1] = (uint8_t)gray;
				outImageData[outIndex + 2] = (uint8_t)gray;

				inIndex += 3;
				outIndex += 3;

			}
		}
		return true;
	}

	return false;
}

void filter::xFilters::helpFunctionBlackAndWhiteNegativeMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight, const float fThreshold) {

	for (int y{ lowHeight }; y < highHeight; ++y) {
		auto pInDataCurrRow = (pInData + (y * rowBytes));
		auto pOutDataCurrRow = (pOutData + (y * rowBytes));
		for (int x{}, index{}; x < Width; ++x, index += 3) {
			uint8_t gray = (pInDataCurrRow[index] + pInDataCurrRow[index + 1] + pInDataCurrRow[index + 2]) / 3;

			if (gray > fThreshold)
				gray = 0;
			else gray = 0xff;

			pOutDataCurrRow[index] = gray;
			pOutDataCurrRow[index + 1] = gray;
			pOutDataCurrRow[index + 2] = gray;
		}
	}

}

bool filter::xFilters::BlackAndWhiteNegativeMultiThread(xImage* pInImage, xImage* pOutImage, const float fThreshold) {

	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const int imageW = pInImage->GetWidth();
		const int imageH = pInImage->GetHeight();
		const int rowBytes = pInImage->GetRowBytes();

		auto pIndata = pInImage->GetData();
		auto pOutData = pOutImage->GetData();

		const int thSize = std::thread::hardware_concurrency();

		const int n = imageH / thSize;
		const int m = imageH % thSize;

		std::vector<std::jthread> threads;

		int low{};
		int high{ n };

		for (int i{}; i < thSize; ++i) {
			if (i != 0) {
				low += n;
				high += (i == thSize - 1 ? m + n : n);
			}
			threads.emplace_back(helpFunctionBlackAndWhiteNegativeMultiThread, pIndata, pOutData, rowBytes, imageW, low, high, fThreshold);
		}
		return true;
	}
	return false;
}


bool filter::xFilters::Negative(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();
		int inIndex{}, outIndex{};

		for (int y{}; y < imageH; y++) {
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x{}; x < imageW; x++) {
				uint8_t red = 0, green = 0, blue = 0;

				blue = inImageData[inIndex + 0];
				green = inImageData[inIndex + 1];
				red = inImageData[inIndex + 2];

				outImageData[outIndex + 0] = 255 - blue;
				outImageData[outIndex + 1] = 255 - green;
				outImageData[outIndex + 2] = 255 - red;

				inIndex += 3;
				outIndex += 3;

			}
		}
		return true;
	}
	return false;
}


void filter::xFilters::helpFunctionNegativeMultiThread(uint8_t* pInData, uint8_t* pOutData, const int rowBytes, const int Width, const int lowHeight, const int highHeight)
{
	for (int y{ lowHeight }; y < highHeight; ++y) {
		auto pInDataCurrRow = (pInData + (y * rowBytes));
		auto pOutDataCurrRow(pOutData + (y * rowBytes));
		for (int x{}, index{}; x < Width; ++x, index += 3) {

			pOutDataCurrRow[index] = 255 - pInDataCurrRow[index];
			pOutDataCurrRow[index + 1] = 255 - pInDataCurrRow[index + 2];
			pOutDataCurrRow[index + 2] = 255 - pInDataCurrRow[index + 2];


		}
	}
}


bool filter::xFilters::NegativeMultiThread(xImage* pInImage, xImage* pOutImage)
{
	if (pInImage && pOutImage && pInImage->IsValid() && pOutImage->IsValid()) {

		const int imageW = pInImage->GetWidth();
		const int imageH = pInImage->GetHeight();
		const int rowBytes = pInImage->GetRowBytes();

		const auto pInData = pInImage->GetData();
		const auto pOutData = pOutImage->GetData();

		const int thSize = std::thread::hardware_concurrency();

		const int n = imageH / thSize;
		const int m = imageH % thSize;

		int low{};
		int high{ n };

		std::vector<std::jthread> threads;

		for (int i{}; i < thSize; ++i) {
			if (i != 0) {
				low += n;
				high += (i == thSize - 1 ? m + n : n);
			}
			threads.emplace_back(helpFunctionNegativeMultiThread, pInData, pOutData, rowBytes, imageW, low, high);
		}

		return true;
	}

	return false;
}



bool filter::xFilters::Contrast(xImage* pInImage, xImage* pOutImage, float iContrast) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex{}, outIndex{};

		const float factor = (259.0f * (iContrast + 255.0f)) / (255.0f * (259.0f - iContrast));

		std::vector<uint8_t> LUT(0x100);
		for (size_t i{}; i != LUT.size(); ++i) {
			LUT[i] = truncate((factor * ((float)i - 128.0f) + 128.0f));
		}


		for (int y{}; y < imageH; y++) {

			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;

			for (int x{}; x < imageW; x++) {
				uint8_t red = 0, green = 0, blue = 0;

				blue = inImageData[inIndex + 0];
				green = inImageData[inIndex + 1];
				red = inImageData[inIndex + 2];

				//std::clamp()
				blue = LUT[blue];//truncate((int)(factor * ((float)blue - 128.0f) + 128.0f));

				green = LUT[green];// truncate((int)(factor * ((float)green - 128.0f) + 128.0f));

				red = LUT[red];//truncate((int)(factor * ((float)red - 128.0f) + 128.0f));

				outImageData[outIndex + 0] = blue;
				outImageData[outIndex + 1] = green;
				outImageData[outIndex + 2] = red;

				outIndex += 3;
				inIndex += 3;
			}
		}

		return true;
	}
	return false;
}

bool filter::xFilters::Brightness(xImage* pInImage, xImage* pOutImage, int ibrightness) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();
		int inIndex{}, outIndex{};

		std::vector<uint8_t> LUT(0x100);
		for (size_t i{}; i != LUT.size(); ++i) {
			LUT[i] = truncate(int(i + ibrightness));
		}


		for (int y{}; y < imageH; y++) {

			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;

			for (int x{}; x < imageW; ++x) {
				uint8_t red = 0, green = 0, blue = 0;

				blue = inImageData[inIndex + 0];
				green = inImageData[inIndex + 1];
				red = inImageData[inIndex + 2];


				blue = LUT[blue];//truncate(blue + ibrightness);

				green = LUT[green];//truncate(green + ibrightness);

				red = LUT[red];//truncate(red + ibrightness);

				outImageData[outIndex + 0] = blue;
				outImageData[outIndex + 1] = green;
				outImageData[outIndex + 2] = red;

				outIndex += 3;
				inIndex += 3;
			}
		}

		return true;
	}
	return false;

}


bool filter::xFilters::Gamma(xImage* pInImage, xImage* pOutImage, float fGamma) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		int inIndex = 0, outIndex = 0;

		const float gammaCorrection = 1.0f / fGamma;

		std::vector<uint8_t> gammaLUT(0x100);
		for (size_t i = 0; i < gammaLUT.size(); i++)
		{
			gammaLUT[i] = truncate(255 * std::pow((i / 255.0f), gammaCorrection));
		}

		for (int y = 0; y < imageH; ++y) {
			inIndex = y * inRowBytes;
			outIndex = y * outRowBytes;
			for (int x = 0; x < imageW; ++x) {
				uint8_t red = 0, green = 0, blue = 0;

				blue = inImageData[inIndex + 0];
				green = inImageData[inIndex + 1];
				red = inImageData[inIndex + 2];

				blue = gammaLUT[blue];
				green = gammaLUT[green];
				red = gammaLUT[red];

				//blue = truncate(255 * std::pow((blue / 255.0f), gammaCorrection));
				//green = truncate(255 * std::pow((green / 255.0f), gammaCorrection));
				//red = truncate(255 * std::pow((red / 255.0f), gammaCorrection));

				outImageData[outIndex + 0] = blue;
				outImageData[outIndex + 1] = green;
				outImageData[outIndex + 2] = red;

				inIndex += 3;
				outIndex += 3;
			}
		}
		return true;
	}

	return false;
}



// Generate Gaussian kernel classic
inline float** filter::xFilters::GenerateGaussianKernelClassic(int iSizeKernel) {

	float sigma = max(iSizeKernel / 2.0f, 1.0f);// 0.3 * ((newKernelSize - 1) * 0.5 - 1) + 0.8;
	float twoQuadratSigma = 2 * sigma * sigma;
	float quadratSigmaPi = twoQuadratSigma * float(std::numbers::pi);

	int step = iSizeKernel / 2;
	float sum{};

	/////
	float** kernel = new float* [iSizeKernel] {};

	for (int i{}; i != iSizeKernel; ++i)
		kernel[i] = new float[iSizeKernel] {};
	/////
	for (int i = -step; i <= step; ++i) {
		for (int j = -step; j <= step; ++j) {
			float quadraticSum = -(float)(i * i + j * j);
			kernel[i + step][j + step] = std::expf(quadraticSum / twoQuadratSigma) / quadratSigmaPi;
			sum += kernel[i + step][j + step];
		}
	}

	for (int i{}; i != iSizeKernel; ++i) {
		for (int j{}; j != iSizeKernel; ++j) {
			kernel[i][j] /= sum;
		}
	}

	return kernel;
}






// Generate Gaussian blur real classic 
bool filter::xFilters::GaussianBlurRealClassic(xImage* pInImage, xImage* pOutImage, int size) {


	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();

		if (!pOutImage->IsValid()) {
			pOutImage->allocateSpace(imageW, imageH, COLOR_ORDER::_BGR);
		}




		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		////

		//std::vector<std::vector<float>> kernel;

		float** kernel = GenerateGaussianKernelClassic(size);

		////

		int radius = size / 2;

		int leftradius{ radius }, rightradius{ radius }, topradius{ radius }, bottomradius{ radius };

		float b{}, g{}, r{};

		float ksum{};

		for (int y{}; y < imageH; ++y) {

			bottomradius = min(radius, y);

			topradius = y + radius > imageH - 1 ? --topradius : topradius;

			for (int x{}, pos{}; x < imageW; ++x, pos += 3) {

				leftradius = min(radius, x);

				rightradius = x + radius > imageW - 1 ? --rightradius : rightradius;

				for (int ky = -1 * bottomradius; ky <= topradius; ++ky) {

					int currentrowposforkernel = ((y + ky) * inRowBytes) + pos;

					int ik = ky + radius;

					for (int kx = -1 * leftradius; kx <= rightradius; ++kx) {

						int jk = kx + radius;

						int imagePosChange = 3 * kx;

						b += (inImageData[currentrowposforkernel + imagePosChange] * kernel[ik][jk]);
						g += (inImageData[currentrowposforkernel + imagePosChange + 1] * kernel[ik][jk]);
						r += (inImageData[currentrowposforkernel + imagePosChange + 2] * kernel[ik][jk]);
						ksum += kernel[ik][jk];

					}
				}

				int currentpos = (y * outRowBytes) + pos;
				outImageData[currentpos] = truncate(b / ksum);
				outImageData[currentpos + 1] = truncate(g / ksum);
				outImageData[currentpos + 2] = truncate(r / ksum);
				b = 0;
				g = 0;
				r = 0;
				ksum = 0;
			}
			rightradius = radius;

		}


		//// Delete kernel
		for (int i = size - 1; i >= 0; --i)
			delete[] kernel[i];

		delete[] kernel;
		////

		return true;

	}

	return false;
}


// read for use code this paper 
// https://d-nb.info/1239404778/34
// https://www.researchgate.net/publication/222453003_Recursive_implementation_of_the_Gaussian_filter
/////////////////////////////
bool filter::xFilters::RecursiveGaussianBlurVliet(xImage* pInImage, xImage* pOutImage, float sigma) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid())
	{


		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();

		if (!pOutImage->IsValid()) {
			pOutImage->allocateSpace(imageW, imageH, COLOR_ORDER::_BYTE);
		}

		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		// allocate intermediate storage ///////////
		float* buffer = new float[imageW * imageH * 3];

		// chose q from paper Equation 11b ////////////////



		float q{};

		if (sigma >= 2.5f)
			q = 0.98711f * sigma - 0.96330f;
		else if (sigma >= 0.5f)
			q = 3.97156f - 4.14554f * std::sqrtf(1.0f - 0.26891f * sigma);
		else
			return false;

		////////////////////////////////////////////////

		// use equation 8c to determine b0,b1,b2,b3 ////////////////

		float b0 = 1.57825f + 2.44413f * q + 1.4281f * q * q + 0.422205f * q * q * q;

		float b1 = 2.44413f * q + 2.85619f * q * q + 1.26661f * q * q * q;

		float b2 = -(1.4281f * q * q + 1.26661f * q * q * q);

		float b3 = 0.422205f * q * q * q;

		// use equation 10 to determine B

		float B = 1.0f - (b1 + b2 + b3) / b0;

		/////////////////////////////////////////////////////////////

		// three previous pixel component = 3 (BGR)
		float prev1[3]{ 0 }, prev2[3]{}, prev3[3]{};

		int inCurrentRow{}, buffCurrentRow{}, inCurrentRowChosePixel{}, buffCurrentRowChosePixel{},
			buffCurrentRowPixel{}, outCurrentRowPixel{}, backwardpixel{};

		// Horinzontal forward pass (from paper: Implement the forward filter with equation 9a)
		// the data is to buffer  
		for (int y{}; y < imageH; ++y) {
			inCurrentRow = y * inRowBytes;
			buffCurrentRow = y * imageW * 3;
			for (UINT n{}; n < 3; ++n) {
				prev1[n] = inImageData[inCurrentRow + n];
				prev2[n] = prev1[n];
				prev3[n] = prev2[n];
			}

			for (int x{}; x < imageW; ++x) {
				int inCurrentRowChosePixel = inCurrentRow + (x * 3);
				int buffCurrentRowChosePixel = buffCurrentRow + (x * 3);
				for (int n{}; n < 3; ++n) {
					float val = B * inImageData[inCurrentRowChosePixel + n] + (b1 * prev1[n] + b2 * prev2[n] + b3 * prev3[n]) / b0;

					buffer[buffCurrentRowChosePixel + n] = val;

					prev3[n] = prev2[n];
					prev2[n] = prev1[n];
					prev1[n] = val;
				}
			}

		}

		// Horizontal backward pass (fromt paper: Implement the backward filter equation 9b)
		// the data is to load out image
		for (int y = imageH - 1; y >= 0; --y) {
			//	UINT inCurrentRow = y * inRowBytes;
			buffCurrentRow = y * imageW * 3;
			backwardpixel = buffCurrentRow + ((imageW - 1) * 3);
			for (UINT n{}; n < 3; ++n) {
				prev1[n] = buffer[backwardpixel + n];
				prev2[n] = prev1[n];
				prev3[n] = prev2[n];
			}

			for (int x = imageW - 1; x >= 0; --x) {
				buffCurrentRowChosePixel = buffCurrentRow + (x * 3);
				for (UINT n{}; n < 3; ++n) {

					const float val = B * buffer[buffCurrentRowChosePixel + n] + (b1 * prev1[n] + b2 * prev2[n] + b3 * prev3[n]) / b0;

					buffer[buffCurrentRowChosePixel + n] = val;

					prev3[n] = prev2[n];
					prev2[n] = prev1[n];
					prev1[n] = val;

				}
			}

		}

		////////////////////////////////////////////////////////////////////////

		// vertical forward pass (from paper: implement forward filter equation 9a)
		for (int x{}; x < imageW; ++x) {

			buffCurrentRowChosePixel = x * 3;
			for (int n{}; n < 3; ++n) {
				prev1[n] = buffer[buffCurrentRowChosePixel + n];
				prev2[n] = prev1[n];
				prev3[n] = prev2[n];
			}
			for (int y{}; y < imageH; ++y) {
				buffCurrentRowPixel = (y * imageW * 3) + buffCurrentRowChosePixel;
				for (int n{}; n < 3; ++n) {
					float val = B * buffer[buffCurrentRowPixel + n] + (b1 * prev1[n] + b2 * prev2[n] + b3 * prev3[n]) / b0;

					buffer[buffCurrentRowPixel + n] = val;

					prev3[n] = prev2[n];
					prev2[n] = prev1[n];
					prev1[n] = val;
				}
			}
		}


		// vertical backward pass (from paper: implement backward filter equation 9b)
		for (int x = imageW - 1; x >= 0; --x) {

			buffCurrentRowChosePixel = x * 3;
			backwardpixel = ((imageH - 1) * imageW * 3);
			for (int n{}; n < 3; ++n) {
				prev1[n] = buffer[backwardpixel + buffCurrentRowChosePixel + n];
				prev2[n] = prev1[n];
				prev3[n] = prev2[n];
			}

			for (int y = imageH - 1; y >= 0; --y) {
				buffCurrentRowPixel = (y * imageW * 3) + buffCurrentRowChosePixel;
				outCurrentRowPixel = (y * outRowBytes) + buffCurrentRowChosePixel;
				for (UINT n{}; n < 3; ++n) {
					float val = B * buffer[buffCurrentRowPixel + n] + (b1 * prev1[n] + b2 * prev2[n] + b3 * prev3[n]) / b0;

					outImageData[outCurrentRowPixel + n] = truncate(val);

					prev3[n] = prev2[n];
					prev2[n] = prev1[n];
					prev1[n] = val;
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		// delete buffer 
		delete[] buffer;
		buffer = nullptr;

		//////////////////////////////

		return true;

	}
	return false;
}



// Generate Guassian 1D kernel   opencv Version
// https://github.com/egonSchiele/OpenCV/blob/master/modules/imgproc/src/smooth.cpp
inline float* filter::xFilters::GenereateGaussianKernelOpencvVersion(int kernelsize, float sigma) {

	const int SMALL_GAUSSIAN_SIZE = 7;
	static const float small_gaussian_tab[][SMALL_GAUSSIAN_SIZE] =
	{
		{1.f},
		{0.25f, 0.5f, 0.25f},
		{0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f},
		{0.03125f, 0.109375f, 0.21875f, 0.28125f, 0.21875f, 0.109375f, 0.03125f}
	};


	const float* fixed_kernel = (kernelsize % 2 == 1) && (kernelsize < SMALL_GAUSSIAN_SIZE && sigma <= 0) ? small_gaussian_tab[kernelsize >> 1] : 0;

	float* kernel1d = new float[kernelsize] {};

	float sigmaX = sigma > 0 ? sigma : ((kernelsize - 1) * 0.5f - 1.0f) * 0.3f + 0.8f;
	float scale2x = -0.5f / (sigmaX * sigmaX);
	float sum{};

	int i{};

	for (i = 0; i < kernelsize; ++i) {

		float x = i - (kernelsize - 1) * 0.5f;
		float t = fixed_kernel ? fixed_kernel[i] : std::expf(scale2x * x * x);

		kernel1d[i] = t;
		sum += kernel1d[i];

	}

	sum = 1.0f / sum;

	for (i = 0; i < kernelsize; ++i) {
		kernel1d[i] *= sum;
	}

	return kernel1d;

}

// Guassian blur use seperable convolution two 1d kernel (n x 1) and (1 x n)
// https://songho.ca/dsp/convolution/convolution2d_separable.html
bool filter::xFilters::GaussianBlurSeperableConvolution(xImage* pInImage, xImage* pOutImage, int kernelSize, float sigmaX, float sigmaY) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();

		if (!pOutImage->IsValid()) {
			pOutImage->allocateSpace(imageW, imageH, COLOR_ORDER::_BGR);
		}



		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		//////////////////////////////////////////////////////////////////////////

		sigmaX = sigmaX == 0 ? kernelSize / 2.0f : sigmaX;

		if (sigmaY == 0)
			sigmaY = sigmaX;

		float* kernelX = GenereateGaussianKernelOpencvVersion(kernelSize, sigmaX);
		float* kernelY{ nullptr };
		if (sigmaX == sigmaY) {

			kernelY = kernelX;
		}
		else {

			kernelY = GenereateGaussianKernelOpencvVersion(kernelSize, sigmaY);
		}

		///////////////////////////////////////////////////////////////////////////


		// allocate Temprorary space
		float* tmpData = new float[imageH * inRowBytes] {};


		float b{}, g{}, r{}, ksum{};
		int currentRow{}, currentRowPixel{};

		int radius = kernelSize / 2;
		int leftradius{ radius }, rightradius{ radius }, bottomradius{ radius }, topradius{ radius };


		for (int y{}; y < imageH; ++y) {

			currentRow = y * inRowBytes;
			for (int x{}; x < imageW; ++x) {

				leftradius = min(radius, x);

				rightradius = x + radius > imageW - 1 ? --rightradius : rightradius;

				currentRowPixel = currentRow + (x * 3);
				for (int kx = -leftradius; kx <= rightradius; ++kx) {

					UINT imagePosKernelChange = kx * 3;

					b += kernelX[kx + radius] * inImageData[currentRowPixel + imagePosKernelChange];
					g += kernelX[kx + radius] * inImageData[currentRowPixel + imagePosKernelChange + 1];
					r += kernelX[kx + radius] * inImageData[currentRowPixel + imagePosKernelChange + 2];
					ksum += kernelX[kx + radius];
				}

				tmpData[currentRowPixel] = utility::saturate_cast(b / ksum);
				tmpData[currentRowPixel + 1] = utility::saturate_cast(g / ksum);
				tmpData[currentRowPixel + 2] = utility::saturate_cast(r / ksum);

				b = 0;
				g = 0;
				r = 0;
				ksum = 0;


			}
			rightradius = radius;
		}
		//////////////////

		b = 0;
		g = 0;
		r = 0;

		////////////////////
		for (int x{}; x < imageW; ++x) {

			int currentPixel = x * 3;


			for (int y{}; y < imageH; ++y) {

				bottomradius = min(radius, y);

				topradius = y + radius > imageH - 1 ? --topradius : topradius;

				currentRowPixel = (y * outRowBytes) + currentPixel;

				for (int ky = -bottomradius; ky <= topradius; ++ky) {

					UINT imagePosKernelChange = (y + ky) * outRowBytes + currentPixel;

					b += tmpData[imagePosKernelChange] * kernelY[ky + radius];
					g += tmpData[imagePosKernelChange + 1] * kernelY[ky + radius];
					r += tmpData[imagePosKernelChange + 2] * kernelY[ky + radius];
					ksum += kernelY[ky + radius];
				}

				outImageData[currentRowPixel] = utility::saturate_cast(b / ksum);
				outImageData[currentRowPixel + 1] = utility::saturate_cast(g / ksum);
				outImageData[currentRowPixel + 2] = utility::saturate_cast(r / ksum);
				b = 0;
				g = 0;
				r = 0;
				ksum = 0;


			}
			topradius = radius;

		}

		//// delete kernelX and kernelY and tmpData //////////////////////////
		if (kernelX != kernelY) {
			delete[] kernelY;
		}
		delete[] kernelX;
		delete[] tmpData;
		////////////////////////////////

		return true;

	}

	return false;
}



////////////////////////// COLOR SPACE ////////////////////////////////////////


extern "C" void filter::xFilters::HSV2RGB(double& hue, double& saturation, double& value, uint8_t& b, uint8_t& g, uint8_t& r) {

	double c{}, x{}, m{}, tmpb{}, tmpg{}, tmpr{};

	//hue = std::fmod(hue, 360.0f);

	if (hue < 0)
		hue += 360.0f;

	double h_dash = hue / 60.0f;
	c = (saturation * value);

	x = c * (1 - fabs(fmod(h_dash, 2) - 1));
	m = value - c;

	if (0.0f <= hue && hue < 60.0f) {
		tmpr = c;
		tmpg = x;
		tmpb = 0;
	}
	else if (60.0f <= hue && hue < 120.0f) {
		tmpr = x;
		tmpg = c;
		tmpb = 0;
	}
	else if (120.0f <= hue && hue < 180.0f) {
		tmpr = 0;
		tmpg = c;
		tmpb = x;
	}
	else if (180.0f <= hue && hue < 240.0f) {
		tmpr = 0;
		tmpg = x;
		tmpb = c;
	}
	else if (240.0f <= hue && hue < 300.0f) {
		tmpr = x;
		tmpg = 0;
		tmpb = c;
	}
	else {
		tmpr = c;
		tmpg = 0;
		tmpb = x;
	}

	r = utility::saturate_cast((tmpr + m) * 255.0);
	g = utility::saturate_cast((tmpg + m) * 255.0);
	b = utility::saturate_cast((tmpb + m) * 255.0);

}



extern "C" void filter::xFilters::RGB2HSV(uint8_t& b, uint8_t& g, uint8_t& r, double& hue, double& saturation, double& value) {
	double tmpr{}, tmpg{}, tmpb{};
	double cmax{}, cmin{}, delta{}, tmphue{}, tmpsaturation{}, tmpvalue{};


	tmpr = r / 255.0;
	tmpg = g / 255.0;
	tmpb = b / 255.0;

	cmax = max(max(tmpr, tmpg), tmpb);
	cmin = min(min(tmpr, tmpg), tmpb);
	delta = cmax - cmin;

	if (delta != 0) {
		if (cmax == tmpr) {
			tmphue = 60 * fmod(((tmpg - tmpb) / delta), 6);
		}
		else if (cmax == tmpg) {
			tmphue = 60 * (((tmpb - tmpr) / delta) + 2);
		}
		else {
			tmphue = 60 * (((tmpr - tmpg) / delta) + 4);
		}
	}
	else {
		tmphue = 0;
	}

	if (tmphue < 0) {
		hue += 360;
	}

	tmpsaturation = (cmax == 0.0) ? 0.0 : (delta / cmax);
	tmpvalue = cmax;

	hue = tmphue;
	saturation = tmpsaturation;
	value = tmpvalue;

}





void filter::xFilters::HSL2RGB(double& hue, double& saturation, double& luminance, uint8_t& b, uint8_t& g, uint8_t& r) {

	double c{}, x{}, m{};
	double tmpr{}, tmpg{}, tmpb{};

	hue = std::fmod(hue, 360);
	if (hue < 0)
		hue += 360;

	saturation = max(0.0, min(1.0, saturation));
	luminance = max(0.0, min(1.0, luminance));

	c = (1.0 - fabs(2.0 * luminance - 1.0)) * saturation;
	x = c * (1.0 - fabs(fmod(hue / 60.0, 2.0) - 1.0));
	m = luminance - c / 2.0;

	if (hue >= 0.0 && hue < 60.0) {
		tmpr = c;
		tmpg = x;
		tmpb = 0;
	}
	else if (hue >= 60.0 && hue < 120.0) {
		tmpr = x;
		tmpg = c;
		tmpb = 0;
	}
	else if (hue >= 120.0 && hue < 180.0) {
		tmpr = 0;
		tmpg = c;
		tmpb = x;
	}
	else if (hue >= 180.0 && hue < 240.0) {
		tmpr = 0;
		tmpg = x;
		tmpb = c;
	}
	else if (hue >= 240.0 && hue < 300.0) {
		tmpr = x;
		tmpg = 0;
		tmpb = c;
	}
	else {
		tmpr = c;
		tmpg = 0;
		tmpb = x;
	}

	b = utility::saturate_cast((tmpb + m) * 255.0);
	g = utility::saturate_cast((tmpg + m) * 255.0);
	r = utility::saturate_cast((tmpr + m) * 255.0);


}


void filter::xFilters::RGB2HSL(uint8_t& b, uint8_t& g, uint8_t& r, double& hue, double& saturation, double& luminance) {

	double dr{}, db{}, dg{};

	dr = r / 255.0;
	db = b / 255.0;
	dg = g / 255.0;


	double max = max(max(db, dg), dr);
	double min = min(min(db, dg), dr);
	double delta = max - min;
	luminance = (max + min) / 2.0;

	if (max == min) {
		hue = 0;
		saturation = 0;
	}
	else {

		saturation = luminance >= 0.5 ? delta / (2.0 - max - min) : delta / (max + min);

		if (max == dr) {
			hue = 60.0 * (dg - db) / delta;
		}
		else if (max == dg) {
			hue = 60.0 * ((db - dr) / delta + 2.0);
		}
		else {
			hue = 60.0 * ((dr - dg) / delta + 4.0);
		}
	}

	if (hue < 0)
		hue += 360.0;
}


void filter::xFilters::YCbCr2RGB(double& Y, double& Cb, double& Cr, uint8_t& b, uint8_t& g, uint8_t& r) {
	double tmpb{}, tmpg{}, tmpr{};

	Y = Y / 255.0;
	Cb = (Cb - 128) / 255.0;
	Cr = (Cr - 128) / 255.0;


	tmpr = 255 * (Y + 1.402 * Cr);
	tmpg = 255 * (Y - 0.344136 * Cb - 0.714136 * Cr);
	tmpb = 255 * (Y + 1.772 * Cb);

	tmpr = max(0, min(255, tmpr));
	tmpg = max(0, min(255, tmpg));
	tmpb = max(0, min(255, tmpb));


	b = utility::saturate_cast(tmpb);
	g = utility::saturate_cast(tmpg);
	r = utility::saturate_cast(tmpr);
}

void filter::xFilters::RGB2YCbCr(uint8_t& b, uint8_t& g, uint8_t& r, double& Y, double& Cb, double& Cr) {
	Y = 0.299 * r + 0.587 * g + 0.114 * b;
	Cb = 128.0 - 0.168736 * r - 0.331264 * g + 0.5 * b;
	Cr = 128.0 + 0.5 * r - 0.418688 * g - 0.081312 * b;

}


void filter::xFilters::RGB2YUV(uint8_t& b, uint8_t& g, uint8_t& r, double& y, double& u, double& v) {


	y = (0.299 * r + 0.587 * g + 0.114 * b);
	u = (-0.14713 * r - 0.28886 * g + 0.436 * b);
	v = (0.615 * r - 0.51499 * g - 0.10001 * b);

	// Ensure YUV values are within the appropriate range [0, 255]
	y = max(0, min(255, y));
	u = max(0, min(255, u + 128)); // Shift to [0, 255] range
	v = max(0, min(255, v + 128)); // Shift to [0, 255] range

}


void  filter::xFilters::YUV2RGB(double& Y, double& U, double& V, uint8_t& b, uint8_t& g, uint8_t& r) {
	Y -= 16;
	U -= 128;
	V -= 128;

	r = utility::saturate_cast((298 * Y + 409 * V + 128));
	g = utility::saturate_cast((298 * Y - 100 * U - 208 * V + 128));
	b = utility::saturate_cast((298 * Y + 516 * U + 128));
}




void filter::xFilters::GeenericColorSpace2BGR(xImage* pInImage, xImage* pOutImage, void(*GenFunIn)(uint8_t&, uint8_t&, uint8_t&, double&, double&, double&),
	void(*GenFunOut)(double&, double&, double&, uint8_t&, uint8_t&, uint8_t&),
	void(*GenFunSlider)(double&, double&, double&, int, int, int),
	int C1, int C2, int C3) {

	double tmpC1{}, tmpC2{}, tmpC3{};
	double r{}, g{}, b{};

	int index{};

	auto pInImageData = pInImage->GetData();
	auto pOutImageData = pOutImage->GetData();
	auto RowBytes = pInImage->GetRowBytes();

	for (int i{}; i < pInImage->GetHeight(); ++i) {
		index = i * RowBytes;
		for (int j{}; j < pInImage->GetWidth(); ++j) {

			GenFunIn(pInImageData[index], pInImageData[index + 1], pInImageData[index + 2], tmpC1, tmpC2, tmpC3);

			GenFunSlider(tmpC1, tmpC2, tmpC3, C1, C2, C3);

			GenFunOut(tmpC1, tmpC2, tmpC3, pOutImageData[index], pOutImageData[index + 1], pOutImageData[index + 2]);

			index += 3;
		}
	}

}



////////////////////////////////  Convolution technique //////////////////////////////////////////////



void filter::xFilters::Convolution1Ch(xImage* pInImage, xImage* pOutImage, std::vector<std::vector<float>>& kernel) {

	if (pInImage && pOutImage && pInImage->IsValid() && pInImage->GetOrder() == COLOR_ORDER::_BYTE) {

		auto imageW = pInImage->GetWidth();
		auto imageH = pInImage->GetHeight();
		auto srcRowBytes = pInImage->GetRowBytes();
		auto srcData = pInImage->GetData();

		if (!pOutImage->IsValid()) {
			pOutImage->allocateSpace(imageW, imageH, COLOR_ORDER::_FLOAT);
		}

		auto dstData = pOutImage->GetData();
		auto dstRowBytes = pOutImage->GetRowBytes();


		int radius = (int)kernel.size() / 2;

		int leftradius{ radius }, rightradius{ radius }, topradius{ radius }, bottomradius{ radius };

		float b{}, g{}, r{};

		float ksum{};

		for (int y{}; y < imageH; ++y) {

			bottomradius = min(radius, y);

			topradius = y + radius > imageH - 1 ? --topradius : topradius;

			for (int x{}; x < imageW; ++x) {

				leftradius = min(radius, x);

				rightradius = x + radius > imageW - 1 ? --rightradius : rightradius;

				for (int ky = -1 * bottomradius; ky <= topradius; ++ky) {

					int currentrowposforkernel = ((y + ky) * srcRowBytes) + x;

					int ik = ky + radius;

					for (int kx = -1 * leftradius; kx <= rightradius; ++kx) {

						int jk = kx + radius;

						int imagePosChange = kx;

						b += (srcData[currentrowposforkernel + imagePosChange] * kernel[ik][jk]);
						ksum += kernel[ik][jk];

					}
				}

				((float*)(dstData + y * dstRowBytes))[x] = b;

				b = 0;

			}
			rightradius = radius;

		}


	}

}


void filter::xFilters::SepConvolution1D(xImage* pInImage, xImage* pOutImage, const std::vector<int>& kernelX, const std::vector<int>& kernelY) {


	if (pInImage != nullptr && pInImage->IsValid() && pInImage->GetOrder() == COLOR_ORDER::_BYTE) {

		int width = pInImage->GetWidth();
		int height = pInImage->GetHeight();
		int inRowbytes = pInImage->GetRowBytes();

		auto inData = pInImage->GetData();


		if (pOutImage->IsValid() == false) {
			pOutImage->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
		}

		int outRowBytes = pOutImage->GetRowBytes();

		// allocate temprorary space
		float* tmpData = new float[height * width];

		auto outData = pOutImage->GetData();

		int radiusX = int(kernelX.size() / 2);
		int radiusY = int(kernelY.size() / 2);

		int leftradius{ radiusY }, rightradius{ radiusY }, topRadius{ radiusX }, botomRadius{ radiusX };

		int currentRowPixel{};

		float accumlator{};
		int ksum{};


		for (int y{}; y < height; ++y) {

			auto tmpCurrentRow = (tmpData + y * width);
			auto inCurrentRow = (inData + y * inRowbytes);
			for (int x{}; x < width; ++x) {

				leftradius = min(radiusY, x);

				rightradius = x + radiusY > width - 1 ? --rightradius : rightradius;

				for (int kx = -1 * leftradius; kx <= rightradius; ++kx) {

					accumlator += kernelY[kx + radiusY] * inCurrentRow[x + kx];
					ksum += kernelY[kx + radiusY];
				}
				tmpCurrentRow[x] = accumlator;

				accumlator = 0;
				ksum = 0;

			}

			rightradius = radiusX;
		}

		for (int x{}; x < width; ++x) {

			for (int y{}; y < height; ++y) {

				topRadius = min(y, radiusY);
				botomRadius = y + radiusY > height - 1 ? --botomRadius : botomRadius;

				currentRowPixel = y * outRowBytes;


				for (int ky = -1 * topRadius; ky <= botomRadius; ++ky) {

					//int currentColComp = (y + ky) * rowbytes + x;
					int currentColComp = (y + ky) * width + x;
					accumlator += kernelX[ky + radiusY] * tmpData[currentColComp];
					ksum += kernelX[ky + radiusX];
				}

				((float*)(outData + currentRowPixel))[x] = accumlator;

				ksum = 0;
				accumlator = 0;
			}
			botomRadius = radiusY;
		}


		delete[] tmpData;
	}

}




void filter::xFilters::SepConvolution64F1D(xImage* pInImage, xImage* pOutImage, const std::vector<int>& kernelX, const std::vector<int>& kernelY) {

	if (pInImage != nullptr && pInImage->IsValid() && pInImage->GetOrder() == COLOR_ORDER::_DOUBLE) {

		int width = pInImage->GetWidth();
		int height = pInImage->GetHeight();
		int inRowbytes = pInImage->GetRowBytes();

		auto inData = pInImage->GetData();

		if (pOutImage->IsValid()) {
			pOutImage->deAllocateSpace();
		}
		else {
			pOutImage->allocateSpace(width, height, COLOR_ORDER::_DOUBLE);
		}

		int outRowBytes = pOutImage->GetRowBytes();

		// allocate temprorary space
		double* tmpData = new double[height * width];

		auto outData = pOutImage->GetData();

		int radiusX = int(kernelX.size() / 2);
		int radiusY = int(kernelY.size() / 2);

		int leftradius{ radiusY }, rightradius{ radiusY }, topRadius{ radiusX }, botomRadius{ radiusX };

		int currentRowPixel{};

		double accumlator{};
		int ksum{};


		for (int y{}; y < height; ++y) {

			auto tmpCurrentRow = (tmpData + y * width);
			auto inCurrentRow = (double*)(inData + y * inRowbytes);
			for (int x{}; x < width; ++x) {

				leftradius = min(radiusY, x);

				rightradius = x + radiusY > width - 1 ? --rightradius : rightradius;

				for (int kx = -1 * leftradius; kx <= rightradius; ++kx) {

					accumlator += kernelY[kx + radiusY] * inCurrentRow[x + kx];
					ksum += kernelY[kx + radiusY];
				}
				tmpCurrentRow[x] = accumlator;

				accumlator = 0;
				ksum = 0;

			}

			rightradius = radiusX;
		}

		for (int x{}; x < width; ++x) {

			for (int y{}; y < height; ++y) {

				topRadius = min(y, radiusY);
				botomRadius = y + radiusY > height - 1 ? --botomRadius : botomRadius;

				currentRowPixel = y * outRowBytes;


				for (int ky = -1 * topRadius; ky <= botomRadius; ++ky) {

					//int currentColComp = (y + ky) * rowbytes + x;
					int currentColComp = (y + ky) * width + x;
					accumlator += kernelX[ky + radiusY] * tmpData[currentColComp];
					ksum += kernelX[ky + radiusX];
				}

				((double*)(outData + currentRowPixel))[x] = accumlator;

				ksum = 0;
				accumlator = 0;
			}
			botomRadius = radiusY;
		}


		delete[] tmpData;
	}



}





////////////////////////// Edge Detection //////////////////////////////////////

void filter::xFilters::getSobelKernels(std::vector<int>& _kx, std::vector<int>& _ky, int dx, int dy, int _ksize)
{
	int i, j, ksizeX = _ksize, ksizeY = _ksize;
	if (ksizeX == 1 && dx > 0)
		ksizeX = 3;
	if (ksizeY == 1 && dy > 0)
		ksizeY = 3;

	auto kx = _kx.data();
	auto ky = _ky.data();


	std::vector<int> kerI(max(ksizeX, ksizeY) + 1);


	for (int k = 0; k < 2; k++)
	{
		auto* kernel = k == 0 ? &kx : &ky;
		int order = k == 0 ? dx : dy;
		int ksize = k == 0 ? ksizeX : ksizeY;



		if (ksize == 1)
			kerI[0] = 1;
		else if (ksize == 3)
		{
			if (order == 0)
				kerI[0] = 1, kerI[1] = 2, kerI[2] = 1;
			else if (order == 1)
				kerI[0] = -1, kerI[1] = 0, kerI[2] = 1;
			else
				kerI[0] = 1, kerI[1] = -2, kerI[2] = 1;
		}
		else
		{
			int oldval, newval;
			kerI[0] = 1;
			for (i = 0; i < ksize; i++)
				kerI[i + 1] = 0;

			for (i = 0; i < ksize - order - 1; i++)
			{
				oldval = kerI[0];
				for (j = 1; j <= ksize; j++)
				{
					newval = kerI[j] + kerI[j - 1];
					kerI[j - 1] = oldval;
					oldval = newval;
				}
			}

			for (i = 0; i < order; i++)
			{
				oldval = -kerI[0];
				for (j = 1; j <= ksize; j++)
				{
					newval = kerI[j - 1] - kerI[j];
					kerI[j - 1] = oldval;
					oldval = newval;
				}
			}
		}

		if (k == 0) {
			for (int i{}; i < kerI.size() - 1; ++i) {
				_kx.push_back(kerI[i]);
			}
		}
		else {
			for (int i{}; i < kerI.size() - 1; ++i) {
				_ky.push_back(kerI[i]);
			}
		}
	}
}



void filter::xFilters::getPrewittKernel(std::vector<int>& kX, std::vector<int>& kY, int kSize) {

	int radius = kSize / 2;

	for (int i{}; i < kSize; ++i) {
		if (i < radius) {
			kX.push_back(1);
		}
		else if (i == radius) {
			kX.push_back(0);
		}
		else {
			kX.push_back(-1);
		}
	}

	for (int i{}; i < kSize; ++i) {
		kY.push_back(1);
	}

}


void filter::xFilters::Sobel(xImage* pInImage, xImage* pOutImage, int kernelSize) {

	xImage gray;

	xImage grad_x;
	xImage grad_y;

	xImage magnitude;

	xImage out;
	std::vector<int> kernelX;
	std::vector<int> kernelY;


	utility::ConvertToGray(pInImage, &gray);

	getSobelKernels(kernelX, kernelY, 1, 0, kernelSize);

	SepConvolution1D(&gray, &grad_x, kernelX, kernelY);

	SepConvolution1D(&gray, &grad_y, kernelY, kernelX);

	utility::magnitude32F1Ch(&grad_x, &grad_y, &magnitude);

	utility::ConvertTo32F28U1Ch(&magnitude, &out);

	//utility::addWeighted1D(&grad_x, &grad_y, &magnitude, 0.5f, 0.5f, 0.0f);

	//utility::Merge(&grad_y, &grad_y, &grad_y, pOutImage);


	//utility::Merge(&grad_x, &grad_x, &grad_x, pOutImage);


	utility::Merge(&out, &out, &out, pOutImage);

}



void filter::xFilters::Prewitt(xImage* pInImage, xImage* pOutImage, int kernelSize) {
	xImage gray;

	xImage grad_x;
	xImage grad_y;

	xImage magnitude;
	xImage out;

	std::vector<int> kernelX;
	std::vector<int> kernelY;

	getPrewittKernel(kernelX, kernelY, kernelSize);


	utility::ConvertToGray(pInImage, &gray);

	SepConvolution1D(&gray, &grad_x, kernelX, kernelY);

	SepConvolution1D(&gray, &grad_y, kernelY, kernelX);

	utility::magnitude32F1Ch(&grad_x, &grad_y, &magnitude);

	utility::ConvertTo32F28U1Ch(&magnitude, &out);

	//utility::addWeighted1D(&grad_x, &grad_y, &magnitude, 0.5f, 0.5f, 0.0f);

	//utility::Merge(&grad_y, &grad_y, &grad_y, pOutImage);


	//utility::Merge(&grad_x, &grad_x, &grad_x, pOutImage);

	utility::Merge(&out, &out, &out, pOutImage);


}

inline void filter::xFilters::GenerateGaussianKernelClassic(std::vector<std::vector<float>>& kernel, int iSizeKernel) {

	float sigma = max(iSizeKernel / 2.0f, 1.0f);// 0.3 * ((newKernelSize - 1) * 0.5 - 1) + 0.8;
	float twoQuadratSigma = 2 * sigma * sigma;
	float quadratSigmaPi = twoQuadratSigma * float(std::numbers::pi);

	int step = iSizeKernel / 2;
	float sum{};

	if (kernel.empty()) {
		kernel.resize(iSizeKernel);
		for (int i{}; i < iSizeKernel; ++i) {
			kernel[i].resize(iSizeKernel);
		}
	}

	/////
	for (int i = -step; i <= step; ++i) {
		for (int j = -step; j <= step; ++j) {
			float quadraticSum = -(float)(i * i + j * j);
			kernel[i + step][j + step] = std::expf(quadraticSum / twoQuadratSigma) / quadratSigmaPi;
			sum += kernel[i + step][j + step];
		}
	}

	for (int i{}; i != iSizeKernel; ++i) {
		for (int j{}; j != iSizeKernel; ++j) {
			kernel[i][j] /= sum;
		}
	}

}

void filter::xFilters::Canny(xImage* pInImage, xImage* pOutImage) {

	xImage gray;

	xImage grad_x;
	xImage grad_y;
	xImage angleF;
	xImage magnitude;
	xImage blurred;
	xImage blurredU8;
	xImage nonMaxSuppression;
	xImage doubleThreshold;


	utility::ConvertToGray(pInImage, &gray);

	std::vector<std::vector<float>> kernel;

	GenerateGaussianKernelClassic(kernel, 3);

	Convolution1Ch(&gray, &blurred, kernel);

	utility::ConvertTo32F28U1Ch(&blurred, &blurredU8);

	std::vector<std::vector<float>> kernelx{
		{-1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 }
	};

	std::vector<std::vector<float>> kernely{
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
	};

	Convolution1Ch(&blurredU8, &grad_x, kernelx);

	Convolution1Ch(&blurredU8, &grad_y, kernely);

	utility::magnitude32F1Ch(&grad_x, &grad_y, &magnitude);

	utility::angleFormedGradients32F1Ch(&grad_x, &grad_y, &angleF);

	utility::nonMaximumSupression32F1ChAdv(&magnitude, &angleF, &nonMaxSuppression);

	utility::doubleThreshold1Ch(&nonMaxSuppression, &doubleThreshold);

	utility::edgeTrackingByHysteresis(&doubleThreshold);


	utility::Merge(&doubleThreshold, &doubleThreshold, &doubleThreshold, pOutImage);

}


//////////////////////////////////////////////////////////////////////////////////



bool filter::xFilters::CopyOriginalToTransformedImage(xImage* pInImage, xImage* pOutImage) {

	if (pInImage != nullptr && pOutImage != nullptr && pInImage->IsValid() && pOutImage->IsValid())
	{
		const auto imageW = pInImage->GetWidth();
		const auto imageH = pInImage->GetHeight();


		auto inImageData = (uint8_t*)pInImage->GetData();
		auto outImageData = (uint8_t*)pOutImage->GetData();
		auto inRowBytes = pInImage->GetRowBytes();
		auto outRowBytes = pOutImage->GetRowBytes();

		std::memcpy(outImageData, inImageData, imageH * inRowBytes);

		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////

