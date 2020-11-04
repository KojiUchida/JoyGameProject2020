#pragma once

struct Vector2;
struct Rect {

	float top;
	float left;
	float bottom;
	float right;

	Rect();
	Rect(const float top, const float left, const float bottom, const float right);
	Rect(const Vector2& topLeft, const Vector2& bottmRight);

	/* ”ÍˆÍ“à‚©H */
	bool InRange(const Vector2& pos);
};

