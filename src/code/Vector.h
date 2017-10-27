#pragma once

// Currently functionality is implemented in lua binding
class Vector
{
public:
	Vector()
		: x(0.0f)
		, y(0.0f) {}
	Vector(float x, float y)
		: x(x)
		, y(y) {}
	float x, y;
};