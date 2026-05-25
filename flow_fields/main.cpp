//#include"gl2d/gl2d.h"
//float w = 800, h = 800;
//int wn = 100, hn = 100;
//std::vector<glm::vec2> field;
//float dist(glm::vec2 a) {
//	return std::sqrt(a.x * a.x + a.y * a.y);
//}
//class partical {
//public:
//	glm::vec2 acs;
//	glm::vec2 vel;
//	glm::vec2 pos;
//	partical() {};
//	partical(glm::vec2 pos_, glm::vec2 vel_) :pos(pos_), vel(vel_), acs(glm::vec2(0)){};
//	void updatepos(float dt) {
//		vel += acs;
//		if (dist(vel) > 500) {
//			vel = glm::normalize(vel) * 500.f;
//		}
//		if (pos.x > w) {
//			pos.x = 0;
//		}
//		else if (pos.x < 0) {
//			pos.x = w;
//		}	
//		if (pos.y > h) {
//			pos.y = 0;
//		}
//		else if (pos.y < 0) {
//			pos.y = h;
//		}
//		pos += vel * dt;
//		acs.x = 0;
//		acs.y = 0;
//
//	}
//};
//std::vector<partical> particals;
//glm::vec2 field_initaetor(float x, float y) {
//	if (x >= 350) {
//		return glm::vec2(10, 0);
//	}
//	else {
//		//return glm::vec2(10, 0);
//	}
//	if (y >= 350) {
//		return glm::vec2(0, -10);
//	}
//	else {
//		return glm::vec2(0, 10);
//	}
//	return glm::vec2(0.1, 0);
//	/*else return glm::vec2(0);*/
//}
//void init_field(glm::vec2(*fun)(float x, float y)) {
//	for (int i = 0; i < hn; i++)
//		for (int j = 0; j < hn; j++)
//			field.push_back(fun(j * (w / wn), i * (h / hn)));
//}
//void render(gl2d::Shader& sh, gl2d::cnv::canvas & cnv) {
//	cnv._style->color = glm::vec4(0, 0, 0, 0);
//
//	cnv.beginPath();
//	for (int i = 0; i < hn; i++)
//		for (int j = 0; j < hn; j++) {
//			cnv.line(glm::vec2(j * (w / wn), i * (h / hn)), glm::vec2(j * (w / wn), i * (h / hn)) + field[j + i * wn]);
//		}
//	cnv.stroke(sh);
//	cnv._style->color = glm::vec4(0, 0, 0, 27);
//
//	for (auto i : particals) {
//		cnv.beginPath();
//
//		cnv.arc(i.pos, 2, gl2d::Math::HALF_PI, gl2d::Math::HALF_PI + gl2d::Math::_2PI);
//
//		cnv.fill(sh);
//	}
//}
//float dist(glm::vec2 a, glm::vec2 b) {
//	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
//}
//glm::vec2 get_force(glm::vec2 pos, glm::vec2 vect, glm::vec2 vectpos) {
//	return vect / dist(pos, vectpos);
//}
//void step(float dt) {
//	for (int k = 0; k < particals.size(); k++) {
//		glm::vec2 d(0);
//		float min = 9999999999;
//		int x = 0, y = 0;
//		float yidx;
//		float xidx;
//		float distence;
//		for (int i = 0; i < hn; i++)
//		{
//			for (int j = 0; j < wn; j++) {
//				yidx = (i * (h / hn));
//				xidx = (j * (w / wn));
//				distence = std::sqrt((xidx - particals[k].pos.x) * (xidx - particals[k].pos.x) + (yidx - particals[k].pos.y) * (yidx - particals[k].pos.y));
//				if (distence < min) {
//
//					min = distence;
//					x = j, y = i;
//				}
//				/*d += get_force(particals[k], glm::vec2(j * (w / wn), i * (h / hn)), field[j + i * wn]) * dt;*/
//			}
//		}
//		particals[k].acs += field[x + y * wn] /*/ (float)(hn * wn)*/;
//
//		particals[k].updatepos(dt);
//	}
//}
//int main() {
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
//	GLFWwindow* win = glfwCreateWindow(800, 800, "0", 0, 0);
//	glfwMakeContextCurrent(win);
//	gladLoadGL();
//	gl2d::CONTEXT_H = gl2d::CONTEXT_W = 800;
//	gl2d::cnv::canvas cnv(glm::vec2(800));
//	cnv._style->color = glm::vec4(0, 0, 0, 127);
//	float currentt = glfwGetTime();
//	float lastt = currentt;
//	gl2d::Shader sh2("framevs.txt", "framefs.txt");
//	gl2d::Shader sh("vert.txt", "frag.txt");
//	init_field(field_initaetor);
//	gl2d::renderer2d renderer;
//	particals.push_back(partical(glm::vec2(400), glm::vec2(0)));
//	glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
//	glClearColor(1, 1, 1, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	std::ifstream log("log.txt");
//	int lastlog = 0;
//	log >> lastlog;
//	while (!glfwWindowShouldClose(win))
//	{
//		currentt = glfwGetTime();
//		if ((currentt - lastt) >= (1 / 60.f)) {
//			glClearColor(1, 1, 1, 1);
//			glClear(GL_COLOR_BUFFER_BIT);
//			if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
//				double x, y;
//				glfwGetCursorPos(win, &x, &y);
//				//particals.push_back(glm::vec2(x, y));
//				particals.push_back(partical(glm::vec2(x, y), glm::vec2(0)));
//
//			}			
//			if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
//				glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
//				unsigned char* pixels = new unsigned char[800 * 800 * 4];
//				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//				stbi_write_png(("screenshot" + std::to_string(lastlog) + ".png").c_str(), 800, 800, 4, pixels, 800 * 4);
//				lastlog++;
//			}
//			step(1 / 128.f);
//			glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
//			render(sh, cnv);
//			renderer.display(sh2);
//			glfwSwapBuffers(win);
//			glfwPollEvents();
//			lastt = currentt;
//		}
//	}
//	log.close();
//	std::ofstream newl("log.txt");
//	newl << lastlog;
//	newl.close();
//	gl2d::cnv::styles::freeStyles();
//	glfwTerminate();
//}

