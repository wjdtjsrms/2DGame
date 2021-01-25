#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "File.h"

Image::Image(const char* filename)
	:mWidth(0), mHeight(0), mData(0)
{
	File f(filename);
	// dword 형식으로 12, 16 번째 번지에 저장되어 있다.
	mHeight = f.getUnsigned(12);
	mWidth = f.getUnsigned(16);
	mData = new unsigned[mWidth* mHeight];
	for (int i = 0; i < mWidth*mHeight; i++)
	{
		// 4byte 씩 데이터를 읽는다.
		mData[i] = f.getUnsigned(128 + i * 4);
	}
}

Image::~Image()
{
	SAFE_DELETE_ARRAY(mData);
}

int Image::height() const
{
	return mHeight;
}

int Image::width() const
{
	return mWidth;
}

namespace { // 현재 cpp 에서만 유효하다.

	unsigned blend(unsigned src, unsigned dst) {
		unsigned srcA = (src & 0xff000000) >> 24;
		unsigned srcR = src & 0xff0000;
		unsigned srcG = src & 0x00ff00;
		unsigned srcB = src & 0x0000ff;
		unsigned dstR = dst & 0xff0000;
		unsigned dstG = dst & 0x00ff00;
		unsigned dstB = dst & 0x0000ff;
		unsigned r = (srcR - dstR) * srcA / 255 + dstR;
		unsigned g = (srcG - dstG) * srcA / 255 + dstG;
		unsigned b = (srcB - dstB) * srcA / 255 + dstB;
		return (r & 0xff0000) | (g & 0x00ff00) | b;
	}

}

void Image::draw(
	int dstX, // dst dstination 출력할 곳
	int dstY,
	int srcX, // src source 읽어 오는 곳
	int srcY,
	int width,
	int height) const
{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {

			unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];	        // 전경
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];	// 배경
			*dst = blend(src, *dst);	                                    // 알파 계산
		}
	}

	return;
}

void Image::drawWithFixedColor(
	int dstX, 
	int dstY,
	int srcX, 
	int srcY,
	int width,
	int height,
	unsigned color) const
{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();

	// 색깔 추출
	unsigned srcR = color & 0xff0000;
	unsigned srcG = color & 0x00ff00;
	unsigned srcB = color & 0x0000ff;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
			unsigned srcA = src & 0xff; // 청색 체널을 이용한다 0이면 투명 255이면 불투명
			unsigned dstR = *dst & 0xff0000;
			unsigned dstG = *dst & 0x00ff00;
			unsigned dstB = *dst & 0x0000ff;
			unsigned r = (srcR - dstR) * srcA / 255 + dstR;
			unsigned g = (srcG - dstG) * srcA / 255 + dstG;
			unsigned b = (srcB - dstB) * srcA / 255 + dstB;
			*dst = (r & 0xff0000) | (g & 0x00ff00) | b;
		}
	}
	return;
}

void Image::draw() const
{
	// 전체 화면 그리기
	draw(0, 0, 0, 0, mWidth, mHeight);
}