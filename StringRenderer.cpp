#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "StringRenderer.h"
#include "Image.h"

// static ������ cpp ���Ͽ��� ����Ҽ� �ְ��Ѵ�.
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
	// ��ü�� �ΰ� �̻� �����Ǿ�� �ȵȴ�.
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

	// ǥ�� ��ġ ����
	int dstX = x * CHAR_WIDTH;
	int dstY = y * CHAR_HEIGHT;

	Framework f = Framework::instance();
	if (dstY < 0 || dstY >= (int)f.height()) {	// y�� ���� Ȯ��
		return;
	}
	// ���ڿ��� ���� ���ö�����
	for (int i = 0; s[i] != '\0'; ++i) {
		// X�� ���� Ȯ��
		if (dstX >= 0 && (dstX + CHAR_WIDTH) < f.width()) {
			// �ƽ�Ű ���� Ȯ��
			int t = (int)s[i];
			if (t < 32 || t >= 128) {
				t = 127; // ������ ��� ���ڴ� 127�� �Ѵ�.
			}
			t -= 32; // �ƽ�Ű 32����(' ') �����Ѵ�.
			int srcX = (t % 16) * CHAR_WIDTH; //X�� �̹��� ������ ���� ������, 16���� ������
			int srcY = (t / 16) * CHAR_HEIGHT; //Y�� �̹��� ������ ���� ��
			mImage->drawWithFixedColor(dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, color);
		}
		dstX += CHAR_WIDTH; // �� ���� ��ŭ �����δ�.
	}
}