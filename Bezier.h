#pragma once

#include "Vector2.h"

class Bezier
{
public:
	Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);

	void draw(SDL_Renderer *renderer) const;
	
	float curveLength;
	float getSegmentLength(Vector2 a, Vector2 b) const;
	Vector2 quadraticBezier(Vector2 p0, Vector2 p1, Vector2 p2, const float n);
	Vector2 cubicBezier(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float n)  const ;
	Vector2 p0, p1, p2, p3; //maybe make private and create function in application .cpp for accessing? (data encapsulation)

private:
	
};
