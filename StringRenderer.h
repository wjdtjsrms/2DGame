#ifndef INCLUDED_STRING_RENDERER_H
#define INCLUDED_STRING_RENDERER_H

class Image;

class StringRenderer
{
public:
	static StringRenderer* Instance();
	void draw(int x, int y, const char* string, unsigned color = 0xffffff) const;
	static void create(const char* fontFileName); // ��ü ������
	static void destroy(); // ��ü �Ҹ���

private:
	StringRenderer(const char* fontFileName);
	StringRenderer(StringRenderer&); // ���������
	~StringRenderer();

	Image* mImage;
	static StringRenderer* mInstance;
};
#endif