#pragma once
#include "particals.h"
class programe : public App {
public:
	void setup() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 8);
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 0);
		//glfwWindowHint(GLFW_ALPHA_BITS, 8);
		windoow_size = { 640, 360 };
		win = glfwCreateWindow(windoow_size.x, windoow_size.y, "0", 0, 0);
		glfwMakeContextCurrent(win);
		gladLoadGL();
		vx::init();
		rend.creat(0, 32000);
		rend.updatecontex(windoow_size);
		inputhandler.win = win;
		l = leg({ 100, 100}, 3, 200.f);
		p = partical_emitter(20, 0.16666 * 6, {0, 0, windoow_size.x, 200}, 3);

		//solver.createchain({ 100, 10 }, 10, 8, 5);
		strs.create(windoow_size);
		/*
		solver.createchain({ 80, 10 }, 10, 6, 8);
		solver.particals[solver.particals.size() - 1].xlr8({ 200.f, 0 });*/

		//solver.createchain({ 200, 10 }, 10, 8, 5);
		//solver.createchain({ 300, 10 }, 10, 8, 5);
		aabbs = {
			{ windoow_size.x - 100, 148, 128, 200 },
			{ windoow_size.x - 164, 164, 164, 200 },
			{ 0, 128, 96, 200 },
			{ 0, 100, 64, 200 }
		};
		for (int i = 0; i < 10; ++i) {
			glm::vec2 pos = { utl::random::GetRandom(0.f, (float)windoow_size.x), utl::random::GetRandom(0.0, 200.0) };
			bool in_block = false;
			for (int i = 0; i < aabbs.size(); ++i) {
				if (pos.x > aabbs[i].x && pos.x < aabbs[i].x + aabbs[i].z && pos.y > aabbs[i].y && pos.y < aabbs[i].y + aabbs[i].w) {
					in_block = true; break;
				}
			}
			if (!in_block) {

				snk.push_back(dragon(pos));
				strs.ff.push_back({ {}, {}, 16 });
			}
			else {
				--i;
			}
		}
	}
	void Run() {
		setup();
		MainLoop();
	}
	void MainLoop() {
		float currentt = glfwGetTime();
		float lastt = currentt;
		vx::shader_prog _default = vx::creatshaderprogramfromfile("assets\\shaders\\vert.vert", "assets\\shaders\\frag.frag");
		vx::shader_prog upsidown = vx::creatshaderprogramfromfile("assets\\shaders\\vert.vert", "assets\\shaders\\upside_down.frag");
		vx::shader_prog lightnesfilter = vx::creatshaderprogramfromfile("assets\\shaders\\vert.vert", "assets\\shaders\\lighteness_threshholder.frag");
		vx::shader_prog bloom = vx::creatshaderprogramfromfile("assets\\shaders\\vert.vert", "assets\\shaders\\bloom.frag");
		vx::shader_prog g_blur = vx::creatshaderprogramfromfile("assets\\shaders\\vert.vert", "assets\\shaders\\gaussian.frag");
		
		
		uint32_t screen_text;
		glGenTextures(1, &screen_text);
		glBindTexture(GL_TEXTURE_2D, screen_text);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windoow_size.x, windoow_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		uint32_t lightness_text;
		glGenTextures(1, &lightness_text);
		glBindTexture(GL_TEXTURE_2D, lightness_text);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windoow_size.x, windoow_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		uint32_t fbo;
		glGenFramebuffers(1, &fbo);
		uint32_t fbo_text;
		glGenTextures(1, &fbo_text);
		glBindTexture(GL_TEXTURE_2D, fbo_text);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windoow_size.x, 200, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		uint32_t blur1[2];
		uint32_t blur_fbos[2];
		glGenFramebuffers(2, blur_fbos);
		glGenTextures(2, blur1);
		for (int i = 0; i < 2; ++i) {
			glBindTexture(GL_TEXTURE_2D, blur1[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windoow_size.x, windoow_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		}
		while (!glfwWindowShouldClose(win))
		{
			currentt = glfwGetTime();
			if ((currentt - lastt) >= (1 / 60.f)) {
				glfwSetWindowTitle(win, std::to_string((currentt - lastt) * 1000).c_str());
				gbl::dt_max = (1 / 60.f);
				gbl::dt = (currentt - lastt);
				gbl::time += gbl::dt;
				auto s = inputhandler.GetJoystickState(GLFW_JOYSTICK_1);
				l.target = inputhandler.GetCursorPos();
				l.update();
				p.update();
				if (inputhandler.GetKey(GLFW_KEY_SPACE, GLFW_PRESS)) {
					for (auto& i : strs.strs) {

				i.particals[i.particals.size() - 1].xlr8({ 2000.f, 0 });
					}
				}
				//solver.update();
				for (int i = 0; i < snk.size(); ++i) {
					snk[i].update(aabbs);
					strs.ff[i].pos = snk[i].body.ch.joints[0];
					strs.ff[i].force = glm::normalize(snk[i].target - snk[i].body.ch.joints[0]) * 500.f;

				}

				strs.update();
				//render the reflection
				rend.current_sh = _default;
				glActiveTexture(GL_TEXTURE0);
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
				glBindTexture(GL_TEXTURE_2D, fbo_text);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_text, 0);
				rend.updatecontex({ windoow_size.x, 200 });
				glViewport(0, 0, windoow_size.x, 200);
				rend.clear({ 18 / 256.0, 18 / 256.0, 18 / 256.0, 1 });
				rend.renderrectangle({ windoow_size.x - 100, 148, 128, 200 }, { 39, 57, 54, 255 });
				rend.renderrectangle({ windoow_size.x - 164, 164, 164, 200 }, { 39, 57, 54, 255 });
				//rend.renderrectangle({ 0, 100, 64, 200 }, { 255, 255, 255, 255 });

				rend.renderrectangle({ 0, 128, 96, 200 }, { 39, 57, 54, 255 });
				rend.renderrectangle({ 0, 100, 64, 200 }, { 39, 57, 54, 255 });
				for (int i = 0; i < 8; ++i)strs.strs[i].render(rend);
				for (auto& i : snk)i.render(rend);
				for (int i = 8; i < 16; ++i)strs.strs[i].render(rend);

				rend.flush();
				//render scene
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
				glBindTexture(GL_TEXTURE_2D, screen_text);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_text, 0);
				rend.updatecontex(windoow_size);
				glViewport(0, 0, windoow_size.x, windoow_size.y);
				rend.clear({ 20 / 256.0, 20 / 256.0, 20 / 256.0, 1 });

				rend.renderrectangle({ windoow_size.x - 100, 148, 128, 200 }, { 39, 57, 54, 255 });
				rend.renderrectangle({ windoow_size.x - 164, 164, 164, 200 }, { 39, 57, 54, 255 });
				//rend.renderrectangle({ 0, 100, 64, 200 }, { 255, 255, 255, 255 });

				rend.renderrectangle({ 0, 128, 96, 200 }, { 39, 57, 54, 255 });
				rend.renderrectangle({ 0, 100, 64, 200 }, { 39, 57, 54, 255 });

				w.render(rend);
				for (int i = 0; i < 8; ++i)strs.strs[i].render(rend);
				for (auto& i : snk)i.render(rend);
				for (int i = 8; i < 16; ++i)strs.strs[i].render(rend);
				p.render(rend);
				//solver.render(rend);
				rend.flush();
				//render water
				rend.current_sh = upsidown;
				glUniform1f(glGetUniformLocation(upsidown.id, "time"), gbl::time);
				rend.renderrectangle({ 0.0, 200.0, windoow_size.x, 200}, { 255, 255, 255, 255 }, { fbo_text } /*, { 0, (windoow_size.y - 200) / windoow_size.y, 1, 1- (windoow_size.y - 200) / windoow_size.y }*/);
				rend.flush();
				//lightness filter
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightness_text, 0);
				rend.updatecontex(windoow_size);
				rend.current_sh = lightnesfilter;

				rend.clear({ 0, 0, 0, 1 });
				rend.renderrectangle({ 0.0, 0, windoow_size.x, windoow_size.y}, { 255, 255, 255, 255 }, { screen_text} /*, { 0, (windoow_size.y - 200) / windoow_size.y, 1, 1- (windoow_size.y - 200) / windoow_size.y }*/);

				rend.flush();
				//blur lightness

				rend.current_sh = g_blur;

				for (int i = 0; i < 2; ++i) {
					glBindFramebuffer(GL_FRAMEBUFFER, blur_fbos[i]);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur1[i], 0);
					rend.clear({ 0., 0., 0., 1 });
					rend.updatecontex(windoow_size);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, 0 == i ? lightness_text : blur1[0]);
					g_blur.bind();
					rend.current_sh = g_blur;

					glUniform1i(glGetUniformLocation(g_blur.id, "sampler"), 0);
					glUniform1i(glGetUniformLocation(g_blur.id, "sampler2"), 1);
					glUniform2f(glGetUniformLocation(g_blur.id, "u_res"), windoow_size.x, windoow_size.y);
					glUniform1f(glGetUniformLocation(g_blur.id, "standered_div"), 8);
					glUniform1i(glGetUniformLocation(g_blur.id, "rad"), 16);
					glUniform1i(glGetUniformLocation(g_blur.id, "virtical_pass"), i);
					rend.renderrectangle({ 0,0,windoow_size.x,windoow_size.y }, { 255, 255, 255, 255 }, {0, 0, 0, 0});

					rend.flush1();
				}
				//bloom
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				bloom.bind();
				rend.updatecontex(windoow_size);
				rend.current_sh = bloom;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, screen_text);
				glUniform1i(glGetUniformLocation(bloom.id, "sampler"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, blur1[1]);
				glUniform1i(glGetUniformLocation(bloom.id, "sampler2"), 1);

				rend.renderrectangle({ 0,0,windoow_size.x, windoow_size.y}, { 255, 255, 255, 255 }, { 0, 0, 0, 4 });


				rend.flush1();
				glfwSwapBuffers(win);
				glfwPollEvents();
				lastt = currentt;
			}
		}
	}
private:
	glm::uvec2 windoow_size;
	std::vector<dragon> snk;
	wing w;
	leg l;
	vx::renderer rend;
	GLFWwindow* win;
	partical_emitter p;
	absloutsolver solver;
	vegetation strs;
	std::vector<glm::vec4> aabbs;
};