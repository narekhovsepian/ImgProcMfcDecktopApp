#include "pch.h"
#include "utility.h"

double utility::atan_f64(double y, double x)
{
	double ax = std::abs(x), ay = std::abs(y);
	double a, c, c2;
	if (ax >= ay)
	{
		c = ay / (ax + (double)DBL_EPSILON);
		c2 = c * c;
		a = (((atan2_p7 * c2 + atan2_p5) * c2 + atan2_p3) * c2 + atan2_p1) * c;
	}
	else
	{
		c = ax / (ay + (double)DBL_EPSILON);
		c2 = c * c;
		a = 90.f - (((atan2_p7 * c2 + atan2_p5) * c2 + atan2_p3) * c2 + atan2_p1) * c;
	}
	if (x < 0)
		a = 180.f - a;
	if (y < 0)
		a = 360.f - a;
	return a;
}





void utility::ConvertToGray(xImage* pInImage, xImage* pOutImage) {

	if (pInImage && pOutImage && pInImage->IsValid()) {

		int width = pInImage->GetWidth();
		int height = pInImage->GetHeight();
		int RowBytesSrc = pInImage->GetRowBytes();

		auto srcData = pInImage->GetData();

		if (pOutImage->IsValid()) {
			pOutImage->deAllocateSpace();
		}
		else {
			pOutImage->allocateSpace(width, height, COLOR_ORDER::_BYTE);
		}

		int RowBytesDst = pOutImage->GetRowBytes();
		auto dstData = pOutImage->GetData();


		for (int y{}; y < height; ++y) {
			auto srcRow = (srcData + y * RowBytesSrc);
			auto dstRow = (dstData + y * RowBytesDst);
			for (int x{}, i{}; x < width; ++x, i += 3) {
				dstRow[x] = (UCHAR)((srcRow[i] + srcRow[i + 1] + srcRow[i + 2]) / 3);
			}
		}

	}

}




bool utility::Split(xImage* srcBGR, xImage* outB, xImage* outG, xImage* outR) {

	if (!srcBGR) {
		return false;
	}

	if (srcBGR->IsValid() == false) {
		return false;
	}

	if (!outB && !outG && !outR) {
		return false;
	}

	int width = srcBGR->GetWidth();
	int height = srcBGR->GetHeight();
	int RowBytesSrc = srcBGR->GetRowBytes();
	auto srcData = srcBGR->GetData();

	int RowBytesDst{};
	uint8_t* outBData{ nullptr }, * outGData{ nullptr }, * outRData{ nullptr };

	if (outB) {
		if (outB->IsValid() == false)
			outB->allocateSpace(width, height, COLOR_ORDER::_BYTE);
		outBData = outB->GetData();
		RowBytesDst = outB->GetRowBytes();
	}

	if (outG) {
		if (outG->IsValid() == false)
			outG->allocateSpace(width, height, COLOR_ORDER::_BYTE);
		outGData = outG->GetData();
		RowBytesDst = outG->GetRowBytes();

	}

	if (outR) {
		if (outR->IsValid() == false)
			outR->allocateSpace(width, height, COLOR_ORDER::_BYTE);
		outRData = outR->GetData();
		RowBytesDst = outR->GetRowBytes();

	}

	int index{};

	for (int y{}; y < height; ++y) {
		for (int x{}, index = y * RowBytesSrc, indexDst = y * RowBytesDst; x < width; ++x, index += 3, ++indexDst) {

			if (outBData) {
				outBData[indexDst] = srcData[index];
			}

			if (outGData) {
				outGData[indexDst] = srcData[index + 1];
			}

			if (outRData) {
				outRData[indexDst] = srcData[index + 2];
			}

		}
	}

	return true;
}

