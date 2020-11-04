#include "Rect.h"
#include "Vector2.h"

Rect::Rect() :
	top(0),
	left(0),
	bottom(0),
	right(0) {
}

Rect::Rect(const float top, const float left, const float bottom, const float right) :
	top(top),
	left(left),
	bottom(bottom),
	right(right) {
}

Rect::Rect(const Vector2& topLeft, const Vector2& bottmRight) :
	top(topLeft.y),
	bottom(bottmRight.y),
	left(topLeft.x),
	right(bottmRight.x) {
}

bool Rect::InRange(const Vector2& pos) {
	if (pos.x < left || right < pos.x)return false;
	if (pos.y < top || bottom < pos.y)return false;
	return true;
}