//#include"gl2d/gl2d.h"
//float w = 800, h = 800;
//int wn = 100, hn = 100;
//std::vector<glm::vec2> field;
//float dist(glm::vec2 a) {
//	return std::sqrt(a.x * a.x + a.y * a.y);
//}
//class partical {
//public:
//	glm::vec2 acs;
//	glm::vec2 vel;
//	glm::vec2 pos;
//	partical() {};
//	partical(glm::vec2 pos_, glm::vec2 vel_) :pos(pos_), vel(vel_), acs(glm::vec2(0)) {};
//	void updatepos(float dt) {
//		vel += acs;
//		if (dist(vel) > 500) {
//			vel = glm::normalize(vel) * 500.f;
//		}
//		if (pos.x > w) {
//			pos.x = 0;
//		}
//		else if (pos.x < 0) {
//			pos.x = w;
//		}
//		if (pos.y > h) {
//			pos.y = 0;
//		}
//		else if (pos.y < 0) {
//			pos.y = h;
//		}
//		pos += vel * dt;
//		acs.x = 0;
//		acs.y = 0;
//
//	}
//};
//std::vector<partical> particals;
//int particalmax = 200;
//glm::vec2 field_initaetor(float x, float y) {
//	if (x >= 350) {
//		return glm::vec2(10, 0);
//	}
//	else {
//		//return glm::vec2(10, 0);
//	}
//	if (y >= 350) {
//		return glm::vec2(0, -10);
//	}
//	else {
//		return glm::vec2(0, 10);
//	}
//	return glm::vec2(0.1, 0);
//	/*else return glm::vec2(0);*/
//}
//void init_field(glm::vec2(*fun)(float x, float y)) {
//	for (int i = 0; i < hn; i++)
//		for (int j = 0; j < hn; j++)
//			field.push_back(fun(j * (w / wn), i * (h / hn)));
//}
//void render(gl2d::Shader& sh, gl2d::cnv::canvas& cnv) {
//	cnv._style->color = glm::vec4(0, 0, 0, 0);
//
//	cnv.beginPath();
//	for (int i = 0; i < hn; i++)
//		for (int j = 0; j < hn; j++) {
//			cnv.line(glm::vec2(j * (w / wn), i * (h / hn)), glm::vec2(j * (w / wn), i * (h / hn)) + field[j + i * wn]);
//		}
//	cnv.stroke(sh);
//	cnv._style->color = glm::vec4(0, 0, 0, 27);
//
//	for (auto i : particals) {
//		cnv.beginPath();
//
//		cnv.arc(i.pos, 2, gl2d::Math::HALF_PI, gl2d::Math::HALF_PI + gl2d::Math::_2PI);
//
//		cnv.fill(sh);
//	}
//}
//float dist(glm::vec2 a, glm::vec2 b) {
//	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
//}
//glm::vec2 get_force(glm::vec2 pos, glm::vec2 vect, glm::vec2 vectpos) {
//	return vect / dist(pos, vectpos);
//}
//void step(float dt) {
//	for (int k = 0; k < particals.size(); k++) {
//		glm::vec2 d(0);
//		float min = 9999999999;
//		int x = 0, y = 0;
//		float yidx;
//		float xidx;
//		float distence;
//		for (int i = 0; i < hn; i++)
//		{
//			for (int j = 0; j < wn; j++) {
//				yidx = (i * (h / hn));
//				xidx = (j * (w / wn));
//				distence = std::sqrt((xidx - particals[k].pos.x) * (xidx - particals[k].pos.x) + (yidx - particals[k].pos.y) * (yidx - particals[k].pos.y));
//				if (distence < min) {
//
//					min = distence;
//					x = j, y = i;
//				}
//				/*d += get_force(particals[k], glm::vec2(j * (w / wn), i * (h / hn)), field[j + i * wn]) * dt;*/
//			}
//		}
//		particals[k].acs += field[x + y * wn] /*/ (float)(hn * wn)*/;
//
//		particals[k].updatepos(dt);
//	}
//}
//int main() {
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
//	GLFWwindow* win = glfwCreateWindow(800, 800, "0", 0, 0);
//	glfwMakeContextCurrent(win);
//	gladLoadGL();
//	gl2d::CONTEXT_H = gl2d::CONTEXT_W = 800;
//	gl2d::cnv::canvas cnv(glm::vec2(800));
//	cnv._style->color = glm::vec4(0, 0, 0, 127);
//	float currentt = glfwGetTime();
//	float lastt = currentt;
//	gl2d::Shader sh2("framevs.txt", "framefs.txt");
//	gl2d::Shader sh("vert.txt", "frag.txt");
//	init_field(field_initaetor);
//	gl2d::renderer2d renderer;
//	particals.push_back(partical(glm::vec2(400), glm::vec2(0)));
//	glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
//	glClearColor(1, 1, 1, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	std::ifstream log("log.txt");
//	int lastlog = 0;
//	log >> lastlog;
//	std::random_device rng;
//	std::uniform_real_distribution<float> ydist(0, 800);
//	std::uniform_real_distribution<float> xdist(0, 800);
//	for (int i = 0; i < particalmax; i++) {
//		particals.push_back(partical(glm::vec2(xdist(rng), ydist(rng)), glm::vec2(0)));
//	}
//	while (!glfwWindowShouldClose(win))
//	{
//		currentt = glfwGetTime();
//		if ((currentt - lastt) >= (1 / 60.f)) {
//			glClearColor(1, 1, 1, 1);
//			glClear(GL_COLOR_BUFFER_BIT);
//			if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
//				double x, y;
//				glfwGetCursorPos(win, &x, &y);
//				//particals.push_back(glm::vec2(x, y));
//				particals.push_back(partical(glm::vec2(x, y), glm::vec2(0)));
//
//			}
//			if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
//				glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
//				unsigned char* pixels = new unsigned char[800 * 800 * 4];
//				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//				stbi_write_png(("screenshot" + std::to_string(lastlog) + ".png").c_str(), 800, 800, 4, pixels, 800 * 4);
//				stbi_image_free(pixels);
//				lastlog++;
//			}
//			step(1 / 128.f);
//			glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
//			render(sh, cnv);
//			renderer.display(sh2);
//			glfwSwapBuffers(win);
//			glfwPollEvents();
//			lastt = currentt;
//		}
//	}
//	log.close();
//	std::ofstream newl("log.txt");
//	newl << lastlog;
//	newl.close();
//	gl2d::cnv::styles::freeStyles();
//	glfwTerminate();
//}

