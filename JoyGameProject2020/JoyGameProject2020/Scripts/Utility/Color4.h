#pragma once

struct Color4 {

	float r;
	float g;
	float b;
	float a;

	/* �R���X�g���N�^ */
	Color4();
	Color4(const float r, const float g, const float b, const float a);

	/* ���g��0~1��clamp���郁�\�b�h */
	Color4& saturate();

	/* �P�����Z�q */
	Color4 operator + () const;
	Color4 operator - () const;

	/* ������Z�q */
	Color4 operator = (const Color4& c2);
	Color4 operator += (const Color4& c2) const;
	Color4 operator -= (const Color4& c2) const;
	Color4 operator *= (const Color4& c2) const;
	Color4 operator /= (const Color4& c2) const;
	Color4 operator *= (const float s) const;
	Color4 operator /= (const float s) const;

};

/* �񍀉��Z�q */
const Color4 operator + (const Color4& c1, const Color4& c2);
const Color4 operator - (const Color4& c1, const Color4& c2);
const Color4 operator * (const Color4& c1, const Color4& c2) ;
const Color4 operator * (const float s, const Color4& c);
const Color4 operator * (const Color4& c, float s);
const Color4 operator / (const Color4& c1, const Color4& c2);
const Color4 operator / (const Color4& c, float s);

