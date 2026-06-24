#pragma once
#include "engine/app.h"

#include <unordered_map>
#include <iostream>
#include <vector>
#include<numeric>
#include <string>
#include <array>
#include<numbers>
#include<random>
#define NRM_RGBA(r, g, b, a) r / 255.f, g / 255.f, b / 255.f, a / 255.f
GLFWwindow* game_window = nullptr;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
namespace utl {
	float MoveTowards(float current, float target, float maxDelta)
	{
		if (std::abs(target - current) <= maxDelta)
		{
			return target;
		}
		return current + std::copysign(maxDelta, target - current);
	}
	glm::vec2 MoveTowards(glm::vec2 current, glm::vec2 target, float maxDelta)
	{
		if (glm::distance(target, current) <= maxDelta)
		{
			return target;
		}
		return current + glm::normalize(target - current) * maxDelta;
	}
	namespace random {
		thread_local inline static std::random_device rng;
		template<typename t>
		t GetRandom(t min, t max) {
			std::uniform_real_distribution<t> dis(min, max);
			return dis(rng);
		}
	}
	namespace math {
		float lerp(float a, float b, float t) {
			return a + t * (b - a);
		}
		glm::vec2 lerp(glm::vec2 a, glm::vec2 b, float t) {
			return { lerp(a.x, b.x, t),
					lerp(a.y, b.y, t)
			};
		}
		glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t) {
			return { lerp(a.x, b.x, t),
					lerp(a.y, b.y, t),
					lerp(a.z, b.z, t)
			};
		}
		glm::vec4 lerp(glm::vec4 a, glm::vec4 b, float t) {
			return { lerp(a.x, b.x, t),
					lerp(a.y, b.y, t),
					lerp(a.z, b.z, t),
					lerp(a.w, b.w, t)
			};
		}
		float heading(glm::vec2 a) {
			return atan2f(a.y, a.x);
		}
		glm::vec2 from_angel(float angle) {
			return { glm::cos(angle), glm::sin(angle) };
		}
	}
	namespace input {

		inline static std::unordered_map<int, bool> keyState;
		inline static std::unordered_map<int, bool> prevKeyState;

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			keyState[key] = action == GLFW_PRESS;
			//if (action == GLFW_PRESS)
			//{
			//	keyState[key] = true;
			//	return;
			//}
			//keyState[key] = false;
		}

		void updateKeyStates()
		{
			for (auto& key : keyState)
			{
				prevKeyState[key.first] = key.second;
			}
		}

		bool GetButtonDown(int key)
		{
			return keyState[key] && (!prevKeyState[key]);
		}

		bool GetButtonHeld(int key)
		{
			return keyState[key];
		}
		bool GetKey(int key, int state) {
			return glfwGetKey(game_window, key) == state;
		}
		bool GetMouseButton(int button, int state) {
			return glfwGetMouseButton(game_window, button) == state;
		}
		glm::dvec2 GetCursorPos() {
			double w, h;
			glfwGetCursorPos(game_window, &w, &h);
			return { w, h };
		}
		int8_t GetAxisRawH() {
			if (GetKey(GLFW_KEY_A, GLFW_PRESS))
				return -1;
			else if (GetKey(GLFW_KEY_D, GLFW_PRESS))
				return 1;
			return 0;
		}
		int8_t GetAxisRawV() {
			if (GetKey(GLFW_KEY_W, GLFW_PRESS))
				return -1;
			else if (GetKey(GLFW_KEY_S, GLFW_PRESS))
				return 1;
			return 0;
		}
	}
}