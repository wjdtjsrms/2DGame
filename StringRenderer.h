#ifndef INCLUDED_STRING_RENDERER_H
#define INCLUDED_STRING_RENDERER_H

class Image;

class StringRenderer
{
public:
	static StringRenderer* Instance();
	void draw(int x, int y, const char* string, unsigned color = 0xffffff) const;
	static void create(const char* fontFileName); // 대체 생성자
	static void destroy(); // 대체 소멸자

private:
	StringRenderer(const char* fontFileName);
	StringRenderer(StringRenderer&); // 복사생성자
	~StringRenderer();

	Image* mImage;
	static StringRenderer* mInstance;
};
#endif