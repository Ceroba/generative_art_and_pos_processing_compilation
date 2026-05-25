#pragma once
#include "engine/app.h"

class programe : public App {
public:
	void setup() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 0);
		//glfwWindowHint(GLFW_ALPHA_BITS, 8);

		win = glfwCreateWindow(1000, 1000, "0", 0, 0);
		glfwMakeContextCurrent(win);
		gladLoadGL();
		vx::init();
		rend.creat(0, 32000);
		rend.updatecontex({ 1000, 1000 });
		inputhandler.win = win;
	}
	void Run() {
		setup();
		MainLoop();
	}
	void MainLoop() {
		float currentt = glfwGetTime();
		float lastt = currentt;
		vx::shader_prog prog = vx::creatshaderprogramfromfile("glframework\\assets\\shaders\\shader.vert", "glframework\\assets\\shaders\\shader.frag");
		rend.current_sh = prog;
		uint32_t iTime = glGetUniformLocation(rend.current_sh.id, "iTime");
		float time = 0;
		while (!glfwWindowShouldClose(win))
		{
			currentt = glfwGetTime();
			if ((currentt - lastt) >= (1 / 60.f)) {
				glfwSetWindowTitle(win, std::to_string((currentt - lastt) * 1000).c_str());

				auto s = inputhandler.GetJoystickState(GLFW_JOYSTICK_1);
				time += (currentt - lastt);
				glUniform1f(iTime, time);
				rend.clear({ 0, 0, 0, 1 });
				rend.renderrectangle({ 0, 0, 1000, 1000}, { 255, 255, 255, 255 });

				rend.flush();

				glfwSwapBuffers(win);
				glfwPollEvents();
				lastt = currentt;
			}
		}
	}
	vx::renderer rend;
	GLFWwindow* win;
};

