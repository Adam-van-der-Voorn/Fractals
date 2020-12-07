#pragma once
#include "pi.h"

inline double toDeg(double rad) {
	double angle_in_deg = rad * (180 / m_pi);
	if (angle_in_deg < 0) {
		angle_in_deg += 360;
	}
	return angle_in_deg;
}

inline double toRad(double deg) {
	return deg * (m_pi / 180);
}