#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "StringRenderer.h"
#include "Image.h"

// static 변수를 cpp 파일에서 사용할수 있게한다.
StringRenderer* StringRenderer::mInstance = 0;

StringRenderer::StringRenderer(const char* fontFileName)
	:mImage(0)
{
	mImage = new Image(fontFileName);
}

StringRenderer::~StringRenderer()
{
	SAFE_DELETE(mImage);
}

void StringRenderer::create(const char* fontFileName)
{
	// 객체가 두개 이상 생성되어서는 안된다.
	STRONG_ASSERT(!mInstance && "StringRenderer::create() called twice!");
	mInstance = new StringRenderer(fontFileName);
}

void StringRenderer::destroy()
{
	SAFE_DELETE(mInstance);
}

StringRenderer* StringRenderer::Instance()
{
	return mInstance;
}

void StringRenderer::draw(int x, int y, const char* s, unsigned color) const {
	const int CHAR_WIDTH = 8;
	const int CHAR_HEIGHT = 16;

	// 표시 위치 지정
	int dstX = x * CHAR_WIDTH;
	int dstY = y * CHAR_HEIGHT;

	Framework f = Framework::instance();
	if (dstY < 0 || dstY >= (int)f.height()) {	// y축 범위 확인
		return;
	}
	// 문자열의 끝이 나올때까지
	for (int i = 0; s[i] != '\0'; ++i) {
		// X축 범위 확인
		if (dstX >= 0 && (dstX + CHAR_WIDTH) < f.width()) {
			// 아스키 범위 확인
			int t = (int)s[i];
			if (t < 32 || t >= 128) {
				t = 127; // 범위를 벗어난 문자는 127로 한다.
			}
			t -= 32; // 아스키 32부터(' ') 시작한다.
			int srcX = (t % 16) * CHAR_WIDTH; //X는 이미지 폭으로 나눈 나머지, 16개씩 한줄임
			int srcY = (t / 16) * CHAR_HEIGHT; //Y는 이미지 폭으로 나눈 몫
			mImage->drawWithFixedColor(dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, color);
		}
		dstX += CHAR_WIDTH; // 한 문자 만큼 움직인다.
	}
}