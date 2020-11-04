#pragma once

struct Color4 {

	float r;
	float g;
	float b;
	float a;

	/* コンストラクタ */
	Color4();
	Color4(const float r, const float g, const float b, const float a);

	/* 中身を0~1にclampするメソッド */
	Color4& saturate();

	/* 単項演算子 */
	Color4 operator + () const;
	Color4 operator - () const;

	/* 代入演算子 */
	Color4 operator = (const Color4& c2);
	Color4 operator += (const Color4& c2) const;
	Color4 operator -= (const Color4& c2) const;
	Color4 operator *= (const Color4& c2) const;
	Color4 operator /= (const Color4& c2) const;
	Color4 operator *= (const float s) const;
	Color4 operator /= (const float s) const;

};

/* 二項演算子 */
const Color4 operator + (const Color4& c1, const Color4& c2);
const Color4 operator - (const Color4& c1, const Color4& c2);
const Color4 operator * (const Color4& c1, const Color4& c2) ;
const Color4 operator * (const float s, const Color4& c);
const Color4 operator * (const Color4& c, float s);
const Color4 operator / (const Color4& c1, const Color4& c2);
const Color4 operator / (const Color4& c, float s);

