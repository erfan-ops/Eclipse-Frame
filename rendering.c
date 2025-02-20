#include "rendering.h"

#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#define TAU_F 6.2831853071f

void aaline(const float* r, const float* g, const float* b, const float* a, const float* startX, const float* startY, const float* endX, const float* endY, const float* lineWidth) {
	glLineWidth(*lineWidth);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(*r, *g, *b, *a);
	glBegin(GL_LINES);
	glVertex2f(*startX, *startY);
	glVertex2f(*endX, *endY);
	glEnd();
}

void filledCircle(const float* centerX, const float* centerY, const float* radius, const float* r, const float* g, const float* b, const float* a, const int* nSegments) {
	const float nSegments_f = (float)(*nSegments);
	glColor4f(*r, *g, *b, *a);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(*centerX, *centerY);
	for (int i = 0; i <= *nSegments; i++) {
		float theta = TAU_F * (float)i / nSegments_f;
		float x = *radius * cosf(theta);
		float y = *radius * sinf(theta);
		glVertex2f(x + *centerX, y + *centerY);
	}
	glEnd();
}

