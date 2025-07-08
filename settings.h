#pragma once

#include <string>
#include <vector>
#include <array>

// Define a struct to represent your settings
struct Settings {
	float targetFPS;
	std::vector<std::array<float, 4>> backGroundColors;

	struct Stars {
		float radius;
		std::array<float, 4> color;
		int count;
		float minSpeed;
		float maxSpeed;
		std::array<float, 4> lineColor;
		float lineMinWidth;
		float lineMaxWidth;
		int nSegments;
		bool draw;
	} stars;

	float mouseRadius;
	float starsLineRadius;
};

// Function to load settings from a JSON file
Settings loadSettings(const std::string& filename);
