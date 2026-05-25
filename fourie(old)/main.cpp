//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
//#include<vector>
//#include<iostream>
//
//#include <glad\glad.h>
//
//#include <GLFW\glfw3.h>
//
//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
//#include "glm\glm.hpp"
//#include "glm\gtc\type_ptr.hpp"
//#include "glm\gtc\matrix_transform.hpp"
//#include <iostream>
//#include <cmath>
//#include "stb_image.h"
//#include "stb_image_write.h"
#include"gl2d/gl2d.h"
//// Function to find frequencies present in a sine wave
//void findSineFrequencies(double* signal, int signalSize, double samplingRate) {
//	// Create plans for FFT
//	fftw_complex* fftInput = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * signalSize));
//	fftw_complex* fftOutput = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * signalSize));
//	fftw_plan plan = fftw_plan_dft_1d(signalSize, fftInput, fftOutput, FFTW_FORWARD, FFTW_ESTIMATE);
//
//	// Initialize FFT input
//	for (int i = 0; i < signalSize; ++i) {
//		fftInput[i][0] = signal[i]; // Real part
//		fftInput[i][1] = 0;         // Imaginary part
//	}
//
//	// Perform FFT
//	fftw_execute(plan);
//
//	// Calculate frequencies and amplitudes
//	for (int i = 0; i < signalSize / 2; ++i) {
//		double frequency = i * (samplingRate / signalSize);
//		double amplitude = sqrt(fftOutput[i][0] * fftOutput[i][0] + fftOutput[i][1] * fftOutput[i][1]);
//
//		// Output frequencies and amplitudes
//		std::cout << "Frequency: " << frequency << " Hz, Amplitude: " << amplitude << std::endl;
//	}
//
//	// Clean up
//	fftw_destroy_plan(plan);
//	fftw_free(fftInput);
//	fftw_free(fftOutput);
//}
//double G(double x) {
//	return std::sin(x * 4) + 1;
//}
//double inner(double t, double f) {
//	double x = t * f;
//	return std::abs(std::sqrt(sin(x) * sin(x) + cos(x) * cos(x)));
//}
//glm::vec2 iner(double t, double f) {
//	double x = t * f;
//	return glm::vec2(sin(x), cos(x));
//}
//double integral(double a, double b, int n, double(*func)(double)) {
//	double h = (b - a) / n; // Width of each subinterval
//	double result = 0.5 * (func(a) + func(b)); // Area of the first and last intervals
//
//	for (int i = 1; i < n; ++i) {
//		double x = a + i * h;
//		result += func(x);
//	}
//
//	result *= h; // Multiply by the width of the subintervals
//	return result;
//}
//double fourie(double a, double b, double f, double(*func)(double)) {
//	int n = 1000;
//	double res = 0;
//	double t = 0;
//	for (int i = 1; i <= n; i++) {
//		t += (double)1.f / n;
//		res += G(t) * inner(t, f);
//	}
//	res *= (double)1.f / n;
//	return res;
//}
class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	~Shader() {
		glDeleteProgram(ID);
	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	//-------------------------------------------------------------------------
private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};



const double PI = 3.14159265358979323846;
const double _2PI = 3.14159265358979323846 * 2;
const double HALF_PI = 3.14159265358979323846  / 2;
#include <regex>
std::vector<double> extractFloats(const std::string& input) {
	std::vector<double> floats;

	try {
		// Regular expression pattern to match floats
		std::regex pattern(R"([-+]?\d+(\.\d+)?([eE][-+]?\d+)?)");

		// Iterator for matching results
		std::sregex_iterator iterator(input.begin(), input.end(), pattern);
		std::sregex_iterator end;

		// Iterate through matching results and extract floats
		while (iterator != end) {
			floats.push_back(std::stod(iterator->str()));
			++iterator;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Error converting string to double: " << e.what() << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Error converting string to double (out of range): " << e.what() << std::endl;
	}

	return floats;
}

struct out
{
	glm::vec2 comp;
	float freq, ampl, phase;
};
std::vector<out> DFT(const std::vector<double>& x) {
	std::vector<out> X;
	int N = x.size();
	for (int k = 0; k < N; k++) {
		float im = 0, re = 0;
		for (int n = 0; n < N; n++) {
			float phi = (float)(_2PI * k * n) / N;
			re += x[n] * cos(phi);
			im -= x[n] * sin(phi);
		}
		re = re / N;
		im = im / N;
		float freq = k;
		float ampl = (sqrt(re * re + im * im));
		float phase = atan2(im, re);

		X.push_back({ glm::vec2(re, im), freq, ampl, phase });
	}
	return X;
}
std::vector<double> Y;
std::vector<out> forieY;
std::vector<double> X;
std::vector<out> forieX;
unsigned int vao, vbo, ebo;
int x, y, z;
unsigned char* data = stbi_load("dragon.png", &x, &y, &z, 4);
std::vector<std::string> lines;

void setup2() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	std::string s;
	std::ifstream input("drawing.txt");
	int skip = 10;
	while (std::getline(input, s))
	{
		lines.push_back(s);
	}
	for (int i = 0; i < lines.size() / skip; i++) {
		Y.push_back(800 - extractFloats( lines[i * skip])[1]);
		X.push_back(extractFloats(lines[i * skip])[0]);
	}
	//for (int i = 0; i < 1000; i++) {
	//	Y.push_back((i));
	//}
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	forieX = DFT(X);
	forieY = DFT(Y);
	//for (auto i : forieY) {
	//	std::cout << "{\nre: " << i.comp.x << "\nim: " << i.comp.y << "\nfreq: " << i.freq << "\namp: " << i.ampl << "\nphase: " << i.phase << "\n}\n";
	//}
}

