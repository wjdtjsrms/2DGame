#ifndef INCLUDED_VECTOR2_H
#define INCLUDED_VECTOR2_H

class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	Vector2(double x, double y);
	Vector2(const Vector2&);

	void operator+=(const Vector2&);
	void operator-=(const Vector2&);
	void operator=(const Vector2&);

	void setAdd(const Vector2&, const Vector2&);
	void setSub(const Vector2&, const Vector2&);

	// a + (u * ab) + (v * ac) 보간 함수
	void setInterporation(
		const Vector2& a,
		const Vector2& ab,
		const Vector2& ac,
		double u,
		double v);

	double x;
	double y;
};
#endif