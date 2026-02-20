#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

auto SetupWindow() -> GLFWwindow*;