bool utility::Merge(xImage* srcB, xImage* srcG, xImage* srcR, xImage* dstBGR) {

	int width{};
	int height{};
	int RowBytesDst{};
	int RowBytesSrc{};
	uint8_t* srcBData{ nullptr };
	uint8_t* srcGData{ nullptr };
	uint8_t* srcRData{ nullptr };
	uint8_t* dstBGRData{ nullptr };

	if (!dstBGR) {
		return false;
	}

	if (srcB && srcG && srcR)/* B G R*/ {
		if ((srcB->GetWidth() != srcG->GetWidth() || srcB->GetHeight() != srcG->GetHeight()) || (srcG->GetWidth() != srcR->GetWidth() || srcG->GetHeight() != srcR->GetHeight()) ||
			(srcB->GetWidth() != srcR->GetWidth() || srcB->GetHeight() != srcR->GetHeight())) {
			return false;
		}

		width = srcB->GetWidth();
		height = srcB->GetHeight();
		RowBytesSrc = srcB->GetRowBytes();

		srcBData = srcB->GetData();
		srcGData = srcG->GetData();
		srcRData = srcR->GetData();
	}
	else if (srcB && !srcG && !srcR)/* B */ {

		width = srcB->GetWidth();
		height = srcB->GetHeight();
		RowBytesSrc = srcB->GetRowBytes();

		srcBData = srcB->GetData();


	}
	else if (!srcB && srcG && !srcR) /* G */ {
		width = srcG->GetWidth();
		height = srcG->GetHeight();
		RowBytesSrc = srcG->GetRowBytes();

		srcGData = srcG->GetData();

	}
	else if (!srcB && !srcG && srcR) /* R */ {
		width = srcR->GetWidth();
		height = srcR->GetHeight();
		RowBytesSrc = srcR->GetRowBytes();

		srcRData = srcR->GetData();

	}
	else if (srcB && srcG && !srcR) /* B G */ {
		if (srcB->GetWidth() != srcG->GetWidth() || srcB->GetHeight() != srcG->GetHeight()) {
			return false;
		}

		width = srcB->GetWidth();
		height = srcB->GetHeight();
		RowBytesSrc = srcB->GetRowBytes();


		srcBData = srcB->GetData();
		srcGData = srcG->GetData();
	}
	else if (srcB && !srcG && srcR) /* B R */ {
		if (srcB->GetWidth() != srcR->GetWidth() || srcB->GetHeight() != srcR->GetHeight()) {
			return false;
		}

		width = srcB->GetWidth();
		height = srcB->GetHeight();
		RowBytesSrc = srcB->GetRowBytes();


		srcBData = srcB->GetData();
		srcRData = srcR->GetData();
	}
	else if (!srcB && srcG && srcR) /* G R */ {
		if (srcG->GetWidth() != srcR->GetWidth() || srcG->GetHeight() != srcR->GetHeight()) {
			return false;
		}

		width = srcG->GetWidth();
		height = srcG->GetHeight();
		RowBytesSrc = srcG->GetRowBytes();


		srcGData = srcG->GetData();
		srcRData = srcR->GetData();
	}
	else
	{
		return false;
	}

	if (dstBGR->IsValid() == false)
		dstBGR->allocateSpace(width, height, COLOR_ORDER::_BGR);

	RowBytesDst = dstBGR->GetRowBytes();

	dstBGRData = dstBGR->GetData();

	int index{};

	for (int y{}; y < height; ++y) {

		for (int x{}, index = y * RowBytesDst, indexSrc = y * RowBytesSrc; x < width; ++x, index += 3, ++indexSrc) {

			dstBGRData[index] = srcBData ? srcBData[indexSrc] : 0;
			dstBGRData[index + 1] = srcGData ? srcGData[indexSrc] : 0;
			dstBGRData[index + 2] = srcRData ? srcRData[indexSrc] : 0;
		}
	}


	return true;
}



bool utility::addWeighted1Ch(xImage* src1Image, xImage* src2Image, xImage* dst, float alpha, float beta, float gamma) {

	if (src1Image->IsValid() && src2Image->IsValid()) {

		if (src1Image->GetWidth() == src2Image->GetWidth() && src1Image->GetHeight() == src2Image->GetHeight()) {


			int width = src1Image->GetWidth();
			int height = src1Image->GetHeight();
			int srcRowBytes = src1Image->GetRowBytes();

			auto src1Data = src1Image->GetData();
			auto src2Data = src2Image->GetData();



			if (dst->IsValid() == false) {
				dst->allocateSpace(width, height, COLOR_ORDER::_DOUBLE);
			}

			auto dstData = dst->GetData();

			auto dstRowBytes = dst->GetRowBytes();


			for (int y{}; y < height; ++y) {
				auto tmpSrc1Data = (int*)(src1Data + y * srcRowBytes);
				auto tmpSrc2Data = (int*)(src2Data + y * srcRowBytes);
				auto tmpDstData = (double*)(dstData + y * dstRowBytes);
				for (int x{}; x < width; ++x) {
					tmpDstData[x] = std::abs(tmpSrc1Data[x]) * alpha + std::abs(tmpSrc2Data[x]) * beta + gamma;
				}
			}


			return true;
		}
	}


	return false;

}




