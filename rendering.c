#include "rendering.h"

#include <Windows.h>
#include <gl/GL.h>

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
