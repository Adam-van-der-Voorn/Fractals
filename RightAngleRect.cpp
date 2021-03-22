#include "RightAngleRect.h"
#include "Vec2.h"


RightAngleRect::RightAngleRect(const Vec2& pa, const Vec2& pb) {
	if (pa.x < pb.x) {
		w_edge = pa.x;
		e_edge = pb.x;
	}
	else {
		e_edge = pa.x;
		w_edge = pb.x;
	}
	if (pa.y < pb.y) {
		n_edge = pa.y;
		s_edge = pb.y;
	}
	else {
		s_edge = pa.y;
		n_edge = pb.y;
	}
}

RightAngleRect RightAngleRect::fromSize(const Vec2& tl_pos, const Vec2& size)
{
	return RightAngleRect(tl_pos, tl_pos + size);
}

RightAngleRect RightAngleRect::translate(const Vec2& translation) const
{
	return RightAngleRect(
		tL() + translation,
		bR() + translation
	);
}


RightAngleRect::RightAngleRect()
{
	n_edge = 0;
	s_edge = 0;
	e_edge = 0;
	w_edge = 0;
}


double RightAngleRect::top() const
{
	return n_edge;
}

double RightAngleRect::bottom() const
{
	return s_edge;
}

double RightAngleRect::right() const
{
	return e_edge;
}

double RightAngleRect::left() const
{
	return w_edge;
}

Vec2 RightAngleRect::tL() const
{
	return { left(), top() };
}

Vec2 RightAngleRect::tR() const
{
	return { right(), top() };
}

Vec2 RightAngleRect::bL() const
{
	return { left(), bottom() };
}

Vec2 RightAngleRect::bR() const
{
	return { right(), bottom() };
}

Vec2 RightAngleRect::size() const
{
	return bR() - tL();
}

bool operator== (const RightAngleRect& a, const RightAngleRect& b) {
	return (
		a.left() == b.left() &&
		a.bottom() == b.bottom() &&
		a.right() == b.right() &&
		a.top() == b.top()
		);
}
bool operator!= (const RightAngleRect& a, const RightAngleRect& b) {
	return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const RightAngleRect& rect)
{
	os << "( top: " << rect.top() << ", bottom: " << rect.bottom() << ", left: " << rect.left() << ", right: " << rect.right() << ')';
	return os;
}