void utility::magnitude32F1Ch(xImage* src1, xImage* src2, xImage* dst) {

	if (src1 && src2 && dst && src1->IsValid() && src2->IsValid()) {

		if (src1->GetWidth() == src2->GetWidth() && src1->GetHeight() == src2->GetHeight() &&
			src1->GetOrder() == COLOR_ORDER::_FLOAT && src2->GetOrder() == COLOR_ORDER::_FLOAT) {


			int width = src1->GetWidth();
			int height = src1->GetHeight();
			int srcRowBytes = src1->GetRowBytes();

			auto src1Data = src1->GetData();
			auto src2Data = src2->GetData();

			if (dst->IsValid()) {
				dst->deAllocateSpace();
			}
			else {
				dst->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
			}

			auto dstData = dst->GetData();

			int dstRowBytes = dst->GetRowBytes();

			for (int y{}; y < height; ++y) {
				auto* dstD = (float*)(dstData + (y * dstRowBytes));

				auto* src1D = (float*)(src1Data + (y * srcRowBytes));
				auto* src2D = (float*)(src2Data + (y * srcRowBytes));

				for (int x{}; x < width; ++x) {
					dstD[x] = std::sqrt(src1D[x] * src1D[x] + src2D[x] * src2D[x]);
					//dstD[x] = std::abs(src1D[x]) + std::abs(src2D[x]);
				}
			}

		}
	}

}




void utility::doubleThreshold1Ch(xImage* src, xImage* dst, float minThrRatio, float MaxThrRatio) {

	if (src && dst && src->IsValid() && src->GetOrder() == COLOR_ORDER::_FLOAT) {


		int width = src->GetWidth();
		int height = src->GetHeight();
		int srcRowBytes = src->GetRowBytes();

		if (dst->IsValid() == false) {
			dst->allocateSpace(width, height, COLOR_ORDER::_BYTE);
		}
		auto srcData = src->GetData();
		auto dstData = dst->GetData();

		double max{};

		auto iterData = (float*)srcData;
		for (int y{}; y < height * width; ++y) {
			if (max < iterData[y]) {
				max = iterData[y];
			}
		}



		double highThr = max * MaxThrRatio;
		double lowThr = highThr * minThrRatio;



		int dstRowBytes = dst->GetRowBytes();

		for (int y{}; y < height; ++y) {

			auto* tSrcData = (float*)(srcData + (y * srcRowBytes));
			auto* tDstData = (dstData + (y * dstRowBytes));
			for (int x{}; x < width; ++x) {
				if (tSrcData[x] >= highThr) {
					tDstData[x] = 255;
				}
				else if (tSrcData[x] >= lowThr) {
					tDstData[x] = 75;
				}
				else {
					tDstData[x] = 0;
				}
			}
		}


	}



}


bool utility::angleFormedGradients32F1Ch(xImage* gradX, xImage* gradY, xImage* angle) {


	if (gradX && gradY && gradX->IsValid() && gradY->IsValid() && gradX->GetOrder() == COLOR_ORDER::_FLOAT && gradY->GetOrder() == COLOR_ORDER::_FLOAT) {

		if (gradX->GetWidth() == gradY->GetWidth() && gradX->GetHeight() == gradY->GetHeight()) {

			int width = gradX->GetWidth();
			int height = gradY->GetHeight();
			int srcRowBytes = gradY->GetRowBytes();

			if (angle->IsValid()) {
				angle->deAllocateSpace();
			}
			else {
				angle->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
			}

			auto gradXData = gradX->GetData();
			auto gradYData = gradY->GetData();
			auto angleData = angle->GetData();

			auto angleRowBytes = angle->GetRowBytes();

			const auto scale =  180.0f / (float)std::numbers::pi;

			for (int y{}; y < height; ++y) {

				auto* tmpgradXData = (float*)(gradXData + (y * srcRowBytes));
				auto* tmpgradYData = (float *)(gradYData + (y * srcRowBytes));
				auto* tmpAngleData = (float*)(angleData + (y * angleRowBytes));

				for (int x{}; x < width; ++x) {

					tmpAngleData[x] = std::atan2(tmpgradYData[x], tmpgradXData[x]) * scale;

					//tmpAngleData[x] = tmpAngleData[x] < 0 ? tmpAngleData[x] + 180.0f : tmpAngleData[x];

					//tmpAngleData[x] = atan_f64(tmpgradYData[x], tmpgradXData[x]);

				}
			}

			return true;

		}
	}

	return false;
}