#include"gl2d/gl2d.h"
#include "FastNoiseSIMD/FastNoiseSIMD.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"
	std::random_device rng;
	std::uniform_real_distribution<float> ydist(0, 800);
	std::uniform_real_distribution<float> xdist(0, 800);
	std::uniform_int_distribution<long long> sd(-999999999999999999, 999999999999999999);
float w = 800, h = 800;
int wn = 100, hn = 100;
float* noiseData = new float[wn * hn];
float dist(glm::vec2 a) {
	return std::sqrt(a.x * a.x + a.y * a.y);
}
class partical {
public:
	glm::vec2 acs;
	glm::vec2 vel;
	glm::vec2 pos;
	partical() {};
	partical(glm::vec2 pos_, glm::vec2 vel_) :pos(pos_), vel(vel_), acs(glm::vec2(0)) {};
	void updatepos(float dt) {
		vel += acs;
		if (dist(vel) > 500) {
			vel = glm::normalize(vel) * 500.f;
		}
		if (pos.x > w) {
			pos.x = 0;
		}
		else if (pos.x < 0) {
			pos.x = w;
		}
		if (pos.y > h) {
			pos.y = 0;
		}
		else if (pos.y < 0) {
			pos.y = h;
		}
		pos += vel * dt;
		acs.x = 0;
		acs.y = 0;

	}
};
std::vector<partical> particals;

