#include "Easing.h"
#define _USE_MATH_DEFINES
#include <cmath>

double  Easing::EaseInSine(double t) {
	return sin(1.5707963 * t);
}

double  Easing::EaseOutSine(double t) {
	return 1 + sin(1.5707963 * (--t));
}

double  Easing::EaseInOutSine(double t) {
	return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
}

double Easing::EaseInQuad(double t) {
	return t * t;
}

double  Easing::EaseOutQuad(double t) {
	return t * (2 - t);
}

double  Easing::EaseInOutQuad(double t) {
	return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

double  Easing::EaseInCubic(double t) {
	return t * t * t;
}

double  Easing::EaseOutCubic(double t) {
	return 1 + (--t) * t * t;
}

double  Easing::EaseInOutCubic(double t) {
	return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
}

double  Easing::EaseInQuart(double t) {
	t *= t;
	return t * t;
}

double  Easing::EaseOutQuart(double t) {
	t = (--t) * t;
	return 1 - t * t;
}

double  Easing::EaseInOutQuart(double t) {
	if (t < 0.5) {
		t *= t;
		return 8 * t * t;
	} else {
		t = (--t) * t;
		return 1 - 8 * t * t;
	}
}

double  Easing::EaseInQuint(double t) {
	double t2 = t * t;
	return t * t2 * t2;
}

double  Easing::EaseOutQuint(double t) {
	double t2 = (--t) * t;
	return 1 + t * t2 * t2;
}

double  Easing::EaseInOutQuint(double t) {
	double t2;
	if (t < 0.5) {
		t2 = t * t;
		return 16 * t * t2 * t2;
	} else {
		t2 = (--t) * t;
		return 1 + 16 * t * t2 * t2;
	}
}

double Easing::EaseInExpo(double t) {
	return (pow(2, 8 * t) - 1) / 255;
}

double Easing::EaseOutExpo(double t) {
	return 1 - pow(2, -8 * t);
}

double  Easing::EaseInOutExpo(double t) {
	if (t < 0.5) {
		return (pow(2, 16 * t) - 1) / 510;
	} else {
		return 1 - 0.5 * pow(2, -16 * (t - 0.5));
	}
}

double Easing::EaseInCirc(double t) {
	return 1 - sqrt(1 - t);
}

double  Easing::EaseOutCirc(double t) {
	return sqrt(t);
}

double  Easing::EaseInOutCirc(double t) {
	if (t < 0.5) {
		return (1 - sqrt(1 - 2 * t)) * 0.5;
	} else {
		return (1 + sqrt(2 * t - 1)) * 0.5;
	}
}

double  Easing::EaseInBack(double t) {
	return t * t * (2.70158 * t - 1.70158);
}

double  Easing::EaseOutBack(double t) {
	return 1 + (--t) * t * (2.70158 * t + 1.70158);
}

double  Easing::EaseInOutBack(double t) {
	if (t < 0.5) {
		return t * t * (7 * t - 2.5) * 2;
	} else {
		return 1 + (--t) * t * 2 * (7 * t + 2.5);
	}
}

double  Easing::EaseInElastic(double t) {
	double t2 = t * t;
	return t2 * t2 * sin(t * M_PI * 4.5);
}

double Easing::EaseOutElastic(double t) {
	double t2 = (t - 1) * (t - 1);
	return 1 - t2 * t2 * cos(t * M_PI * 4.5);
}

double Easing::EaseInOutElastic(double t) {
	double t2;
	if (t < 0.45) {
		t2 = t * t;
		return 8 * t2 * t2 * sin(t * M_PI * 9);
	} else if (t < 0.55) {
		return 0.5 + 0.75 * sin(t * M_PI * 4);
	} else {
		t2 = (t - 1) * (t - 1);
		return 1 - 8 * t2 * t2 * sin(t * M_PI * 9);
	}
}

double Easing::EaseInBounce(double t) {
	return pow(2, 6 * (t - 1)) * abs(sin(t * M_PI * 3.5));
}

double  Easing::EaseOutBounce(double t) {
	return 1 - pow(2, -6 * t) * abs(cos(t * M_PI * 3.5));
}

double Easing::EaseInOutBounce(double t) {
	if (t < 0.5) {
		return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * M_PI * 7));
	} else {
		return 1 - 8 * pow(2, -8 * t) * abs(sin(t * M_PI * 7));
	}
}