bool utility::ConvertTo32F28U1Ch(xImage* src, xImage* dst) {


	if (src && src->IsValid() && src->GetOrder() == COLOR_ORDER::_FLOAT && dst) {

		const int width = src->GetWidth();
		const int height = src->GetHeight();

		if (!dst->IsValid())
			dst->allocateSpace(width, height, COLOR_ORDER::_BYTE);

		auto* srcData = src->GetData();
		auto* dstData = dst->GetData();

		const auto srcRowBytes = src->GetRowBytes();
		const auto dstRowBytes = dst->GetRowBytes();

		for (auto y = 0; y < height; y++)
		{
			auto* srcRow = (float*)(srcData + y * srcRowBytes);
			uint8_t* dstRow = (dstData + y * dstRowBytes);

			for (auto x = 0; x < width; x++)
			{
				dstRow[x] = saturate_cast(srcRow[x]);

			}

		}

		return true;
	}

	return false;
}
bool utility::ConvertTo64F28U1Ch(xImage* src, xImage* dst) {

	if (src && src->IsValid() && src->GetOrder() == COLOR_ORDER::_DOUBLE && dst) {

		const int width = src->GetWidth();
		const int height = src->GetHeight();

		if (dst->IsValid()) {
			dst->deAllocateSpace();
		}
		else {
			dst->allocateSpace(width, height, COLOR_ORDER::_BYTE);
		}
		auto* srcData = src->GetData();
		auto* dstData = dst->GetData();

		const auto srcRowBytes = src->GetRowBytes();
		const auto dstRowBytes = dst->GetRowBytes();

		for (auto y = 0; y < height; y++)
		{
			auto* srcRow = (double*)(srcData + y * srcRowBytes);
			uint8_t* dstRow = (dstData + y * dstRowBytes);

			for (auto x = 0; x < width; x++)
			{
				dstRow[x] = saturate_cast(srcRow[x]);

			}

		}

		return true;
	}

	return false;
}

bool  utility::ConvertTo16S28U1Ch(xImage* src, xImage* dst) {


	if (src && src->IsValid() && src->GetOrder() == COLOR_ORDER::_SHORT && dst) {

		const int width = src->GetWidth();
		const int height = src->GetHeight();

		if (!dst->IsValid())
			dst->allocateSpace(width, height, COLOR_ORDER::_BYTE);

		auto* srcData = src->GetData();
		auto* dstData = dst->GetData();

		const auto srcRowBytes = src->GetRowBytes();
		const auto dstRowBytes = dst->GetRowBytes();

		for (auto y = 0; y < height; y++)
		{
			auto* srcRow = (short*)(srcData + y * srcRowBytes);
			uint8_t* dstRow = (dstData + y * dstRowBytes);

			for (auto x = 0; x < width; x++)
			{
				dstRow[x] = saturate_cast((int)srcRow[x]);
			}

		}

		return true;
	}

	return false;

}



bool utility::ConvertTo64Int28U1Ch(xImage* src, xImage* dst) {

	if (src && src->IsValid() && src->GetOrder() == COLOR_ORDER::_INT && dst) {

		const int width = src->GetWidth();
		const int height = src->GetHeight();

		if (!dst->IsValid())
			dst->allocateSpace(width, height, COLOR_ORDER::_BYTE);

		auto* srcData = src->GetData();
		auto* dstData = dst->GetData();

		const auto srcRowBytes = src->GetRowBytes();
		const auto dstRowBytes = dst->GetRowBytes();

		for (auto y = 0; y < height; y++)
		{
			auto* srcRow = (int*)(srcData + y * srcRowBytes);
			uint8_t* dstRow = (dstData + y * dstRowBytes);

			for (auto x = 0; x < width; x++)
			{
				dstRow[x] = saturate_cast(srcRow[x]);
			}

		}

		return true;
	}

	return false;

}



