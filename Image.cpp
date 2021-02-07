#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "File.h"
#include "Vector2.h"

// a�� ���ų� ū 2�� ����� ���Ѵ�.
static int powerOfTwo(int a) {
	ASSERT(a < 0x40000000);
	int r = 1;
	while (r < a) {
		r *= 2;
	}
	return r;
}

Image::Image(const char* fileName)
	:mWidth(0), 
	mHeight(0), 
	mTextureWidth(0), 
	mTextureHeight(0),
	mTexture(0)
{
	File f(fileName);
	// dword �������� 12, 16 ��° ������ ����Ǿ� �ִ�.
	mHeight = f.getUnsigned(12);
	mWidth = f.getUnsigned(16);

	unsigned* buffer = new unsigned[mWidth* mHeight];
	for (int i = 0; i < mWidth * mHeight; i++)
	{
		// 4byte �� �����͸� �д´�.
		buffer[i] = f.getUnsigned(128 + i * 4);
	}

	mTextureWidth = powerOfTwo(mWidth);
	mTextureHeight = powerOfTwo(mHeight);

	// buffer[mWidth * mHeight] -> createTexture() ->mTexture[mTextureWidth * mTextureHeight]
	Framework::instance().createTexture(
		&mTexture,
		mTextureWidth,
		mTextureHeight,
		buffer,
		mWidth,
		mHeight);

	SAFE_DELETE_ARRAY(buffer);
}

Image::~Image()
{
	Framework::instance().destroyTexture(&mTexture);
}

int Image::height() const
{
	return mHeight;
}

int Image::width() const
{
	return mWidth;
}

void Image::draw(
	int dstX, // dst dstination ����� ��
	int dstY,
	int srcX, // src source �о� ���� ��
	int srcY,
	int width,
	int height) const
{
	double x0 = static_cast<double> (dstX);
	double y0 = static_cast<double> (dstY);
	double x1 = static_cast<double> (dstX + width);
	double y1 = static_cast<double> (dstY + height);

	Vector2 p0(x0, y0); // �»�
	Vector2 p1(x1, y0); // ���
	Vector2 p2(x0, y1); // ����
	Vector2 p3(x1, y1); // ����

	// u,v ������ 0.0 ~ 1.0
	double rcpTw = 1.0 / static_cast<double> (mTextureWidth);
	double rcpTh = 1.0 / static_cast<double> (mTextureHeight);

	// �ؽ��� ��ǥ ����
	double u0 = static_cast<double> (srcX) * rcpTw;
	double u1 = static_cast<double> (srcX + width) *rcpTw;
	double v0 = static_cast<double> (srcY) * rcpTh;
	double v1 = static_cast<double> (srcY + height) *rcpTh;

	Vector2 t0(u0, v0); // �»�
	Vector2 t1(u1, v0); // ���
	Vector2 t2(u0, v1); // ����
	Vector2 t3(u1, v1); // ����
	
	Framework f = Framework::instance();

	f.setTexture(mTexture); // �ؽ��� ����
	f.setBlendMode(Framework::BLEND_LINEAR); // ���� �ռ�
	
	f.drawTriangle2D(&p0.x, &p1.x, &p2.x, &t0.x, &t1.x, &t2.x); // �»� ��� ����
	f.drawTriangle2D(&p3.x, &p1.x, &p2.x, &t3.x, &t1.x, &t2.x); // ���� ��� ����

	return;
}


void Image::draw() const
{
	// ��ü ȭ�� �׸���
	draw(0, 0, 0, 0, mWidth, mHeight);
}