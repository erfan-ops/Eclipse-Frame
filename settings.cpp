#include "settings.h"
#include <fstream>
#include <nlohmann/json.hpp>

// Function to load settings from a JSON file
Settings loadSettings(const std::string& filename) {
	std::ifstream file(filename);
	nlohmann::json j;
	file >> j;

	Settings settings;

	settings.targetFPS = j["fps"];
	settings.backGroundColors = j["background-colors"].get<std::vector<std::array<float, 4>>>();

	settings.stars.radius = j["stars"]["radius"];
	settings.stars.color = j["stars"]["color"].get<std::array<float, 4>>();
	settings.stars.count = j["stars"]["count"];
	settings.stars.minSpeed = j["stars"]["min-speed"];
	settings.stars.maxSpeed = j["stars"]["max-speed"];
	settings.stars.lineColor = j["stars"]["line-color"].get<std::array<float, 4>>();
	settings.stars.lineMinWidth = j["stars"]["line-min-width"];
	settings.stars.lineMaxWidth = j["stars"]["line-max-width"];
	settings.stars.nSegments = j["stars"]["segments"];
	settings.stars.draw = j["stars"]["draw-stars"];

	settings.mouseRadius = j["mouse-radius"];
	settings.starsLineRadius = j["stars-line-radius"];

	return settings;
}
