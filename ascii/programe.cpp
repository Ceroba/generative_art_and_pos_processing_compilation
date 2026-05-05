#pragma once
#include "engine/app.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"
//using ImGui im;
vx::texture load(const char* filename, int slot) {
	int w, h, n;
	unsigned char* data = stbi_load(filename, &w, &h, &n, 4);
	uint32_t id = -1;
	if (data) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		//GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER      GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	return{ id, w, h, n };
}
class programe : public App {
public:
	int ww, wh;
	void setup() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 0);
		//glfwWindowHint(GLFW_ALPHA_BITS, 8);
		ww = 1920; wh = 1024;
		win = glfwCreateWindow(ww, wh, "0", 0, 0);
		glfwMakeContextCurrent(win);
		gladLoadGL();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Setup Platform/Renderer bindings
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(win, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		vx::init();
		rend.creat(0, 32000);
		rend.updatecontex({ ww, wh });
		inputhandler.win = win;
	}
	void Run() {
		setup();
		MainLoop();
	}
	void MainLoop() {
		float currentt = glfwGetTime();
		float lastt = currentt;
		std::string comp_code;
		std::ifstream comp_file;
		std::stringstream ss;
		comp_file.open("glframework\\assets\\shaders\\edge_scale.comp");
		ss << comp_file.rdbuf();
		comp_code = ss.str();
		uint32_t comp_sh = vx::inter::creatshader(comp_code.c_str(), GL_COMPUTE_SHADER);
		uint32_t comp = glCreateProgram();
		glAttachShader(comp, comp_sh);
		glLinkProgram(comp);
		vx::velidateshader(comp);
		vx::shader_prog blur = vx::creatshaderprogramfromfile("glframework\\assets\\shaders\\shader.vert", "glframework\\assets\\shaders\\gaussian.frag");
		vx::shader_prog default_sh = vx::creatshaderprogramfromfile("glframework\\assets\\shaders\\shader.vert", "glframework\\assets\\shaders\\def.frag");
		vx::shader_prog Dog = vx::creatshaderprogramfromfile("glframework\\assets\\shaders\\shader.vert", "glframework\\assets\\shaders\\DOG.frag");
		vx::shader_prog edges = vx::creatshaderprogramfromfile("glframework\\assets\\shaders\\shader.vert", "glframework\\assets\\shaders\\edges.frag");
		vx::shader_prog ascii = vx::creatshaderprogramfromfile("glframework\\assets\\shaders\\shader.vert", "glframework\\assets\\shaders\\shader.frag");
		rend.current_sh = blur;
		vx::texture tex = load("glframework\\assets\\sprite\\ho.png", 0);
		vx::texture chars = load("glframework\\assets\\sprite\\1x0-8x8-3.png", 1);
		vx::texture charsedges = load("glframework\\assets\\sprite\\edges-8x40.png", 3);
		uint32_t fbos[2];
		glGenFramebuffers(2, fbos);
		uint32_t DOG;
		uint32_t compimg;
		uint32_t blur1[2];
		uint32_t blur2[2];
		glGenTextures(2, blur1);
		glGenTextures(2, blur2);
		for (int i = 0; i < 2; ++i) {
			glBindTexture(GL_TEXTURE_2D, blur1[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, blur2[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		}
		glGenTextures(1, &DOG);
		glBindTexture(GL_TEXTURE_2D, DOG);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glUseProgram(comp);
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &compimg);
		glBindTexture(GL_TEXTURE_2D, compimg);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, tex.w / 8, tex.h / 8);
		glBindImageTexture(0, compimg, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << tex.w << " " << tex.h;
		float segma = 8;
		float k_segma = 0.25;
		int rad = 8;

		float r = 0.1;
		
		float soble_edge_thr = 1;

		float comp_edge_thr = 0.05;
		
		while (!glfwWindowShouldClose(win))
		{
			currentt = glfwGetTime();
			if ((currentt - lastt) >= (1 / 60.f)) {
				glfwSetWindowTitle(win, std::to_string((currentt - lastt) * 1000).c_str());

				auto s = inputhandler.GetJoystickState(GLFW_JOYSTICK_1);

				rend.clear({ 0.5, 0.5, 0.9, 1 });
				
#pragma region blur


					rend.current_sh = blur;

					for (int i = 0; i < 2; ++i) {
						glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur1[i], 0);
						rend.updatecontex({ tex.w, tex.h });

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, 0 == i ? tex.id : blur1[0]);
						blur.bind();
						rend.current_sh = blur;

						glUniform1i(glGetUniformLocation(blur.id, "sampler"), 0);
						glUniform1i(glGetUniformLocation(blur.id, "sampler2"), 1);
						glUniform2f(glGetUniformLocation(blur.id, "u_res"), tex.w, tex.h);
						glUniform1f(glGetUniformLocation(blur.id, "standered_div"), segma);
						glUniform1i(glGetUniformLocation(blur.id, "rad"), rad);
						glUniform1i(glGetUniformLocation(blur.id, "virtical_pass"), i);
						rend.renderrectangle({ 0,0,tex.w,tex.h }, { 255, 255, 255, 255 }, tex);

						rend.flush(3);
					}
#pragma endregion
#pragma region blur with scalar


					rend.current_sh = blur;

					for (int i = 0; i < 2; ++i) {
						glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur2[i], 0);
						rend.updatecontex({ tex.w, tex.h });

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, 0 == i ? tex.id : blur2[0]);
						rend.current_sh = blur;

						blur.bind();
						glUniform1i(glGetUniformLocation(blur.id, "sampler"), 0);
						glUniform1i(glGetUniformLocation(blur.id, "sampler2"), 1);
						glUniform2f(glGetUniformLocation(blur.id, "u_res"), tex.w, tex.h);
						glUniform1f(glGetUniformLocation(blur.id, "standered_div"), segma*k_segma);
						glUniform1i(glGetUniformLocation(blur.id, "rad"), rad);
						glUniform1i(glGetUniformLocation(blur.id, "virtical_pass"), i);
						rend.renderrectangle({ 0,0,tex.w,tex.h }, { 255, 255, 255, 255 }, tex);

						rend.flush(3);
					}
#pragma endregion
#pragma region THE DOG


					glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DOG, 0);

					rend.current_sh = Dog;
					rend.updatecontex({ tex.w, tex.h });
					Dog.bind();
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, blur1[1]);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, blur2[1]);
					glUniform1i(glGetUniformLocation(Dog.id, "sampler"), 0);
					glUniform1f(glGetUniformLocation(Dog.id, "r"), r);
					glUniform1i(glGetUniformLocation(Dog.id, "sampler1"), 1);

					rend.renderrectangle({ 0,0,tex.w,tex.h }, { 255, 255, 255, 255 }, {0, tex.w, tex.h, tex.n});

					rend.flush(3);
