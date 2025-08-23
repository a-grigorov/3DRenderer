#include <math.h>
#include "vector.h"

vec3_t rotate_around_x(vec3_t point, float angle) {
	vec3_t rotatedPoint = {
        point.x,
		point.z * cos(angle) - point.y * sin(angle),
		point.z * sin(angle) + point.y * cos(angle),
	};

	return rotatedPoint;
}

vec3_t rotate_around_y(vec3_t point, float angle) {
	vec3_t rotatedPoint = {
		point.x * cos(angle) - point.z * sin(angle),
		point.y,
		point.x * sin(angle) + point.z * cos(angle)
	};

	return rotatedPoint;
}

vec3_t rotate_around_z(vec3_t point, float angle) {
	vec3_t rotatedPoint = {
		point.x * cos(angle) - point.y * sin(angle),
		point.x * sin(angle) + point.y * cos(angle),
		point.z
	};

	return rotatedPoint;
}