long long seed = sd(rng);
std::vector<glm::vec2> field;
int particalmax = 200;
float t = 0;
glm::vec2 field_initaetor(float x, float y) {
	//if (x >= 350) {
	//	return glm::vec2(10, 0);
	//}
	//else {
	//	//return glm::vec2(10, 0);
	//}
	//if (y >= 350) {
	//	return glm::vec2(0, -10);
	//}
	//else {
	//	return glm::vec2(0, 10);
	//}
	//return glm::vec2(0.1, 0);
	/*else return glm::vec2(0);*///glm::vec2(std::sin(t * speed + off) * l + o.x, std::cos(t * speed + off) * l + o.y);
	
	return glm::vec2(std::sin(noiseData[(int)(x + y * wn)] * gl2d::Math::_2PI /*+ t*/) * 10, std::cos(noiseData[(int)(x + y * wn)] * gl2d::Math::_2PI /*+ t*/) * 10);

}
void init_field(glm::vec2(*fun)(float x, float y)) {
	for (int i = 0; i < hn; i++)
		for (int j = 0; j < wn; j++)
			field.push_back(fun(j/* * (w / wn)*/, i /** (h / hn)*/));
}
void update_field(glm::vec2(*fun)(float x, float y)) {
	for (int i = 0; i < hn; i++)
		for (int j = 0; j < wn; j++)
			field[j + i * wn] = fun(j, i);
}
float particalColor[4] = { 0 / 255.f, 0 / 255.f, 0 / 255.f, 27 / 255.f };
float r = 2;
void render(gl2d::Shader& sh, gl2d::cnv::canvas& cnv) {
	cnv._style->color = glm::vec4(1, 1, 1, 1);

	cnv.beginPath();
	for (int i = 0; i < hn; i++)
		for (int j = 0; j < hn; j++) {
			cnv.line(glm::vec2(j * (w / wn), i * (h / hn)), glm::vec2(j * (w / wn), i * (h / hn)) + field[j + i * wn]);
		}
	cnv.stroke(sh);

	cnv._style->color = glm::vec4(0, 0, 0, 27);
	cnv._style->color = glm::vec4(particalColor[0] * 255.f, particalColor[1] * 255.f, particalColor[2] * 255.f, particalColor[3] * 255.f);


	for (auto i : particals) {
		cnv.beginPath();

		cnv.arc(i.pos, r, gl2d::Math::HALF_PI, gl2d::Math::HALF_PI + gl2d::Math::_2PI);

		cnv.fill(sh);
	}
}
float dist(glm::vec2 a, glm::vec2 b) {
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
glm::vec2 get_force(glm::vec2 pos, glm::vec2 vect, glm::vec2 vectpos) {
	return vect / dist(pos, vectpos);
}
void generatePerlinNoise(float* noiseData) {
	FastNoiseSIMD* fastNoise = FastNoiseSIMD::NewFastNoiseSIMD();
	fastNoise->SetNoiseType(FastNoiseSIMD::Perlin);
	fastNoise->SetSeed(seed);
	//fastNoise->SetSeed(157375536);
	fastNoise->SetFrequency(0.02);

	fastNoise->FillNoiseSet(noiseData, 0, 0, 0, wn, hn, 1);

	// Delete the instance when done
	delete fastNoise;
}
void step(float dt) {
	for (int k = 0; k < particals.size(); k++) {
		glm::vec2 d(0);
		float min = 9999999999;
		int x = 0, y = 0;
		float yidx;
		float xidx;
		float distence;
		for (int i = 0; i < hn; i++)
		{
			for (int j = 0; j < wn; j++) {
				yidx = (i * (h / hn));
				xidx = (j * (w / wn));
				distence = std::sqrt((xidx - particals[k].pos.x) * (xidx - particals[k].pos.x) + (yidx - particals[k].pos.y) * (yidx - particals[k].pos.y));
				if (distence < min) {

					min = distence;
					x = j, y = i;
				}
				/*d += get_force(particals[k], glm::vec2(j * (w / wn), i * (h / hn)), field[j + i * wn]) * dt;*/
			}
		}
		particals[k].acs += field[x + y * wn] /*/ (float)(hn * wn)*/;
		
		particals[k].updatepos(dt);
	}
}
void reset(GLFWwindow*win) {
	seed = sd(rng);
	delete[] noiseData;
	noiseData = new float[wn * hn];
	field.clear();
	particals.clear();
	glfwDestroyWindow(win);
}
typedef struct Clock
{
	float currentt = glfwGetTime();
	float lastt = currentt;
	void restart() {
		lastt = currentt;
	}
	void update() {
		currentt = glfwGetTime();
	}
	float getmilli() {
		update();
		return (currentt - lastt) * 1000;
	}
	float getsec() {
		update();
		return (currentt - lastt) * 1000;
	}
}Clock;
#include<thread>
GLFWwindow* confwin;
//void otherwinhan() {
//
//	glfwMakeContextCurrent(confwin);
//	float currentt = glfwGetTime();
//	float lastt = currentt;
//	while (!glfwWindowShouldClose(confwin))
//	{
//		currentt = glfwGetTime();
//		if ((currentt - lastt) >= (1 / 60.f)) {
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//		glClearColor(1, 0, 1, 1);
//		glClear(GL_COLOR_BUFFER_BIT);
//		ImGui::Begin("scene objects");
//
//		ImGui::Text("fhjfsdvhc");
//		ImGui::End();
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//		glfwSwapBuffers(confwin);
//		glfwPollEvents();
//		lastt = currentt;
//
//
//		}
//	}
//
//
//}
float background[4] = { 1, 1, 1, 1 };
int main() {
	Clock clickclock;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 0);
	confwin = glfwCreateWindow(500, 600, "1", 0, 0);
	glfwSetWindowPos(confwin, 1400, 30);
	glfwMakeContextCurrent(confwin);
	gladLoadGL();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	// Setup Platform/Renderer bindings
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(confwin, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	int generate = 2;
resetp:
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	GLFWwindow* win = glfwCreateWindow(800, 800, "0", 0, 0);
	glfwSetWindowPos(win, 560, 3);
	glfwMakeContextCurrent(win);
	gladLoadGL();

	// Setup Dear ImGui style
	gl2d::Shader sh2("framevs.txt", "framefs.txt");
	gl2d::Shader sh("vert.txt", "frag.txt");
	std::ifstream log("log.txt");
	int lastlog = 0;
	log >> lastlog;
	gl2d::CONTEXT_H = gl2d::CONTEXT_W = 800;
	gl2d::renderer2d renderer;
	gl2d::cnv::canvas cnv(glm::vec2(800));
	generatePerlinNoise(noiseData);
	init_field(field_initaetor);
	cnv._style->color = glm::vec4(0, 0, 0, 127);
	float currentt = glfwGetTime();
	float lastt = currentt;
	//particals.push_back(partical(glm::vec2(400), glm::vec2(0)));
	glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
	glClearColor(background[0], background[1], background[2], background[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	switch (generate)
	{
	case 0:
		for (int i = 0; i < particalmax; i++) {
			particals.push_back(partical(glm::vec2(/*xdist(rng)*/0, ydist(rng)), glm::vec2(0)));
		}
		break;
	case 1:
		for (int i = 0; i < particalmax; i++) {
			particals.push_back(partical(glm::vec2(/*xdist(rng)*/800, ydist(rng)), glm::vec2(0)));
		}
		break;
	case 2:
		for (int i = 0; i < particalmax; i++) {
			particals.push_back(partical(glm::vec2(xdist(rng), 800), glm::vec2(0)));
		}
		break;
	case 3:
		for (int i = 0; i < particalmax; i++) {
			particals.push_back(partical(glm::vec2(xdist(rng), 0), glm::vec2(0)));
		}
		break;
	case 4:
		for (int i = 0; i < particalmax; i++) {
			particals.push_back(partical(glm::vec2(xdist(rng), ydist(rng)), glm::vec2(0)));
		}
		break;
	default:
		break;
	}
	while (!glfwWindowShouldClose(win))
	{
		currentt = glfwGetTime();
		if ((currentt - lastt) >= (1 / 60.f)) {

			{
				glfwMakeContextCurrent(win);

				glClearColor(1, 1, 1, 1);
				glClear(GL_COLOR_BUFFER_BIT);

				if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					double x, y;
					glfwGetCursorPos(win, &x, &y);
					//particals.push_back(glm::vec2(x, y));
					particals.push_back(partical(glm::vec2(x, y), glm::vec2(0)));

				}
				if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
					glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
					unsigned char* pixels = new unsigned char[800 * 800 * 4];
					glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
					stbi_write_png(("screenshot" + std::to_string(lastlog) + ".png").c_str(), 800, 800, 4, pixels, 800 * 4);
					stbi_image_free(pixels);
					lastlog++;
				}
				if ((glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) && (clickclock.getmilli() > 220)) {
					reset(win);
					goto resetp;
					clickclock.restart();
				}
				t += gl2d::Math::_2PI / 10;
				update_field(field_initaetor);

				step(1 / 128.f);
				glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
				render(sh, cnv);
				renderer.display(sh2);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				renderer.display(sh2);
			}

			{
				glfwMakeContextCurrent(confwin);
				glClearColor(1, 1, 1, 1);
				glClear(GL_COLOR_BUFFER_BIT);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				ImGui::Begin("settings");
				
				ImGui::End();
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			glfwSwapBuffers(win);
			glfwSwapBuffers(confwin);
			glfwPollEvents();
			lastt = currentt;


		}
	}
	log.close();
	std::ofstream newl("log.txt");
	newl << lastlog;
	newl.close();
	gl2d::cnv::styles::freeStyles();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}
