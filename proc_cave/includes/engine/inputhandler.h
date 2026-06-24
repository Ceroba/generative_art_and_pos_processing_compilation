#pragma once
#include "vortex.h"
class InputHandler {
public:
	GLFWwindow* win = nullptr;
	struct controller_state
	{
		bool pres = 0;
		bool isgpad = 0;
		GLFWgamepadstate state = {};
		int n_axes = -1;
		int n_buttons = -1;
		const float* axes = nullptr;
		const unsigned char* buttons = nullptr;
	};
	bool GetKey(int key, int state);
	bool GetMouseButton(int button, int state);
	glm::dvec2 GetCursorPos();
	glm::dvec2 GetWindowCursorPos();
	controller_state GetJoystickState(int joystick);

};