void  utility::nonMaximumSupression32F1ChAdv(xImage* magnitude, xImage* angle, xImage* nonMaxSupp) {

	if (magnitude && angle && nonMaxSupp && magnitude->GetOrder() == COLOR_ORDER::_FLOAT && angle->GetOrder() == COLOR_ORDER::_FLOAT) {

		if (magnitude->GetWidth() == angle->GetWidth() && magnitude->GetHeight() == angle->GetHeight()) {

			int width = magnitude->GetWidth();
			int height = magnitude->GetHeight();
			int srcRowBytes = magnitude->GetRowBytes();

			if (!nonMaxSupp->IsValid()) {
				nonMaxSupp->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
			}

			auto MagnitudeData = magnitude->GetData();
			auto AngleData = angle->GetData();
			auto NonMaxSupData = nonMaxSupp->GetData();

			int outRowBytes = nonMaxSupp->GetRowBytes();


			float neighbor1{}, neighbor2{};
			float* tmp1Magnitude{ nullptr }, * tmp2Magnitude{ nullptr };
			for (int y{ 1 }; y < height - 1; ++y) {

				auto* angle = (float*)(AngleData + y * srcRowBytes);
				auto* mag = (float*)(MagnitudeData + y * srcRowBytes);
				auto* nonMaxSup = (float*)(NonMaxSupData + (y * outRowBytes));
				tmp1Magnitude = (float*)(MagnitudeData + ((y - 1) * srcRowBytes));
				tmp2Magnitude = (float*)(MagnitudeData + ((y + 1) * srcRowBytes));
				for (int x{ 1 }; x < width - 1; ++x) {

					if ((angle[x] >= -22.5 && angle[x] <= 22.5) || (angle[x] >= 157.5 || angle[x] <= -157.5)) {
						//neighbor1 = (mag[x - 1] * (1 - std::abs(angle[x] / 22.5))) + tmp1Magnitude[x - 1] * std::abs(angle[x] / 22.5);
						//neighbor2 = (mag[x + 1] * (1 - std::abs(angle[x] / 22.5))) + tmp2Magnitude[x + 1] * std::abs(angle[x] / 22.5);

						neighbor1 = mag[x - 1];
						neighbor2 = mag[x + 1];

					}
					else if ((angle[x] >= 22.5 && angle[x] <= 67.5) || (angle[x] >= -157.5 && angle[x] <= -112.5)) {

						neighbor1 = tmp1Magnitude[x + 1];// *(1 - std::abs((angle[x] - 45) / 22.5)) + tmp1Magnitude[x] * std::abs((angle[x] - 45) / 22.5);
						neighbor2 = tmp2Magnitude[x - 1];// *(1 - std::abs((angle[x] - 45) / 22.5)) + tmp2Magnitude[x] * std::abs((angle[x] - 45) / 22.5);
					}
					else if ((angle[x] >= 67.5 && angle[x] <= 112.5) || (angle[x] >= -112.5 && angle[x] <= -67.5)) {

						neighbor1 = tmp1Magnitude[x];// * (1 - std::abs((angle[x] - 90) / 22.5)) + tmp1Magnitude[x - 1] * std::abs((angle[x] - 90) / 22.5);
						neighbor2 = tmp2Magnitude[x];// *(1 - std::abs((angle[x] - 90) / 22.5)) + tmp2Magnitude[x + 1] * std::abs((angle[x] - 90) / 22.5);
					}
					else if ((angle[x] >= 112.5 && angle[x] <= 157.5) || (angle[x] >= -67.5 && angle[x] <= -22.5)) {

						neighbor1 = tmp1Magnitude[x - 1];// *(1 - std::abs((angle[x] - 135) / 22.5)) + mag[x - 1] * std::abs((angle[x] - 135) / 22.5);
						neighbor2 = tmp2Magnitude[x + 1];// *(1 - std::abs((angle[x] - 135) / 22.5)) + mag[x + 1] * std::abs((angle[x] - 135) / 22.5);
					}

					if (mag[x] >= neighbor1 && mag[x] >= neighbor2) {
						nonMaxSup[x] = mag[x];
					}
					else {
						nonMaxSup[x] = 0;
					}


				}
			}

		}
	}

}