void setup() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	for (int i = 0; i < 800; i++) {
		for (int j = 0; j < 800; j++) {
			if (data[(i) * 800 * 4 + (j) * 4 + 3]) {
				Y.push_back(i);
				X.push_back(j);
			}
		}
	}
	//for (int i = 0; i < 1000; i++) {
	//	Y.push_back((i));
	//}
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	//Y.push_back(-100);
	forieX = DFT(X);
	forieY = DFT(Y);
	//for (auto i : forieY) {
	//	std::cout << "{\nre: " << i.comp.x << "\nim: " << i.comp.y << "\nfreq: " << i.freq << "\namp: " << i.ampl << "\nphase: " << i.phase << "\n}\n";
	//}
}
float t = 0;
std::vector<float>verts;
void draw2(gl2d::Shader& sh, GLFWwindow* win, gl2d::cnv::canvas& cnv) {
	float x = 0, y = 0;
	float prevx = 0, prevy = 0;
	for (int i = 0; i < forieY.size(); i++) {
		x += forieX[i].ampl * cos(forieX[i].freq * t + forieX[i].phase /*+ HALF_PI*/);
		y += forieY[i].ampl * sin(forieY[i].freq * t + forieY[i].phase + HALF_PI);
		//x +=   cos(t + HALF_PI);
		//y +=  sin(t + HALF_PI);
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//sh.use();
		//glBindVertexArray(vao);
		//glDrawArrays(GL_LINES, 0, 2);
		prevx = x / 1000; prevy = y / 1000;
	}
	//verts.push_back(t / 10);
	//verts.push_back(y / 1500);
	//verts.push_back(0);
	verts.insert(verts.begin() + 0, 1, x /*/ 800*/);
	verts.insert(verts.begin() + 1, 1, y /*/ 800*/);
	verts.insert(verts.begin() + 2, 1, 0);
	cnv.beginPath();
	cnv.goTo(glm::vec2(verts[0], verts[1]));
	for (int i = 0; i < (verts.size() / 3); i++) {
		cnv.lineTo(glm::vec2(verts[i * 3], verts[i * 3 + 1]));
		//		sh.use();
		//glBindVertexArray(vao);
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//
		//
		//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
		//
		//glDrawArrays(GL_POINTS, 0, verts.size() / 3);
	}
	cnv.stroke(sh);
	
	//sh.use();
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glDrawArrays(GL_LINES, 0, verts.size() / 3);

}
void draw(gl2d::Shader& sh, GLFWwindow *win, gl2d::cnv::canvas &cnv) {
	float x = 0, y = 0;
	float prevx = 0, prevy = 0;
	for (int i = 0; i < forieY.size(); i++) {
		x += forieY[i].ampl * cos(forieY[i].freq * t + forieY[i].phase + HALF_PI);
		y += forieY[i].ampl * sin(forieY[i].freq * t + forieY[i].phase + HALF_PI);
		//x +=   cos(t + HALF_PI);
		//y +=  sin(t + HALF_PI);
		float vert[] = {
			prevx - 0.40, prevy/*- 0.40*/, 0,
			x / 1000 - 0.40, y / 1000 /*- 0.40*/, 0
		};

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		sh.use();
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, 2);
		prevx = x / 1000; prevy = y / 1000;
	}
	//verts.push_back(t / 10);
	//verts.push_back(y / 1500);
	//verts.push_back(0);
	verts.insert(verts.begin() + 0, 1, t / 10);
	verts.insert(verts.begin() + 1, 1, y / 1500);
	verts.insert(verts.begin() + 2, 1, 0);
	cnv.beginPath();
	cnv.goTo(glm::vec2(verts[0], verts[1]));
	for (int i = 0; i < (verts.size() / 3); i++) {
		cnv.lineTo(glm::vec2(verts[i * 3], verts[i * 3 + 1]));
//		sh.use();
//glBindVertexArray(vao);
//glBindBuffer(GL_ARRAY_BUFFER, vbo);
//
//
//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
//
//glDrawArrays(GL_POINTS, 0, verts.size() / 3);
	}
	cnv.stroke(sh);
	//sh.use();
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glDrawArrays(GL_LINES, 0, verts.size() / 3);

}
void draw3(gl2d::Shader& sh, GLFWwindow* win, gl2d::cnv::canvas& cnv) {
	float x = 0, y = 0;
	float prevx = 0, prevy = 0;
	for (int i = 0; i < forieY.size(); i++) {
		x += forieX[i].ampl * cos(forieX[i].freq * t + forieX[i].phase /*+ HALF_PI*/);
		y += forieY[i].ampl * sin(forieY[i].freq * t + forieY[i].phase + HALF_PI);
		//x +=   cos(t + HALF_PI);
		//y +=  sin(t + HALF_PI);
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//sh.use();
		//glBindVertexArray(vao);
		//glDrawArrays(GL_LINES, 0, 2);
		prevx = x / 1000; prevy = y / 1000;
	}
	//verts.push_back(t / 10);
	//verts.push_back(y / 1500);
	//verts.push_back(0);
	verts.insert(verts.begin() + 0, 1, x /* / 800*/);
	verts.insert(verts.begin() + 1, 1, y /* / 800*/);
	verts.insert(verts.begin() + 2, 1, 0);
	//cnv.beginPath();
	//cnv.goTo(glm::vec2(verts[0], verts[1]));
	for (int i = 0; i < (verts.size() / 3); i++) {
		//cnv.lineTo(glm::vec2(verts[i * 3], verts[i * 3 + 1]));
		sh.use();
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);


		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

		glDrawArrays(GL_POINTS, 0, verts.size() / 3);
	}
	//cnv.stroke(sh);

	//sh.use();
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glDrawArrays(GL_LINES, 0, verts.size() / 3);

}
int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	GLFWwindow* win = glfwCreateWindow(800, 800, "0", 0, 0);
	glfwMakeContextCurrent(win);
	gladLoadGL();
	int size = 800 * 800 * 4;
	unsigned char* out = new uint8_t[size];
	for (int i = 0; i < 800; i++) {
		for (int j = 0; j < 800; j++) {
			out[(i) * 800 * 4 + (j) * 4] = 0;
			out[(i) * 800 * 4 + (j) * 4 + 1] = 0;
			out[(i) * 800 * 4 + (j) * 4 + 2] = 0;
			out[(i) * 800 * 4 + (j) * 4 + 3] = 0;
		}
	}


	float vert[] = {
0, 0, 0,
1, 1, 0
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, out);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
//	unsigned int vao, vbo, ebo;
	gl2d::shaders::load();
	gl2d::Shader sh("vert.txt", "frag.txt");

	gl2d::cnv::canvas cnv(glm::vec2(800)/*, gl2d::cnv::styles::glconxet*/);
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	setup2();
	float currentt = glfwGetTime();
	float lastt = currentt;
			glEnable(GL_PROGRAM_POINT_SIZE);
	while (!glfwWindowShouldClose(win))
	{
		currentt = glfwGetTime();
		if ((currentt - lastt) >= (1 / 60.f)) {
			glfwMakeContextCurrent(win);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			//sh.use();
			//glBindVertexArray(vao);
			//glBindTexture(GL_TEXTURE_2D, texture);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//glBindBuffer(GL_ARRAY_BUFFER, vbo);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//sh.use();
			//glBindVertexArray(vao);
			//glDrawArrays(GL_LINES, 0, 2);
			draw2(sh, win, cnv);
			t = (t > _2PI) ? 0 : (t + _2PI / forieY.size());

			glfwSwapBuffers(win);
			glfwPollEvents();
			lastt = currentt;
		}
	}
	glfwTerminate();
	gl2d::cnv::styles::freeStyles();
}
