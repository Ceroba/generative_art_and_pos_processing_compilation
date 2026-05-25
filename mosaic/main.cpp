//#include"gl2d/gl2d.h"
//std::vector<glm::ivec2> points;
//std::vector<glm::vec3> colors;
///*  const uint k = 1103515245U;  // GLIB C
////const uint k = 134775813U;   // Delphi and Turbo Pascal
////const uint k = 20170906U;    // Today's date (use three days ago's dateif you want a prime)
////const uint k = 1664525U;     // Numerical Recipes
//
//vec3 hash( uvec3 x )
//{
//    x = ((x>>8U)^x.yzx)*k;
//    x = ((x>>8U)^x.yzx)*k;
//    x = ((x>>8U)^x.yzx)*k;
//    
//    return vec3(x)*(1.0/float(0xffffffffU));
//}
//*/
//int n_points = 1000;
//int x = 800, y = 800, nh = 4;
//unsigned char* out;
//std::random_device rng;
//void distributepoints(unsigned char* in) {
//	std::uniform_int_distribution<int> ydist(0, y);
//	std::uniform_int_distribution<int> xdist(0, x);
//	for (int i = 0; i < n_points; i++) {
//		points.push_back(glm::vec2(xdist(rng), ydist(rng)));
//		colors.push_back(glm::vec3(in[points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x + 0], in[points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x + 1], in[points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x + 2]));
//	}
//}
////void distributepoints() {
////	std::uniform_int_distribution<int> ydist(0, y);
////	std::uniform_int_distribution<int> xdist(0, x);
////	for (int i = 0; i < n_points; i++) {
////		points.push_back(glm::vec2(xdist(rng), ydist(rng)));
////		colors.push_back(glm::vec3(i / (float)(n_points)));
////	}
////}
//void generateoutput() {
//	for (int i = 0; i < y; i++) {
//		for (int j = 0; j < x; j++) {
//			int idx = (i) * x * 4 + (j) * 4;
//			float min = 99999999999.99999999999;
//			int ind = 0;
//			for (int k = 0; k < n_points; k++){
//				if (gl2d::Math::dist(glm::vec2(j, i), glm::vec2(points[k])) < min) {
//					min = gl2d::Math::dist(glm::vec2(j, i), glm::vec2(points[k]));
//					ind = k;
//				}
//			}
//			out[idx] = colors[ind].x;
//			out[idx + 1] = colors[ind].y;
//			out[idx + 2] = colors[ind].z;
//			out[idx + 3] = 255;
//		}
//	}
//}
//
//typedef struct Clock
//{
//	float currentt = glfwGetTime();
//	float lastt = currentt;
//	void restart() {
//		lastt = currentt;
//	}
//	void update() {
//		currentt = glfwGetTime();
//	}
//	float getmilli() {
//		update();
//		return (currentt - lastt) * 1000;
//	}
//	float getsec() {
//		update();
//		return (currentt - lastt) * 1000;
//	}
//}Clock;
//int main() {
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
//	GLFWwindow* win = glfwCreateWindow(800, 800, "0", 0, 0);
//	glfwMakeContextCurrent(win);
//	gladLoadGL();
//	stbi_set_flip_vertically_on_load(1);
//	unsigned char* in = stbi_load("input.png", &x, &y, &nh, 4);
//	distributepoints(in);
//	std::ifstream log("log.txt");
//	int lastlog = 0;
//	log >> lastlog;
//	int size = x * y * nh;
//	out = new uint8_t[size];
//	Clock c; c.restart();
//	generateoutput();
//	float time = c.getmilli();
//	std::cout << time;
//	stbi_write_png(("mosiac" + std::to_string(lastlog) + ".png").c_str(), x, y, nh, out, x * nh);
//	lastlog++;
//	float vert[] = {
//		1.f, 1.f, 0.f, 1.f, 1.f,
//		1.f, -1.f, 0.f, 1.f, 0.f,
//		-1.f, 1.f, 0.f, 0.f, 1.f,
//		-1.f, -1.f, 0.f, 0.f, 0.f
//	};
//	unsigned int ind[] = {
//		0, 1, 2,
//		2, 1, 3
//	};
//	std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER };
//	std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST };
//
//	unsigned int texture;
//	if (out) {
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//
//		for (int i = 0; i < params.size(); i++) {
//			glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
//		}
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, out);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	unsigned int vao, vbo, ebo;
//	gl2d::Shader sh("vertsp.txt", "fragsp.txt");
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	float currentt = glfwGetTime();
//	float lastt = currentt;
//	while (!glfwWindowShouldClose(win))
//	{
//		currentt = glfwGetTime();
//		if ((currentt - lastt) >= (1 / 60.f)) {
//			glfwMakeContextCurrent(win);
//			glClearColor(0, 0, 0, 0);
//			glClear(GL_COLOR_BUFFER_BIT);
//			sh.use();
//			glBindVertexArray(vao);
//			glBindTexture(GL_TEXTURE_2D, texture);
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//			glfwSwapBuffers(win);
//			glfwPollEvents();
//			lastt = currentt;
//		}
//	}
//	log.close();
//	std::ofstream newl("log.txt");
//	newl << lastlog;
//	newl.close();
//	stbi_image_free(in);
//	stbi_image_free(out);
//	glfwTerminate();
//}
//
//



