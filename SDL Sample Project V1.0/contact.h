#pragma once
#include "vec2.h"

class contact {

public:
	vec2<float> normal;
	float distance;

	contact::contact(vec2<float> n, float d) {

		normal = n;
		distance = d;

	}

};