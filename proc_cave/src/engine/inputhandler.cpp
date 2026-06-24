#include "inputhandler.h"

bool InputHandler::GetKey(int key, int state) {
	return glfwGetKey(win, key) == state;
}
bool InputHandler::GetMouseButton(int button, int state) {
	return glfwGetMouseButton(win, button) == state;
}
glm::dvec2 InputHandler::GetCursorPos() {
	glm::dvec2 pos;
	glfwGetCursorPos(win, &pos.x, &pos.y);
	return pos;
}

glm::dvec2 InputHandler::GetWindowCursorPos()
{
	glm::ivec2 pos;
	glfwGetWindowPos(win, &pos.x, &pos.y);
	return GetCursorPos() - glm::dvec2(pos);
}

InputHandler::controller_state InputHandler::GetJoystickState(int joystick)
{
	InputHandler::controller_state s = {};
	s.pres = glfwJoystickPresent(joystick);
	if (s.pres) {
		s.isgpad = glfwJoystickIsGamepad(joystick);
		if (s.isgpad) {
			GLFWgamepadstate state;
			if (glfwGetGamepadState(joystick, &state)) {
				s.state = state;
				s.n_axes = 6;
				s.n_buttons = 15;
			}
		}
		else {
			s.axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &s.n_axes);
			s.buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &s.n_buttons);
		}
	}
	return s;
}