#include"gl2d/gl2d.h"
#include<thread>
std::vector<glm::ivec2> points;
std::vector<glm::vec3> colors;
/*  const uint k = 1103515245U;  // GLIB C
//const uint k = 134775813U;   // Delphi and Turbo Pascal
//const uint k = 20170906U;    // Today's date (use three days ago's dateif you want a prime)
//const uint k = 1664525U;     // Numerical Recipes

vec3 hash( uvec3 x )
{
	x = ((x>>8U)^x.yzx)*k;
	x = ((x>>8U)^x.yzx)*k;
	x = ((x>>8U)^x.yzx)*k;

	return vec3(x)*(1.0/float(0xffffffffU));
}
*/
int n_points = 1600;
int x, y, nh;
int thread_n = std::thread::hardware_concurrency();;
unsigned char* out;
std::random_device rng;
void distributepoints(unsigned char* in) {
	std::uniform_int_distribution<int> ydist(0, y);
	std::uniform_int_distribution<int> xdist(0, x);
	for (int i = 0; i < n_points; i++) {
		points.push_back(glm::vec2(xdist(rng), ydist(rng)));
		int idx = points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x;
		colors.push_back(glm::vec3(in[glm::clamp(idx, 0, x * y * nh)], in[glm::clamp(idx + 1, 0, x * y * nh)], in[glm::clamp(idx + 2, 0, x * y * nh)]));
	}
}
//void distributepoints() {
//	std::uniform_int_distribution<int> ydist(0, y);
//	std::uniform_int_distribution<int> xdist(0, x);
//	for (int i = 0; i < n_points; i++) {
//		points.push_back(glm::vec2(xdist(rng), ydist(rng)));
//		colors.push_back(glm::vec3(i / (float)(n_points)));
//	}
//}
void generateout() {
	static int currentslice = -1;
	currentslice++;
	float slicew = x / (float)thread_n;
	float slicestart = slicew * currentslice;
	float sliceend = slicestart + slicew;
	float dist;
	float min;
	int idx;
	for (int i = 0; i < y; i++) {
		for (int j = std::ceil(slicestart); j < std::ceil(sliceend); j++) {
			idx = (i)*x * nh + (j) * nh;
			min = 99999999999.99999999999;
			int ind = 0;
			for (int k = 0; k < n_points; k++) {
				dist = gl2d::Math::dist(glm::vec2(j, i), glm::vec2(points[k]));
				if (dist < min) {
					min = dist;
					ind = k;
				}
			}
			out[idx] = colors[ind].x;
			out[idx + 1] = colors[ind].y;
			out[idx + 2] = colors[ind].z;
			out[idx + 3] = 255;
		}
	}
}
void generateoutput() {
	std::vector<std::thread> thr;
	for (int i = 0; i < thread_n; i++) {
		thr.push_back(std::thread(generateout));
	}
	for (int i = 0; i < thread_n; i++) {
		thr[i].join();
	}
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
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	GLFWwindow* win = glfwCreateWindow(800, 800, "0", 0, 0);
	glfwMakeContextCurrent(win);
	gladLoadGL();
	stbi_set_flip_vertically_on_load(1);
	unsigned char* in = stbi_load("input.png", &x, &y, &nh, 4);
	distributepoints(in);
	std::ifstream log("log.txt");
	int lastlog = 0;
	log >> lastlog;
	int size = x * y * nh;
	out = new uint8_t[size];
	Clock c; c.restart();
	generateoutput();
	float time = c.getmilli();
	std::cout << time;
	stbi_write_png(("mosiac" + std::to_string(lastlog) + ".png").c_str(), x, y, nh, out, x * nh);
	lastlog++;
	log.close();
	std::ofstream newl("log.txt");
	newl << lastlog;
	newl.close();
	float vert[] = {
		1.f, 1.f, 0.f, 1.f, 1.f,
		1.f, -1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 0.f, 0.f, 1.f,
		-1.f, -1.f, 0.f, 0.f, 0.f
	};
	unsigned int ind[] = {
		0, 1, 2,
		2, 1, 3
	};
	std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER };
	std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST };

	unsigned int texture;
	if (out) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		for (int i = 0; i < params.size(); i++) {
			glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, out);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	unsigned int vao, vbo, ebo;
	gl2d::Shader sh("vertsp.txt", "fragsp.txt");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float currentt = glfwGetTime();
	float lastt = currentt;
	while (!glfwWindowShouldClose(win))
	{
		currentt = glfwGetTime();
		if ((currentt - lastt) >= (1 / 60.f)) {
			glfwMakeContextCurrent(win);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			sh.use();
			glBindVertexArray(vao);
			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glfwSwapBuffers(win);
			glfwPollEvents();
			lastt = currentt;
		}
	}
	stbi_image_free(in);
	stbi_image_free(out);
	glfwTerminate();
}







