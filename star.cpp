#include "star.h"
#include "rendering.h"


Star::Star(float x, float y, float speed, float angle, float radius, std::array<float, 4> color)
    : x(x), y(y), orgx(x), orgy(y), speedx(cosf(angle)*speed), speedy(sinf(angle)*speed), radius(radius), color(color) {}
Star::Star()
    : x(0.0f), y(0.0f), orgx(0.0f), orgy(0.0f), speedx(0.0f), speedy(0.0f), radius(0.0f), color({0.0f, 0.0f , 0.0f , 0.0f}) {}

void Star::move(float& dt) noexcept {
    this->orgx += this->speedx * dt;
    this->orgy += this->speedy * dt;

    float xdis = this->orgx - this->x;
    float ydis = this->orgy - this->y;

    this->x += xdis * dt;
    this->y += ydis * dt;
}


void Star::render(const int* nSegments) const {
    // Call the function assigned to renderFunc
    (this->*renderFunc)(nSegments);
}

// Actual drawing logic
void Star::realRender(const int* nSegments) const {
    filledCircle(&x, &y, &radius, &color[0], &color[1], &color[2], &color[3], nSegments);
}

void Star::emptyRender(const int* nSegments) const {}