void utility::nonMaximumSupression32F1Ch(xImage* magnitude, xImage* angle, xImage* nonMaxSupp) {

	if (magnitude && angle && nonMaxSupp && magnitude->GetOrder() == COLOR_ORDER::_FLOAT && angle->GetOrder() == COLOR_ORDER::_FLOAT) {

		if (magnitude->GetWidth() == angle->GetWidth() && magnitude->GetHeight() == angle->GetHeight()) {

			int width = magnitude->GetWidth();
			int height = magnitude->GetHeight();
			int srcRowBytes = magnitude->GetRowBytes();

			TRACE(L"\n\n\n %d   %d\n\n\n", width, height);

			if (!nonMaxSupp->IsValid()) {
				nonMaxSupp->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
			}

			auto MagnitudeData = magnitude->GetData();
			auto AngleData = angle->GetData();
			auto NonMaxSupData = nonMaxSupp->GetData();

			int outRowBytes = nonMaxSupp->GetRowBytes();

			float neighbor1{255}, neighbor2{255};
			float* tmp1Magnitude{ nullptr }, * tmp2Magnitude{ nullptr };
			for (int y{ 1 }; y < height - 1; ++y) {

				auto* angle = (float*)(AngleData + y * srcRowBytes);
				auto* mag = (float*)(MagnitudeData + y * srcRowBytes);
				auto* nonMaxSup = (float*)(NonMaxSupData + (y * outRowBytes));
				tmp1Magnitude = (float*)(MagnitudeData + ((y - 1) * srcRowBytes));
				tmp2Magnitude = (float*)(MagnitudeData + ((y + 1) * srcRowBytes));
				for (int x{ 1 }; x < width - 1; ++x) {

					if ((angle[x] >= 0 && angle[x] < 22.5) || (angle[x] >= 157.5 && angle[x] <= 180)) {

						neighbor1 = mag[x - 1];
						neighbor2 = mag[x + 1];

					}
					else if (angle[x] >= 22.5 && angle[x] < 67.5) {

						neighbor1 = tmp1Magnitude[x + 1];
						neighbor2 = tmp2Magnitude[x - 1];
					}
					else if (angle[x] >= 67.5 && angle[x] < 112.5) {

						neighbor1 = tmp1Magnitude[x];
						neighbor2 = tmp2Magnitude[x];
					}
					else if(angle[x] >= 112.5 && angle[x] < 157.5) {

						neighbor1 = tmp1Magnitude[x - 1];
						neighbor2 = tmp2Magnitude[x + 1];
					}

					if (mag[x] >= neighbor1 && mag[x] >= neighbor2) {
						nonMaxSup[x] = mag[x];
					}
					else {
						nonMaxSup[x] = 0;
					}


				}
			}

		}
	}

}








