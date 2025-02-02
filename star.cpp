#include "star.h"


Star::Star(float x, float y, float speedx, float speedy) : x(x), y(y), orgx(x), orgy(y), speedx(speedx), speedy(speedy) {}
Star::Star() : x(0), y(0), orgx(0), orgy(0), speedx(0), speedy(0) {}

void Star::move(float& dt) noexcept {
    this->orgx += this->speedx * dt;
    this->orgy += this->speedy * dt;

    float xdis = this->orgx - this->x;
    float ydis = this->orgy - this->y;

    this->x += xdis * dt;
    this->y += ydis * dt;
}
