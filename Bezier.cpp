#include "stdafx.h"
#include "Bezier.h"
using namespace std;
Bezier::Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3)
	: p0(p0), p1(p1), p2(p2), p3(p3)
{

}

void Bezier::draw(SDL_Renderer* renderer) const
{
	// TODO
	const int n = 20;
	const float invN = 1.0f / n;
	Vector2 pLast = cubicBezier(p0, p1, p2, p3, 0);
	
	for (int i = 1; i <= n; ++i) {
		float t = i * invN;  // Using invN to increment t uniformly because it didnt work in the for loop :(
		Vector2 pFinal = cubicBezier(p0, p1, p2, p3, t);
		SDL_RenderDrawLine(renderer, pLast.x, pLast.y, pFinal.x, pFinal.y);
		
		float curveLength = getSegmentLength(pLast, pFinal);

		pLast = pFinal;
	}
		
}


Vector2 Bezier::quadraticBezier(Vector2 p0, Vector2 p1, Vector2 p2, const float t) {
	Vector2 point;
	
	point.x = pow(1 - t, 2) * p0.x + (1 - t) * 2 * t * p1.x + pow(t, 2) * p2.x;
	point.y = pow(1 - t, 2) * p0.y + (1 - t) * 2 * t * p1.y + pow(t, 2) * p2.y;
	return point;
}

Vector2 Bezier::cubicBezier(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t) const {
	Vector2 point;

	point.x = pow(float(1 - t), 3) * p0.x + 3 * pow(float(1 - t), 2) * t * p1.x + 3 * (1 - t) * pow(t, 2) * p2.x + pow(t, 3) * p3.x;
	point.y = pow(float(1 - t), 3) * p0.y + 3 * pow(float(1 - t), 2) * t * p1.y + 3 * (1 - t) * pow(t, 2) * p2.y + pow(t, 3) * p3.y;
	
	return point;
}

float Bezier::getSegmentLength(Vector2 a, Vector2 b) const {
	float segmentLength = sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	return segmentLength;
}