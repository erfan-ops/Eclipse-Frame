#pragma once

#include <vector>
#include <array>

class BG {
private:
    const std::vector<std::array<float, 4>>& colors;
    const int& width;
    const int& height;

public:
    BG(const BG&) = delete;
    BG(const std::vector<std::array<float, 4>>& colors, const int& width, const int& height);
    void fillGradient() const;
    static void fillGradient(const std::vector<std::array<float, 4>>& colors, const int& width, const int& height);
};
