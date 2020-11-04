#include "Color4.h"
#include <math.h>
#include <cassert>

#pragma region コンストラクタ

Color4::Color4() :
	r(1), g(1), b(1), a(1)
{
}

Color4::Color4(const float r, const float g, const float b, const float a) :
	r(r), g(g), b(b), a(a)
{
}

#pragma endregion

//中身を0~1にclampするメソッド
Color4 & Color4::saturate()
{
	r = fminf(1.0f, fmaxf(0.0f, r));
	g = fminf(1.0f, fmaxf(0.0f, g));
	b = fminf(1.0f, fmaxf(0.0f, b));
	a = fminf(1.0f, fmaxf(0.0f, a));
	return *this;
}

#pragma region 単項演算子

Color4 Color4::operator+() const
{
	return *this;
}

Color4 Color4::operator-() const
{
	return Color4(-r, -g, -b, -a);
}

#pragma endregion

#pragma region 代入演算子

Color4 Color4::operator=(const Color4 & c2)
{
	r = c2.r;
	g = c2.g;
	b = c2.b;
	a = c2.a;
	return *this;
}

Color4 Color4::operator+=(const Color4 & c2) const
{
	return Color4(this->r + c2.r, this->b + c2.b, this->g + c2.g, this->a + c2.a);
}

Color4 Color4::operator-=(const Color4 & c2) const
{
	return Color4(this->r - c2.r, this->b - c2.b, this->g - c2.g, this->a + -c2.a);
}

Color4 Color4::operator*=(const Color4 & c2) const
{
	return Color4(this->r * c2.r, this->b * c2.b, this->g * c2.g, this->a * c2.a);

}

Color4 Color4::operator/=(const Color4 & c2) const
{
	if (c2.r == 0 || c2.g == 0 || c2.b == 0 || c2.a == 0) return *this;
	return Color4(this->r / c2.r, this->b / c2.b, this->g / c2.g, this->a / c2.a);
}

Color4 Color4::operator*=(const float s) const
{
	return Color4(this->r * s, this->b * s, this->g * s, this->a * s);

}

Color4 Color4::operator/=(const float s) const
{
	if (s == 0) return *this;
	return Color4(this->r / s, this->b / s, this->g / s, this->a / s);
}

#pragma endregion

#pragma region ニ項演算子

const Color4 operator+(const Color4 & c1, const Color4 & c2)
{
	return Color4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
}

const Color4 operator-(const Color4 & c1, const Color4 & c2)
{
	return Color4(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
}

const Color4 operator*(const Color4 & c1, const Color4 & c2)
{
	return Color4(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
}

const Color4 operator*(const float s, const Color4 & c)
{
	return Color4(s * c.r, s * c.g, s * c.b, s * c.a);
}

const Color4 operator*(const Color4 & c, const float s)
{
	return Color4(s * c.r, s * c.g, s * c.b, s * c.a);
}

const Color4 operator/(const Color4 & c1, const Color4 & c2)
{
	if (c2.r == 0 || c2.g == 0 || c2.b == 0 || c2.a == 0)return c1;
	return Color4(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a);
}

const Color4 operator/(const Color4 & c, float s)
{
	if (s == 0) return c;
	return Color4(c.r / s, c.g / s, c.b / s, c.a / s);
}

#pragma endregion