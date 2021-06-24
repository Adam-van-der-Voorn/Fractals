#pragma once
#include <ostream>

struct Vec2;

class RightAngleRect
{
public:
	RightAngleRect();
	RightAngleRect(const Vec2& pa, const Vec2& pb);
	static RightAngleRect fromSize(const Vec2& tl_pos, const Vec2& size);
	
	RightAngleRect translate(const Vec2& translation) const;

	double top() const;
	double bottom() const;
	double right() const;
	double left() const;

	Vec2 tL() const;
	Vec2 tR() const;
	Vec2 bL() const;
	Vec2 bR() const;

	Vec2 size() const;

private:
	double n_edge, s_edge, e_edge, w_edge;
};

std::ostream& operator<<(std::ostream& os, const RightAngleRect& rect);

bool operator== (const RightAngleRect& a, const RightAngleRect& b);
bool operator!= (const RightAngleRect& a, const RightAngleRect& b);