#pragma endregion
#pragma region debug render
					glBindFramebuffer(GL_FRAMEBUFFER, 0);

					rend.current_sh = default_sh;
					rend.updatecontex({ ww, wh });
					default_sh.bind();
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, DOG);
					glUniform1i(glGetUniformLocation(default_sh.id, "sampler"), 0);

					rend.renderrectangle({ 0,tex.h,tex.w,tex.h }, { 255, 255, 255, 255 }, { 0, tex.w, tex.h, tex.n });

					rend.flush(3);
#pragma endregion

#pragma region soble


					glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur1[0], 0);
					rend.current_sh = edges;
					edges.bind();
					rend.updatecontex({ tex.w, tex.h });
					glUniform2f(glGetUniformLocation(edges.id, "u_res"), tex.w, tex.h);
					glUniform1f(glGetUniformLocation(edges.id, "edge_thr"), soble_edge_thr);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, DOG);
					glUniform1i(glGetUniformLocation(edges.id, "sampler"), 0);

					rend.renderrectangle({ 0,0,tex.w,tex.h }, { 255, 255, 255, 255 }, { 0, tex.w, tex.h, tex.n });

					rend.flush(3);
#pragma endregion
#pragma region super sample

					glUseProgram(comp);
					glBindTexture(GL_TEXTURE_2D, blur1[0]);
					glUniform1i(glGetUniformLocation(comp, "sampler"), 0);
					glUniform2f(glGetUniformLocation(comp, "u_res"), tex.w, tex.h);
					glUniform1f(glGetUniformLocation(comp, "edge_thr"), comp_edge_thr);
					glDispatchCompute(tex.w / 8, tex.h / 8, 1);
					glMemoryBarrier(GL_ALL_BARRIER_BITS);
#pragma endregion
#pragma region ascii pass


					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					rend.updatecontex({ ww, wh });
					ascii.bind();
					rend.current_sh = ascii;

					glUniform2f(glGetUniformLocation(ascii.id, "u_res"), tex.w, tex.h);
					glUniform1f(glGetUniformLocation(ascii.id, "char_size"), 8.f);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex.id);
					glUniform1i(glGetUniformLocation(ascii.id, "sampler"), 0);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, chars.id);
					glUniform1i(glGetUniformLocation(ascii.id, "sampler2"), 1);
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, compimg);
					glUniform1i(glGetUniformLocation(ascii.id, "sampler3"), 2);
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_2D, charsedges.id);
					glUniform1i(glGetUniformLocation(ascii.id, "sampler4"), 3);

					rend.renderrectangle({ 0,0,tex.w, tex.h }, { 255, 255, 255, 255 }, { 0, tex.w, tex.h, tex.n });


					rend.flush(3);
#pragma endregion
					ImGui_ImplOpenGL3_NewFrame();

		/*		float segma = 8;
		float k_segma = 0.25;
		int rad = 8;

		float r = 0.1;
		
		float soble_edge_thr = 1;

		float comp_edge_thr = 0.05;*/
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					ImGui::Begin("settings");
					ImGui::SliderFloat("segma", &segma, 0, 24);
					ImGui::SliderFloat("segma scalar", &k_segma, 0, 16);
					ImGui::SliderInt("kernal rad", &rad, 0, 16);
					
					ImGui::SliderFloat("tao", &r, 0, 1);
					
					ImGui::SliderFloat("edge threshhold", &soble_edge_thr, 0, 1);

					ImGui::SliderFloat("ss edge threshhold", &comp_edge_thr, 0, 1);

					ImGui::End();
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(win);
				glfwPollEvents();
				lastt = currentt;
			}
		}
	}
	vx::renderer rend;
	GLFWwindow* win;
};

