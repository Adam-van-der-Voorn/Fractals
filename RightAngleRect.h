#pragma once
class Vec2;

class RightAngleRect
{
public:
	RightAngleRect();
	RightAngleRect(Vec2 pa, Vec2 pb);
	static RightAngleRect fromSize(Vec2 tl_pos, double width, double height);

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