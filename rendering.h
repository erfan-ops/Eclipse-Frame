#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void aaline(const float* r, const float* g, const float* b, const float* a, const float* startX, const float* startY, const float* endX, const float* endY, const float* lineWidth);
void filledCircle(const float* centerX, const float* centerY, const float* radius, const float* r, const float* g, const float* b, const float* a, const int* nSegments);

#ifdef __cplusplus
}
#endif
