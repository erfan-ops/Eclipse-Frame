#pragma once

#include <array>

class Star {
public:
	float orgx;
	float orgy;
	float x;
	float y;
	float speedx;
	float speedy;
	float radius;
	std::array<float, 4> color;

	Star(const Star&) = delete;
	Star();
	Star(float x, float y, float speed, float angle, float radius, std::array<float, 4> color);
	void move(float& dt) noexcept;
	void render(const int* nSegments) const;

	void realRender(const int* nSegments) const;

	void emptyRender(const int* nSegments) const;

	static void (Star::* renderFunc)(const int*) const;

};
