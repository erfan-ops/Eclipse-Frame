#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include "BackGround.h"

BG::BG(const std::vector<std::array<float, 4>>& colors, const int& width, const int& height) : colors(colors), width(width), height(height) {}

void BG::fillGradient() const {
    glBegin(GL_QUADS);
    for (size_t i = 0; i < this->colors.size() - 1; ++i) {
        // Extract start and end colors
        auto& [r1, g1, b1, a1] = this->colors[i];
        auto& [r2, g2, b2, a2] = this->colors[i + 1];

        // Calculate start and end Y positions
        float yStart = static_cast<float>(i) / (this->colors.size() - 1) * this->height;
        float yEnd = static_cast<float>(i + 1) / (this->colors.size() - 1) * this->height;

        // Set the gradient colors and vertices
        glColor4f(r1, g1, b1, a1);
        glVertex2f(0.0f, yStart);
        glVertex2f(static_cast<float>(this->width), yStart);

        glColor4f(r2, g2, b2, a2);
        glVertex2f(static_cast<float>(this->width), yEnd);
        glVertex2f(0.0f, yEnd);
    }
    glEnd();
}

void BG::fillGradient(const std::vector<std::array<float, 4>>& colors, const int& width, const int& height) {
    glBegin(GL_QUADS);
    for (size_t i = 0; i < colors.size() - 1; ++i) {
        // Extract start and end colors
        auto& [r1, g1, b1, a1] = colors[i];
        auto& [r2, g2, b2, a2] = colors[i + 1];

        // Calculate start and end Y positions
        float yStart = static_cast<float>(i) / (colors.size() - 1) * height;
        float yEnd = static_cast<float>(i + 1) / (colors.size() - 1) * height;

        // Set the gradient colors and vertices
        glColor4f(r1, g1, b1, a1);
        glVertex2f(0.0f, yStart);
        glVertex2f(static_cast<float>(width), yStart);

        glColor4f(r2, g2, b2, a2);
        glVertex2f(static_cast<float>(width), yEnd);
        glVertex2f(0.0f, yEnd);
    }
    glEnd();
}