void utility::edgeTrackingByHysteresis(xImage* src) {

	if (src && src->IsValid() && src->GetOrder() == COLOR_ORDER::_BYTE) {

		int width = src->GetWidth();
		int height = src->GetHeight();
		int rowBytes = src->GetRowBytes();

		auto srcData = src->GetData();

		/*std::vector<std::pair<int, int>> edgeStack;

		int direction[8][2]{
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1}, {0, 1},
		{1, -1}, {1, 0}, {1, 1}
		};

		for (int i{ 1 }; i < height - 1; ++i) {
			auto tmpEdge = (double*)(srcData + i * rowBytes);
			for (int j{ 1 }; j < width - 1; ++j) {
				if (tmpEdge[j] == 255) {
					edgeStack.push_back({ j,i });
				}
			}
		}


		while (!edgeStack.empty()) {
			auto p = edgeStack.back();
			edgeStack.pop_back();

			for (int k{}; k < 8; ++k) {
				int x = p.first + direction[k][0];
				int y = p.second + direction[k][1];

				if (x >= 0 && x < width && y >= 0 && y < height) {
					auto tmpEdgeData = (double*)(srcData + y * rowBytes);
					if (tmpEdgeData[x] == 75) {
						tmpEdgeData[x] = 255;
						edgeStack.push_back({ x,y });
					}

				}

			}

		}

		for (int y{}; y < height; ++y) {
			auto tmpEdgeData = (double*)(srcData + y * rowBytes);
			for (int x{}; x < width; ++x) {

				if (tmpEdgeData[x] == 75) {
					tmpEdgeData[x] = 0;
				}
			}
		}*/



		for (int y{}; y < height; ++y) {

			auto posData = (srcData + y * rowBytes);
			for (int x{}; x < width; ++x) {

				if (posData[x] == 75) {
					bool connectedStrongEdge = false;
					for (int dy = -1; dy <= 1; ++dy) {
						auto tmpSrcData = (srcData + (y + dy) * rowBytes);

						for (int dx = -1; dx <= 1; ++dx) {

							if (tmpSrcData[x + dx] == 255) {
								connectedStrongEdge = true;
								break;
							}
						}
						if (connectedStrongEdge) {
							break;
						}
					}
					if (connectedStrongEdge) {
						posData[x] = 255;

					}
					else {
						posData[x] = 0;
					}
				}

			}
		}

	}

}


bool utility::range01(xImage* src, xImage* dst) {

	if (src && src->IsValid() && src->GetOrder() == COLOR_ORDER::_FLOAT) {



		auto srcData = src->GetData();
		int width = src->GetWidth();
		int height = src->GetHeight();
		int srcRowBytes = src->GetRowBytes();

		if (dst && dst->IsValid()) {
			dst->deAllocateSpace();
		}
		else {
			dst->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
		}
		auto dstData = dst->GetData();
		int dstRowBytes = dst->GetRowBytes();

		for (int y = 0; y < height; ++y) {
			auto rowSrcData = (float*)(srcData + y * srcRowBytes);
			auto rowDstData = (float*)(dstData + y * dstRowBytes);

			for (int x = 0; x < width; ++x) {
				rowDstData[x] = rowSrcData[x] / 255.0f;
			}
		}

		return true;
	}

	return false;
}



bool utility::range0255(xImage* src, xImage* dst) {

	if (src && dst && src->IsValid() && src->GetOrder() == COLOR_ORDER::_FLOAT) {



		auto srcData = src->GetData();
		int width = src->GetWidth();
		int height = src->GetHeight();
		int srcRowBytes = src->GetRowBytes();

		if (dst && dst->IsValid()) {
			dst->deAllocateSpace();
		}
		else {
			dst->allocateSpace(width, height, COLOR_ORDER::_FLOAT);
		}
		auto dstData = dst->GetData();
		int dstRowBytes = dst->GetRowBytes();

		auto srcData1 = (float*)src->GetData();
		
		float max{};

		for (int i{}; i < height * width; ++i) {
			if (max < srcData1[i]) {
				max = srcData1[i];
			}
		}

		TRACE(L"\n\n%f\n\n", max);


		for (int y = 0; y < height; ++y) {
			auto rowSrcData = (float*)(srcData + y * srcRowBytes);
			auto rowDstData = (float*)(dstData + y * dstRowBytes);

			for (int x = 0; x < width; ++x) {
				rowDstData[x] = rowSrcData[x] / max * 255.0f;
			}
		}

		return true;
	}

	return false;
}

template<typename _Ty>
bool utility::writeData2File(xImage* src) {

	if (src && src->IsValid()) {

		auto w = src->GetWidth();
		auto h = src->GetHeight();
		auto rowBytes = src->GetRowBytes();
		auto data = src->GetData();

		std::fstream fs{ L"./out.txt",std::ios::trunc | std::ios::in | std::ios::out };

		for (int y{}; y < h; ++y) {
			auto row = (_Ty*)(data + y * rowBytes);
			for (int x{}; x < w; ++x) {
				fs << row[x] << "\n";
			}
		}
		fs.close();

		return true;
	}

	return false;
}

void utility::console() {
	static HANDLE cons;
	CString dump;

	dump.Format(L"\n\n Hello \n\n");

	if (AllocConsole()) {
		cons = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	WriteConsole(cons, (LPCTSTR)dump, dump.GetLength(), 0, 0);
}