//#include"gl2d/gl2d.h"
//std::vector<glm::ivec2> points;
//std::vector<glm::vec3> colors;
///*  const uint k = 1103515245U;  // GLIB C
////const uint k = 134775813U;   // Delphi and Turbo Pascal
////const uint k = 20170906U;    // Today's date (use three days ago's dateif you want a prime)
////const uint k = 1664525U;     // Numerical Recipes
//
//vec3 hash( uvec3 x )
//{
//	x = ((x>>8U)^x.yzx)*k;
//	x = ((x>>8U)^x.yzx)*k;
//	x = ((x>>8U)^x.yzx)*k;
//
//	return vec3(x)*(1.0/float(0xffffffffU));
//}
//*/
//int n_points = 1000;
//int x = 800, y = 800, nh = 4;
//std::random_device rng;
//void distributepoints(unsigned char* in) {
//	std::uniform_int_distribution<int> ydist(0, y);
//	std::uniform_int_distribution<int> xdist(0, x);
//	for (int i = 0; i < n_points; i++) {
//		points.push_back(glm::vec2(xdist(rng), ydist(rng)));
//		colors.push_back(glm::vec3(in[points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x + 0], in[points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x + 1], in[points[points.size() - 1].x * nh + points[points.size() - 1].y * nh * x + 2]));
//	}
//}
//
//void generateoutput(unsigned char* out) {
//	for (int i = 0; i < y; i++) {
//		for (int j = 0; j < x; j++) {
//			int idx = (i)*x * 4 + (j) * 4;
//			float min = 99999999999.99999999999;
//			int ind = 0;
//			for (int k = 0; k < n_points; k++) {
//				if (gl2d::Math::dist(glm::vec2(j, i), glm::vec2(points[k])) < min) {
//					min = gl2d::Math::dist(glm::vec2(j, i), glm::vec2(points[k]));
//					ind = k;
//				}
//			}
//			out[idx] = colors[ind].x;
//			out[idx + 1] = colors[ind].y;
//			out[idx + 2] = colors[ind].z;
//			out[idx + 3] = 255;
//		}
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
//	stbi_set_flip_vertically_on_load(1);
//	unsigned char* in = stbi_load("input.png", &x, &y, &nh, 4);
//	//distributepoints(in);
//	//std::ifstream log("log.txt");
//	//int lastlog = 0;
//	//log >> lastlog;
//	//int size = x * y * nh;
//	//unsigned char* out = new uint8_t[size];
//	//generateoutput(out);
//	//stbi_write_png(("mosiac" + std::to_string(lastlog) + ".png").c_str(), x, y, nh, out, x * nh);
//	//lastlog++;
//	float vert[] = {
//		1.f, 1.f, 0.f, 1.f, 1.f,
//		1.f, -1.f, 0.f, 1.f, 0.f,
//		-1.f, 1.f, 0.f, 0.f, 1.f,
//		-1.f, -1.f, 0.f, 0.f, 0.f
//	};
//	unsigned int ind[] = {
//		0, 1, 2,
//		2, 1, 3
//	};
//	std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER };
//	std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST };
//
//	unsigned int texture;
//	if (in) {
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//
//		for (int i = 0; i < params.size(); i++) {
//			glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
//		}
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, in);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	unsigned int vao, vbo, ebo;
//	gl2d::Shader sh("vert3.txt", "frag3.txt");
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	float currentt = glfwGetTime();
//	float lastt = currentt;
//	while (!glfwWindowShouldClose(win))
//	{
//		currentt = glfwGetTime();
//		if ((currentt - lastt) >= (1 / 60.f)) {
//			glfwMakeContextCurrent(win);
//			glClearColor(0, 0, 0, 0);
//			glClear(GL_COLOR_BUFFER_BIT);
//			sh.use();
//			glUniform1f(glGetUniformLocation(sh.ID, "t"), glfwGetTime());
//			glBindVertexArray(vao);
//			glBindTexture(GL_TEXTURE_2D, texture);
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//			glfwSwapBuffers(win);
//			glfwPollEvents();
//			lastt = currentt;
//		}
//	}
//	//log.close();
//	//std::ofstream newl("log.txt");
//	//newl << lastlog;
//	//newl.close();
//	//stbi_image_free(out);
//	stbi_image_free(in);
//	glfwTerminate();
//}
