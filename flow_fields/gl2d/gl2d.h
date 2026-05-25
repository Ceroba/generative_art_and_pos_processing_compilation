#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <random>
#include "stb_image.h"
#include "stb_image_write.h"

int o[] = {
	1, 0,
	0, 1,
	1, 1,
	0, 0
};
namespace gl2d {
	float CONTEXT_W;
	float CONTEXT_H;
	float lerp(float a, float b, float t) {
		return ((a == b) ? b : (a + (b - a) * t));
	}
	template<typename t, int n = 1>
	void set(t t1[], t t2[]) {
		for (int i = 0; i < n; i++) {
			t1[i] = t2[i];
		}
	}
	class Math {
	public:
		static constexpr double PI = 3.14159265358979323846;
		static constexpr double _2PI = 3.14159265358979323846 * 2;
		static constexpr double HALF_PI = 3.14159265358979323846 / 2;
	};
	//gladglloader gladloader;
	//struct transform {
	//	glm::vec3 translation = glm::vec3(0.0), scale = glm::vec3(1.0); 
	//	glm::vec3 rotation = glm::vec3(0.0);
	//	glm::mat4 tranc = glm::mat4(1.0);
	//	void applaytransform() {
	//		if (rotation.x || rotation.y) {
	//			tranc = glm::rotate(tranc, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	//			tranc = glm::rotate(tranc, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	//			tranc = glm::rotate(tranc, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	//		}
	//		if (translation.x || translation.y || translation.z) {
	//			tranc = glm::translate(tranc, translation);
	//		}
	//		if (!(translation.x + translation.y + translation.z) == 3) {
	//			tranc = glm::scale(tranc, scale);
	//		}
	//	}
	//	glm::vec3 trans(glm::vec2 &v, glm::vec2 o) {
	//		if (rotation.x || rotation.y) {
	//			rotate(v, o, rotation.z);
	//		}
	//		if (translation.x || translation.y || translation.z) {
	//			translate(v, translation);
	//		}
	//		if (!((translation.x + translation.y + translation.z) == 3)) {
	//			tranc = glm::scale(tranc, scale);
	//		}
	//	}
	//	void rotate(glm::vec2 &v, glm::vec2 o, float a) {
	//		float l = dist(v, o);
	//		v = glm::vec2(std::sin(a) * l + o.x, std::cos(a) * l + o.y);
	//	}
	//	void translate(glm::vec2 &v, glm::vec2 o) {
	//		v += o;
	//	}
	//	void scale(glm::vec2 &v, glm::vec2 s) {
	//		v *= s;
	//	}
	//};

	class Shader
	{
	public:
		unsigned int ID;
		// constructor generates the shader on the fly
		// ------------------------------------------------------------------------
		Shader(){}
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
			const char * fShaderCode = fragmentCode.c_str();
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
		void setBool(const std::string &name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		// ------------------------------------------------------------------------
		void setInt(const std::string &name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		// ------------------------------------------------------------------------
		void setFloat(const std::string &name, float value) const
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

	namespace shaders {
		//pass the color as a vec4 name:col
		Shader uniColor = Shader()/*("shaders\\cnv_shaders\\vert.txt", "shaders\\cnv_shaders\\frag.txt")*/;
		void free() {
			uniColor.~Shader();
		}
		void load() {
			uniColor = Shader("C:\\Users\\21355\\Desktop\\c++ files\\openGL environment\\gl2d_dev_sol\\gl2d_dev\\gl2d\\shaders\\cnv_shaders\\vert.txt", "C:\\Users\\21355\\Desktop\\c++ files\\openGL environment\\gl2d_dev_sol\\gl2d_dev\\gl2d\\shaders\\cnv_shaders\\frag.txt");
		}
	}

	class converter {
	public:
		static glm::vec2 pixel_map_to_normel(glm::vec2 a) {
			return glm::vec2(lerp(-1, 1, a.x / CONTEXT_W), lerp(1, -1, a.y / CONTEXT_H));
		}
		static glm::vec2 pixel_to_normel(glm::vec2 a) {
			return glm::vec2(a.x / CONTEXT_W, a.y / CONTEXT_H);
		}
		static glm::vec2 pixel_to_normel_map(glm::vec2 a) {
			return glm::vec2(a.x / (CONTEXT_W / 2), a.y / (CONTEXT_H / 2));
		}
		static glm::vec2 pixel_map_to_opengl_uv_coords(glm::vec2 a, glm::vec2 size) {
			return glm::vec2(a.x / size.x, 1 - (a.y / size.y));
		}
		static glm::vec2 pixel_to_opengl_uv_coords(glm::vec2 a, glm::vec2 size) {
			return glm::vec2(a.x / size.x, a.y / size.y);
		}
	};

	class rect {

	public:
		unsigned int vao, vbo, ebo;
		glm::vec2 pos;
		glm::mat4 tranc;
		rect() {
			pos = glm::vec2(0);
			tranc = glm::mat4(1.0);
			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[12] = {
				1, -1, 0.,
				-1, 1., 0.,
				1., 1., 0.,
				-1, -1, 0
			};
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		rect(glm::vec2 size) {
			pos = glm::vec2(0);
			tranc = glm::mat4(1.0);
			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[] = {
				size.x / 2, -size.y / 2, 0,
				-size.x / 2, size.y / 2, 0,
				size.x / 2, size.y / 2, 0,
				-size.x / 2, -size.y / 2, 0
			};
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		~rect() {
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
			glDeleteVertexArrays(1, &vao);
		}
		void render(Shader &sh) {

			sh.use();
			glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		void move(glm::vec2 off) {
			pos += off;
			tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
		}

		void setpos(glm::vec2 apos) {
			pos = apos;
			tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
		}
	};

	class sprite {

	public:
		unsigned int vao, vbo, ebo, texture;
		glm::vec2 pos;
		glm::mat4 tranc;
		sprite() {
			pos = glm::vec2(0);
			tranc = glm::mat4(1.0);
			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[] = {
				1, -1, 0., 1, 0,
				-1, 1., 0., 0, 1,
				1., 1., 0., 1, 1,
				-1, -1, 0, 0, 0
			};
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
			glBindVertexArray(0);
		}
		sprite(glm::vec2 size) {
			pos = glm::vec2(0);
			tranc = glm::mat4(1.0);
			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[] = {
				size.x / 2, -size.y / 2, 0., 1, 0,
				-size.x / 2, size.y / 2, 0., 0, 1,
				size.x / 2, size.y / 2, 0., 1, 1,
				-size.x / 2, -size.y / 2, 0, 0, 0
			};
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
			glBindVertexArray(0);
		}
		sprite(const char * texturepath, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
			pos = glm::vec2(0);
			tranc = glm::mat4(1.0);
			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[] = {
				1, -1, 0., 1, 0,
				-1, 1., 0., 0, 1,
				1., 1., 0., 1, 1,
				-1, -1, 0, 0, 0
			};
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
			glBindVertexArray(0);
			int w, h, nr;
			unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
			if (texturedata) {
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				for (int i = 0; i < params.size(); i++) {
					glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
				}
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			stbi_image_free(texturedata);
		}
		sprite(const char * texturepath, glm::vec2 size, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
			pos = glm::vec2(0);
			tranc = glm::mat4(1.0);
			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[] = {
				size.x / 2, -size.y / 2, 0., 1, 0,
				-size.x / 2, size.y / 2, 0., 0, 1,
				size.x / 2, size.y / 2, 0., 1, 1,
				-size.x / 2, -size.y / 2, 0, 0, 0
			};
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
			glBindVertexArray(0);
			int w, h, nr;
			unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
			if (texturedata) {
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				for (int i = 0; i < params.size(); i++) {
					glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
				}
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			stbi_image_free(texturedata);
		}
		~sprite() {
			glDeleteBuffers(1, &vbo);
			glDeleteTextures(1, &texture);
			glDeleteBuffers(1, &ebo);
			glDeleteVertexArrays(1, &vao);
		}
		void render(Shader &sh) {

			sh.use();
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		void move(glm::vec2 off) {
			pos += off;
			tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
		}

		void setpos(glm::vec2 apos) {
			pos = apos;
			tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
		}
	};

	class camera {
	public:
		glm::vec3 pos;
		glm::vec3 up;
		glm::vec3 front;
		glm::mat4 view;
		glm::mat4 proj;
		camera(float left, float right, float up, float down, glm::vec3 startpos = glm::vec3(0., 0., 0.), glm::vec3 startfront = glm::vec3(0., 0., -1.0), glm::vec3 startup = glm::vec3(0., 1.0, 0.))
			:pos(startpos), up(startup), front(startfront)
		{
			proj = glm::mat4(1.0);
			proj = glm::ortho(left, right, down, up, -1.0f, 100.f);
			view = glm::mat4(1.0);

		}
		void update(Shader &sh) {

			sh.use();
			glUniformMatrix4fv(glGetUniformLocation(sh.ID, "view"), 1, 0, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(sh.ID, "proj"), 1, 0, glm::value_ptr(proj));
			glUniform3f(glGetUniformLocation(sh.ID, "campos"), pos.x, pos.y, pos.z);
		}
		void move(glm::vec3 off) {
			pos += off;

			//view = glm::translate(view, glm::vec3(-off.x, -off.y, 0.));
		}

		void trshake(glm::vec3 offmax, float shake) {
			std::uniform_real_distribution<double> rng(-1, 1);
			std::random_device rnd;
			float xoff = offmax.x * shake * rng(rnd);
			float yoff = offmax.y * shake * rng(rnd);
			pos += glm::vec3(xoff, yoff, 0);
		}
	};

	class renderer2d {
	public: 
		unsigned int fbo, rbo, frame, vbo, vao, ebo;
		renderer2d() {

			unsigned int ind[6] = {
				0, 3, 1,
				1, 2, 0
			};
			float vert[] = {
				1, -1, 0., 1, 0,
				-1, 1., 0., 0, 1,
				1., 1., 0., 1, 1,
				-1, -1, 0, 0, 0
			};
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
			glBindVertexArray(0);

			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glGenTextures(1, &frame);
			glBindTexture(GL_TEXTURE_2D, frame);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CONTEXT_W, CONTEXT_H, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame, 0);
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, CONTEXT_W, CONTEXT_H);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		~renderer2d() {
			glDeleteBuffers(1, &ebo);
			glDeleteVertexArrays(1, &vao);
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(1, &rbo);
			glDeleteTextures(1, &frame);
			glDeleteBuffers(1, &vbo);
		}/*
		void createcontext(const char *titel, std::vector<int> params, std::vector<int >flags){
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			for (int i = 0; i < params.size(); i++) {
				glfwWindowHint(params[i], flags[i]);
			}
			win = glfwCreateWindow(CONTEXT_W, CONTEXT_H, titel, 0, 0);
			glfwMakeContextCurrent(win);
		}*/
		void display(Shader &sh) {
			sh.use();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindVertexArray(vao);
			glBindTexture(GL_TEXTURE_2D, frame);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	};

	namespace dynamics {
		class sprite {

		public:
			unsigned int vao, vbo, ebo, texture;
			unsigned int ind[6];
			float vert[20];

			glm::vec2 pos;
			glm::mat4 tranc;
			sprite() {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[] = {
					1, -1, 0., 1, 0,
					-1, 1., 0., 0, 1,
					1., 1., 0., 1, 1,
					-1, -1, 0, 0, 0
				};
				set<float, 20>(vert, vert1);

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
				glBindVertexArray(0);
			}

			sprite(glm::vec2 size) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[] = {
					size.x / 2, -size.y / 2, 0., 1, 0,
					-size.x / 2, size.y / 2, 0., 0, 1,
					size.x / 2, size.y / 2, 0., 1, 1,
					-size.x / 2, -size.y / 2, 0, 0, 0
				};
				set<float, 20>(vert, vert1);
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
				glBindVertexArray(0);
			}
			sprite(glm::vec2 size, glm::vec2 uvs_start, glm::vec2 uvs_end) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[] = {
					size.x / 2, -size.y / 2, 0., uvs_end.x, uvs_start.y,
					-size.x / 2, size.y / 2, 0., uvs_start.x, uvs_end.y,
					size.x / 2, size.y / 2, 0., uvs_end.x, uvs_end.y,
					-size.x / 2, -size.y / 2, 0, uvs_start.x, uvs_start.y
				};
				set<float, 20>(vert, vert1);
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
				glBindVertexArray(0);
			}
			sprite(const char * texturepath, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[] = {
					1, -1, 0., 1, 0,
					-1, 1., 0., 0, 1,
					1., 1., 0., 1, 1,
					-1, -1, 0, 0, 0
				};
				set<float, 20>(vert, vert1);
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
				glBindVertexArray(0);
				int w, h, nr;
				unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
				if (texturedata) {
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					for (int i = 0; i < params.size(); i++) {
						glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
					}
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				stbi_image_free(texturedata);
			}
			sprite(const char* texturepath, glm::vec2 size, glm::vec2 uvs_start, glm::vec2 uvs_end, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[] = {
					size.x / 2, -size.y / 2, 0., uvs_end.x, uvs_start.y,
					-size.x / 2, size.y / 2, 0., uvs_start.x, uvs_end.y,
					size.x / 2, size.y / 2, 0., uvs_end.x, uvs_end.y,
					-size.x / 2, -size.y / 2, 0, uvs_start.x, uvs_start.y
				};
				set<float, 20>(vert, vert1);
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
				glBindVertexArray(0);
				int w, h, nr;
				unsigned char* texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
				if (texturedata) {
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					for (int i = 0; i < params.size(); i++) {
						glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
					}
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				stbi_image_free(texturedata);
			}
			sprite(const char * texturepath, glm::vec2 size, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
					set<unsigned int, 6>(ind, ind1);
				float vert1[] = {
					size.x / 2, -size.y / 2, 0., 1, 0,
					-size.x / 2, size.y / 2, 0., 0, 1,
					size.x / 2, size.y / 2, 0., 1, 1,
					-size.x / 2, -size.y / 2, 0, 0, 0
				};
					set<float, 20>(vert, vert1);
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
				glBindVertexArray(0);
				int w, h, nr;
				unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
				if (texturedata) {
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					for (int i = 0; i < params.size(); i++) {
						glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
					}
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				stbi_image_free(texturedata);
			}
			void setteexcoords(glm::vec2 uvs_start, glm::vec2 uvs_end) {
				glBindBuffer(GL_ARRAY_BUFFER, vbo);

				for (int i = 0; i < 4; i++) {
					vert[i * 5 + 3] = (o[i * 2] ? uvs_end.x : uvs_start.x);
					vert[i * 5 + 3 + 1] = (o[i * 2 + 1] ? uvs_end.y : uvs_start.y);
				}
				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

			}
			~sprite() {
				glDeleteBuffers(1, &vbo);
				glDeleteTextures(1, &texture);
				glDeleteBuffers(1, &ebo);
				glDeleteVertexArrays(1, &vao);
			}
			void render(Shader &sh) {

				sh.use();
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
				glBindVertexArray(vao);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			void move(glm::vec2 off) {
				pos += off;
				tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
			}

			void setpos(glm::vec2 apos) {
				pos = apos;
				tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
			}
			std::vector<glm::vec3> GetTransformedVertix() {
				static std::vector<glm::vec3> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * 5], vert[i * 5 + 1], vert[i * 5 + 2], 0.0);
				}
				return verts;
			}
			std::vector<glm::vec3> GetTransformedVertix(int stride) {
				static std::vector<glm::vec3> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], vert[i * stride + 2], 0.0);
				}
				return verts;
			}
			void GetTransformedVertix(glm::vec3 * t) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * 5], vert[i * 5 + 1], vert[i * 5 + 2], 0.0);
				}
			}
			void GetTransformedVertix(glm::vec3 * t, int stride) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], vert[i * stride + 2], 0.0);
				}
			}
			std::vector<glm::vec2> Get2dTransformedVertix() {
				static std::vector<glm::vec2> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * 5], vert[i * 5 + 1],0.0, 0.0);
				}
				return verts;
			}
			std::vector<glm::vec2> Get2dTransformedVertix(int stride) {
				static std::vector<glm::vec2> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
				}
				return verts;
			}
			void GetTransformedVertix(glm::vec2 * t) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * 5], vert[i * 5 + 1], 0.0, 0.0);
				}
			}
			void GetTransformedVertix(glm::vec2 * t, int stride) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1],0.0, 0.0);
				}
			}
		};

		class rect {

		public:
			unsigned int vao, vbo, ebo;
			unsigned int ind[6];
			float vert[12];
			glm::vec2 pos;
			glm::mat4 tranc;
			rect() {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[12] = {
					1, -1, 0.,
					-1, 1., 0.,
					1., 1., 0.,
					-1, -1, 0
				};
				set<float, 12>(vert, vert1);
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glGenBuffers(1, &ebo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			rect(glm::vec2 size) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);			
				unsigned int ind1[6] = {
					0, 3, 1,
					1, 2, 0
				};
				set<unsigned int, 6>(ind, ind1);
				float vert1[12] = {
					size.x / 2, -size.y / 2, 0,
					-size.x / 2, size.y / 2, 0,
					size.x / 2, size.y / 2, 0,
					-size.x / 2, -size.y / 2, 0
				};
				set<float, 12>(vert, vert1);
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glGenBuffers(1, &ebo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			~rect() {
				glDeleteBuffers(1, &vbo);
				glDeleteBuffers(1, &ebo);
				glDeleteVertexArrays(1, &vao);
			}
			void render(Shader &sh) {

				sh.use();
				glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
				glBindVertexArray(vao);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			void move(glm::vec2 off) {
				pos += off;
				tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
			}

			void setpos(glm::vec2 apos) {
				pos = apos;
				tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
			}
			std::vector<glm::vec3> GetTransformedVertix() {
				static std::vector<glm::vec3> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], vert[i * 3 + 2], 0.0);
				}
				return verts;
			}
			std::vector<glm::vec3> GetTransformedVertix(int stride) {
				static std::vector<glm::vec3> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], vert[i * stride + 2], 0.0);
				}
				return verts;
			}
			void GetTransformedVertix(glm::vec3 * t) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], vert[i * 3 + 2], 0.0);
				}
			}
			void GetTransformedVertix(glm::vec3 * t, int stride) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], vert[i * stride + 2], 0.0);
				}
			}

			std::vector<glm::vec2> Get2dTransformedVertix() {
				static std::vector<glm::vec2> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], 0.0, 0.0);
				}
				return verts;
			}
			std::vector<glm::vec2> Get2dTransformedVertix(int stride) {
				static std::vector<glm::vec2> verts(12);
				for (int i = 0; i < 4; i++) {
					verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
				}
				return verts;
			}
			void GetTransformedVertix(glm::vec2 * t) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], 0.0, 0.0);
				}
			}
			void GetTransformedVertix(glm::vec2 * t, int stride) {
				for (int i = 0; i < 4; i++) {
					t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
				}
			}
		};

	}
	
	//namespace transformed {
	//	class sprite {
	//	public:
	//		unsigned int vao, vbo, ebo, texture;
	//		unsigned int ind[6];
	//		float vert[20];
	//		glm::vec2 pos;
	//		transform tranc;
	//		sprite() {
	//			pos = glm::vec2(0);
	//			unsigned int ind1[6] = {
	//				0, 3, 1,
	//				1, 2, 0
	//			};
	//			set<unsigned int, 6>(ind, ind1);
	//			float vert1[] = {
	//				1, -1, 0., 1, 0,
	//				-1, 1., 0., 0, 1,
	//				1., 1., 0., 1, 1,
	//				-1, -1, 0, 0, 0
	//			};
	//			set<float, 20>(vert, vert1);
	//			glGenVertexArrays(1, &vao);
	//			glBindVertexArray(vao);
	//			glGenBuffers(1, &vbo);
	//			glGenBuffers(1, &ebo);
	//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//			glEnableVertexAttribArray(0);
	//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//			glEnableVertexAttribArray(1);
	//			glBindBuffer(GL_ARRAY_BUFFER, 0);
	//			glBindVertexArray(0);
	//		}
	//		sprite(glm::vec2 size) {
	//			pos = glm::vec2(0);
	//			unsigned int ind1[6] = {
	//				0, 3, 1,
	//				1, 2, 0
	//			};
	//			set<unsigned int, 6>(ind, ind1);
	//			float vert1[] = {
	//				size.x / 2, -size.y / 2, 0., 1, 0,
	//				-size.x / 2, size.y / 2, 0., 0, 1,
	//				size.x / 2, size.y / 2, 0., 1, 1,
	//				-size.x / 2, -size.y / 2, 0, 0, 0
	//			};
	//			set<float, 20>(vert, vert1);
	//			glGenVertexArrays(1, &vao);
	//			glBindVertexArray(vao);
	//			glGenBuffers(1, &vbo);
	//			glGenBuffers(1, &ebo);
	//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//			glEnableVertexAttribArray(0);
	//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//			glEnableVertexAttribArray(1);
	//			glBindBuffer(GL_ARRAY_BUFFER, 0);
	//			glBindVertexArray(0);
	//		}
	//		sprite(const char * texturepath, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
	//			pos = glm::vec2(0);
	//			unsigned int ind1[6] = {
	//				0, 3, 1,
	//				1, 2, 0
	//			};
	//			set<unsigned int, 6>(ind, ind1);
	//			float vert1[] = {
	//				1, -1, 0., 1, 0,
	//				-1, 1., 0., 0, 1,
	//				1., 1., 0., 1, 1,
	//				-1, -1, 0, 0, 0
	//			};
	//			set<float, 20>(vert, vert1);
	//			glGenVertexArrays(1, &vao);
	//			glBindVertexArray(vao);
	//			glGenBuffers(1, &vbo);
	//			glGenBuffers(1, &ebo);
	//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//			glEnableVertexAttribArray(0);
	//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//			glEnableVertexAttribArray(1);
	//			glBindBuffer(GL_ARRAY_BUFFER, 0);
	//			glBindVertexArray(0);
	//			int w, h, nr;
	//			unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
	//			if (texturedata) {
	//				glGenTextures(1, &texture);
	//				glBindTexture(GL_TEXTURE_2D, texture);
	//				for (int i = 0; i < params.size(); i++) {
	//					glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
	//				}
	//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
	//				glGenerateMipmap(GL_TEXTURE_2D);
	//			}
	//			stbi_image_free(texturedata);
	//		}
	//		sprite(const char * texturepath, glm::vec2 size, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
	//			pos = glm::vec2(0);
	//			unsigned int ind1[6] = {
	//				0, 3, 1,
	//				1, 2, 0
	//			};
	//			set<unsigned int, 6>(ind, ind1);
	//			float vert1[] = {
	//				size.x / 2, -size.y / 2, 0., 1, 0,
	//				-size.x / 2, size.y / 2, 0., 0, 1,
	//				size.x / 2, size.y / 2, 0., 1, 1,
	//				-size.x / 2, -size.y / 2, 0, 0, 0
	//			};
	//			set<float, 20>(vert, vert1);
	//			glGenVertexArrays(1, &vao);
	//			glBindVertexArray(vao);
	//			glGenBuffers(1, &vbo);
	//			glGenBuffers(1, &ebo);
	//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//			glEnableVertexAttribArray(0);
	//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//			glEnableVertexAttribArray(1);
	//			glBindBuffer(GL_ARRAY_BUFFER, 0);
	//			glBindVertexArray(0);
	//			int w, h, nr;
	//			unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
	//			if (texturedata) {
	//				glGenTextures(1, &texture);
	//				glBindTexture(GL_TEXTURE_2D, texture);
	//				for (int i = 0; i < params.size(); i++) {
	//					glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
	//				}
	//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
	//				glGenerateMipmap(GL_TEXTURE_2D);
	//			}
	//			stbi_image_free(texturedata);
	//		}
	//		~sprite() {
	//			glDeleteBuffers(1, &vbo);
	//			glDeleteTextures(1, &texture);
	//			glDeleteBuffers(1, &ebo);
	//			glDeleteVertexArrays(1, &vao);
	//		}
	//		void render(Shader &sh) {
	//			tranc.applaytransform();
	//			sh.use();
	//			glBindTexture(GL_TEXTURE_2D, texture);
	//			glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc.tranc));
	//			glBindVertexArray(vao);
	//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//		}
	//		void move(glm::vec2 off) {
	//			pos += off;
	//			tranc.translation += off;
	//		}
	//		void setpos(glm::vec2 apos) {
	//			pos = apos;
	//			tranc.translation = glm::vec3(apos, 0.0);
	//		}
	//		std::vector<glm::vec3> GetTransformedVertix() {
	//			static std::vector<glm::vec3> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc.trans(glm::vec2(vert[i * 5], vert[i * 5 + 1]), pos);
	//			}
	//			return verts;
	//		}
	//		std::vector<glm::vec3> GetTransformedVertix(int stride) {
	//			static std::vector<glm::vec3> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc.trans(glm::vec2(vert[i * stride], vert[i * stride + 1]), pos);
	//			}
	//			return verts;
	//		}
	//		void GetTransformedVertix(glm::vec3 * t) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc.trans(glm::vec2(vert[i * 5], vert[i * 5 + 1]), pos);
	//			}
	//		}
	//		void GetTransformedVertix(glm::vec3 * t, int stride) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc.trans(glm::vec2(vert[i * stride], vert[i * stride + 1]), pos);
	//			}
	//		}
	//	/*	std::vector<glm::vec2> Get2dTransformedVertix() {
	//			static std::vector<glm::vec2> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc * glm::vec4(vert[i * 5], vert[i * 5 + 1], 0.0, 0.0);
	//			}
	//			return verts;
	//		}
	//		std::vector<glm::vec2> Get2dTransformedVertix(int stride) {
	//			static std::vector<glm::vec2> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
	//			}
	//			return verts;
	//		}
	//		void GetTransformedVertix(glm::vec2 * t) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc * glm::vec4(vert[i * 5], vert[i * 5 + 1], 0.0, 0.0);
	//			}
	//		}
	//		void GetTransformedVertix(glm::vec2 * t, int stride) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
	//			}
	//		}*/
	//	};
	//	class rect {
	//	public:
	//		unsigned int vao, vbo, ebo;
	//		unsigned int ind[6];
	//		float vert[12];
	//		glm::vec2 pos;
	//		transform tranc;
	//		rect() {
	//			pos = glm::vec2(0);
	//			unsigned int ind1[6] = {
	//				0, 3, 1,
	//				1, 2, 0
	//			};
	//			set<unsigned int, 6>(ind, ind1);
	//			float vert1[12] = {
	//				1, -1, 0.,
	//				-1, 1., 0.,
	//				1., 1., 0.,
	//				-1, -1, 0
	//			};
	//			set<float, 12>(vert, vert1);
	//			glGenVertexArrays(1, &vao);
	//			glBindVertexArray(vao);
	//			glGenBuffers(1, &vbo);
	//			glGenBuffers(1, &ebo);
	//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//			glEnableVertexAttribArray(0);
	//			glBindBuffer(GL_ARRAY_BUFFER, 0);
	//			glBindVertexArray(0);
	//		}
	//		rect(glm::vec2 size) {
	//			pos = glm::vec2(0);
	//			unsigned int ind1[6] = {
	//				0, 3, 1,
	//				1, 2, 0
	//			};
	//			set<unsigned int, 6>(ind, ind1);
	//			float vert1[12] = {
	//				size.x / 2, -size.y / 2, 0,
	//				-size.x / 2, size.y / 2, 0,
	//				size.x / 2, size.y / 2, 0,
	//				-size.x / 2, -size.y / 2, 0
	//			};
	//			set<float, 12>(vert, vert1);
	//			glGenVertexArrays(1, &vao);
	//			glBindVertexArray(vao);
	//			glGenBuffers(1, &vbo);
	//			glGenBuffers(1, &ebo);
	//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//			glEnableVertexAttribArray(0);
	//			glBindBuffer(GL_ARRAY_BUFFER, 0);
	//			glBindVertexArray(0);
	//		}
	//		~rect() {
	//			glDeleteBuffers(1, &vbo);
	//			glDeleteBuffers(1, &ebo);
	//			glDeleteVertexArrays(1, &vao);
	//		}
	//		void render(Shader &sh) {
	//			tranc.applaytransform();
	//			sh.use();
	//			glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
	//			glBindVertexArray(vao);
	//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//		}
	//		void move(glm::vec2 off) {
	//			pos += off;
	//			tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
	//		}
	//		void setpos(glm::vec2 apos) {
	//			pos = apos;
	//			tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
	//		}
	//		std::vector<glm::vec3> GetTransformedVertix() {
	//			static std::vector<glm::vec3> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], vert[i * 3 + 2], 0.0);
	//			}
	//			return verts;
	//		}
	//		std::vector<glm::vec3> GetTransformedVertix(int stride) {
	//			static std::vector<glm::vec3> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], vert[i * stride + 2], 0.0);
	//			}
	//			return verts;
	//		}
	//		void GetTransformedVertix(glm::vec3 * t) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], vert[i * 3 + 2], 0.0);
	//			}
	//		}
	//		void GetTransformedVertix(glm::vec3 * t, int stride) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], vert[i * stride + 2], 0.0);
	//			}
	//		}
	//		std::vector<glm::vec2> Get2dTransformedVertix() {
	//			static std::vector<glm::vec2> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], 0.0, 0.0);
	//			}
	//			return verts;
	//		}
	//		std::vector<glm::vec2> Get2dTransformedVertix(int stride) {
	//			static std::vector<glm::vec2> verts(12);
	//			for (int i = 0; i < 4; i++) {
	//				verts[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
	//			}
	//			return verts;
	//		}
	//		void GetTransformedVertix(glm::vec2 * t) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc * glm::vec4(vert[i * 3], vert[i * 3 + 1], 0.0, 0.0);
	//			}
	//		}
	//		void GetTransformedVertix(glm::vec2 * t, int stride) {
	//			for (int i = 0; i < 4; i++) {
	//				t[i] = tranc * glm::vec4(vert[i * stride], vert[i * stride + 1], 0.0, 0.0);
	//			}
	//		}
	//	};
	//}

	namespace fast {
		unsigned int rectebo;
		void init() {
			unsigned int ind1[6] = {
				0, 3, 1,
				1, 2, 0
			};
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind1), ind1, GL_STATIC_DRAW);

		}
		void termanate() {
			glDeleteBuffers(1, &rectebo);
		}
		class rect {

		public:
			unsigned int vao, vbo;
			glm::vec2 pos;
			glm::mat4 tranc;
			rect() {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);

				float vert[12] = {
					1, -1, 0.,
					-1, 1., 0.,
					1., 1., 0.,
					-1, -1, 0
				};
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			rect(glm::vec2 size) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
				float vert[12] = {
					size.x / 2, -size.y / 2,
					-size.x / 2, size.y / 2,
					size.x / 2, size.y / 2,
					-size.x / 2, -size.y / 2
				};
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			~rect() {
				glDeleteBuffers(1, &vbo);
				glDeleteVertexArrays(1, &vao);
			}
			void render(Shader &sh) {

				sh.use();
				glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
				glBindVertexArray(vao);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			void move(glm::vec2 off) {
				pos += off;
				tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
			}

			void setpos(glm::vec2 apos) {
				pos = apos;
				tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
			}
		};

		class sprite {

		public:
			unsigned int vao, vbo, texture;
			glm::vec2 pos;
			glm::mat4 tranc;
			sprite() {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);

				float vert[] = {
					1, -1, 0., 1, 0,
					-1, 1., 0., 0, 1,
					1., 1., 0., 1, 1,
					-1, -1, 0, 0, 0
				};
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			sprite(glm::vec2 size) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);

				float vert[] = {
					size.x / 2, -size.y / 2, 0., 1, 0,
					-size.x / 2, size.y / 2, 0., 0, 1,
					size.x / 2, size.y / 2, 0., 1, 1,
					-size.x / 2, -size.y / 2, 0, 0, 0
				};
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			sprite(const char * texturepath, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);
;
				float vert[] = {
					1, -1, 0., 1, 0,
					-1, 1., 0., 0, 1,
					1., 1., 0., 1, 1,
					-1, -1, 0, 0, 0
				};
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
				int w, h, nr;
				unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
				if (texturedata) {
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					for (int i = 0; i < params.size(); i++) {
						glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
					}
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				stbi_image_free(texturedata);
			}
			sprite(const char * texturepath, glm::vec2 size, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
				pos = glm::vec2(0);
				tranc = glm::mat4(1.0);

				float vert[] = {
					size.x / 2, -size.y / 2, 0., 1, 0,
					-size.x / 2, size.y / 2, 0., 0, 1,
					size.x / 2, size.y / 2, 0., 1, 1,
					-size.x / 2, -size.y / 2, 0, 0, 0
				};
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
				int w, h, nr;
				unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
				if (texturedata) {
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					for (int i = 0; i < params.size(); i++) {
						glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
					}
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				stbi_image_free(texturedata);
			}
			~sprite() {
				glDeleteBuffers(1, &vbo);
				glDeleteTextures(1, &texture);
				glDeleteVertexArrays(1, &vao);
			}
			void render(Shader &sh) {

				sh.use();
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
				glBindVertexArray(vao);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			void move(glm::vec2 off) {
				pos += off;
				tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
			}

			void setpos(glm::vec2 apos) {
				pos = apos;
				tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
			}
		};

		namespace dynamics {
			class sprite {

			public:
				unsigned int vao, vbo, texture;
				float vert[20];

				glm::vec2 pos;
				glm::mat4 tranc;
				sprite() {
					pos = glm::vec2(0);
					tranc = glm::mat4(1.0);

					float vert1[] = {
						1, -1, 0., 1, 0,
						-1, 1., 0., 0, 1,
						1., 1., 0., 1, 1,
						-1, -1, 0, 0, 0
					};
					set<float, 20>(vert, vert1);

					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

					glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					
				}

				sprite(glm::vec2 size) {
					pos = glm::vec2(0);
					tranc = glm::mat4(1.0);

					float vert1[] = {
						size.x / 2, -size.y / 2, 0., 1, 0,
						-size.x / 2, size.y / 2, 0., 0, 1,
						size.x / 2, size.y / 2, 0., 1, 1,
						-size.x / 2, -size.y / 2, 0, 0, 0
					};
					set<float, 20>(vert, vert1);
					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

					glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}
				sprite(const char * texturepath, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
					pos = glm::vec2(0);
					tranc = glm::mat4(1.0);

					float vert1[] = {
						1, -1, 0., 1, 0,
						-1, 1., 0., 0, 1,
						1., 1., 0., 1, 1,
						-1, -1, 0, 0, 0
					};
					set<float, 20>(vert, vert1);
					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

					glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					int w, h, nr;
					unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
					if (texturedata) {
						glGenTextures(1, &texture);
						glBindTexture(GL_TEXTURE_2D, texture);

						for (int i = 0; i < params.size(); i++) {
							glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
						}
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
						glGenerateMipmap(GL_TEXTURE_2D);
					}
					stbi_image_free(texturedata);
				}

				sprite(const char * texturepath, glm::vec2 size, std::vector<GLenum> params = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER }, std::vector<GLint >flags = { GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST }) {
					pos = glm::vec2(0);
					tranc = glm::mat4(1.0);

					float vert1[] = {
						size.x / 2, -size.y / 2, 0., 1, 0,
						-size.x / 2, size.y / 2, 0., 0, 1,
						size.x / 2, size.y / 2, 0., 1, 1,
						-size.x / 2, -size.y / 2, 0, 0, 0
					};
					set<float, 20>(vert, vert1);
					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

					glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					int w, h, nr;
					unsigned char *texturedata = stbi_load(texturepath, &w, &h, &nr, 4);
					if (texturedata) {
						glGenTextures(1, &texture);
						glBindTexture(GL_TEXTURE_2D, texture);

						for (int i = 0; i < params.size(); i++) {
							glTexParameteri(GL_TEXTURE_2D, params[i], flags[i]);
						}
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
						glGenerateMipmap(GL_TEXTURE_2D);
					}
					stbi_image_free(texturedata);
				}
				~sprite() {
					glDeleteBuffers(1, &vbo);
					glDeleteTextures(1, &texture);
					glDeleteVertexArrays(1, &vao);
				}
				void render(Shader &sh) {

					sh.use();
					glBindTexture(GL_TEXTURE_2D, texture);
					glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
					glBindVertexArray(vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				void move(glm::vec2 off) {
					pos += off;
					tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
				}

				void setpos(glm::vec2 apos) {
					pos = apos;
					tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
				}

			};

			class rect {

			public:
				unsigned int vao, vbo;
				float vert[12];
				glm::vec2 pos;
				glm::mat4 tranc;
				rect() {
					pos = glm::vec2(0);
					tranc = glm::mat4(1.0);

					float vert1[12] = {
						1, -1, 0.,
						-1, 1., 0.,
						1., 1., 0.,
						-1, -1, 0
					};
					set<float, 12>(vert, vert1);
					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

					glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}
				rect(glm::vec2 size) {
					pos = glm::vec2(0);
					tranc = glm::mat4(1.0);

					float vert1[12] = {
						size.x / 2, -size.y / 2,
						-size.x / 2, size.y / 2,
						size.x / 2, size.y / 2,
						-size.x / 2, -size.y / 2
					};
					set<float, 12>(vert, vert1);
					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectebo);

					glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}
				~rect() {
					glDeleteBuffers(1, &vbo);
					glDeleteVertexArrays(1, &vao);
				}
				void render(Shader &sh) {

					sh.use();
					glUniformMatrix4fv(glGetUniformLocation(sh.ID, "tranc"), 1, 0, glm::value_ptr(tranc));
					glBindVertexArray(vao);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				void move(glm::vec2 off) {
					pos += off;
					tranc = glm::translate(tranc, glm::vec3(off.x, off.y, 0.0));
				}

				void setpos(glm::vec2 apos) {
					pos = apos;
					tranc = glm::translate(glm::mat4(1), glm::vec3(apos.x, apos.y, 0.0));
				}
			};
		}
	}

	namespace cnv {
		struct style
		{
			//drawing properties
			bool noFill = false;
			bool useEBO = false;
			bool inPixels = false;
			//color properties
			bool isColorNorml = false;
			bool isAlpha = true;
			glm::vec4 color = glm::vec4(0);
			//anti aliase properties
			bool antiAlias = false;
			float antiAliasLevel = 4.f;
			//style();
			//
			//bool noFill = false;
			//bool noFill = false;
			//bool noFill = false;
			//bool noFill = false;
			//bool noFill = false;
		};
		typedef struct style style;
		namespace styles {
			style *antiAliased = new style{ false, false, true, false, true, glm::vec4(255, 255, 255, 255), true, 4.f };
			style *defualt = new style{ false, false, true, false, true, glm::vec4(255, 255, 255, 255), false, 0 };
			void freeStyles() {
				delete antiAliased;
				delete defualt;
			}
		}
		class canvas {
		public:
			style* _style;
			unsigned int vao, *ebo, vbo;
			glm::vec2 _size;
			std::vector<float> vert;
			GLFWwindow* win;
			glm::vec2 prev;
			std::vector<unsigned int> ind;
			glm::vec2 pixel_map_to_normel(glm::vec2 a) {
				return glm::vec2(lerp(-1, 1, a.x / _size.x), lerp(1, -1, a.y / _size.y));
			}
			glm::vec2 pixel_to_normel(glm::vec2 a) {
				return glm::vec2(a.x / _size.x, a.y / _size.y);
			}
			glm::vec2 pixel_to_normel_map(glm::vec2 a) {
				return glm::vec2(a.x / (_size.x / 2), a.y / (_size.y / 2));
			}
			void beginPath() {
				if (_style->antiAlias) {
					glEnable(GL_MULTISAMPLE);
					glfwSetWindowAttrib(win, GLFW_SAMPLES, _style->antiAliasLevel);
				}
				if (_style->isAlpha) {
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				if (_style->useEBO)glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			void goTo(glm::vec2 pos) {
				prev = pos;
			}
			void lineTo(glm::vec2 to) {
				if (_style->inPixels) {
					glm::vec2 prevnorm = pixel_map_to_normel(prev);
					glm::vec2 toinnorm = pixel_map_to_normel(to);
					vert.push_back(prevnorm.x);
					vert.push_back(prevnorm.y);
					vert.push_back(0);
					vert.push_back(toinnorm.x);
					vert.push_back(toinnorm.y);
					vert.push_back(0);
					//prev = toinnorm;

				}
				else
				{
					vert.push_back(prev.x);
					vert.push_back(prev.y);
					vert.push_back(0);
					vert.push_back(to.x);
					vert.push_back(to.y);
					vert.push_back(0);
				}
					prev = to;
			}
			void line(glm::vec2 from, glm::vec2 to) {
				if (_style->inPixels) {
					glm::vec2 prevnorm = pixel_map_to_normel(from);
					glm::vec2 toinnorm = pixel_map_to_normel(to);
					vert.push_back(prevnorm.x);
					vert.push_back(prevnorm.y);
					vert.push_back(0);
					vert.push_back(toinnorm.x);
					vert.push_back(toinnorm.y);
					vert.push_back(0);
					//prev = toinnorm;

				}
				else
				{
					vert.push_back(from.x);
					vert.push_back(from.y);
					vert.push_back(0);
					vert.push_back(to.x);
					vert.push_back(to.y);
					vert.push_back(0);
				}
			}
			void arc(glm::vec2 pos, float rad, float start, float end, uint32_t vertex_count = 64u) {
				float step = (end - start) / vertex_count;
				goTo(glm::vec2(std::cos(start) * rad + pos.x, std::sin(start) * rad + pos.y));
				for (float t = 0; t <= (end - start); t += step) {
					lineTo(glm::vec2(std::cos(t * (1) + start) * rad + pos.x, std::sin(t * (1) + start) * rad + pos.y));
				}
			}
			void fill(Shader& sh = shaders::uniColor) {
				sh.use();
				if (_style->isColorNorml)
				{
					glUniform4f(glGetUniformLocation(sh.ID, "col"), _style->color.x, _style->color.y, _style->color.z, _style->color.w);
				}
				else {
					glUniform4f(glGetUniformLocation(sh.ID, "col"), _style->color.x / 255.f, _style->color.y / 255.f, _style->color.z / 255.f, _style->color.w / 255.f);

				}
				float x = 0, y = 0;
				for (int i = 0; i < vert.size() / 3; i++) {
					x += vert[i * 3];
					y += vert[i * 3 + 1];
				}
				x /= vert.size() / 3;
				y /= vert.size() / 3;
				vert.push_back(x);
				vert.push_back(y);
				vert.push_back(0);
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);
				if (_style->useEBO) {
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
					glDrawElements(GL_LINES, static_cast<unsigned int>(ind.size()), GL_UNSIGNED_INT, (void*)0);
				}
				else {
					glDrawArrays(GL_TRIANGLE_FAN, 0, vert.size() / 3);
				}
				vert.clear();
				ind.clear();
				if (_style->useEBO) {
					glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, ebo);
					//delete ebo; 
				}
				if (_style->antiAlias) {
					//glDisable(GL_MULTISAMPLE);
					//glfwSetWindowAttrib(win, GLFW_SAMPLES, _style->antiAliasLevel);
				}
				if (_style->isAlpha) {
					glDisable(GL_BLEND);
				}
				//glDeleteBuffers(GL_ARRAY_BUFFER, &vbo);
			}
			void stroke(Shader& sh = shaders::uniColor) {
				sh.use();
				if(_style->isColorNorml)
				{
					glUniform4f(glGetUniformLocation(sh.ID, "col"), _style->color.x, _style->color.y, _style->color.z, _style->color.w);
				}
				else {
					glUniform4f(glGetUniformLocation(sh.ID, "col"), _style->color.x / 255.f, _style->color.y / 255.f, _style->color.z / 255.f, _style->color.w / 255.f);

				}
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);
				if (_style->useEBO) {
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
					glDrawElements(GL_LINES, static_cast<unsigned int>(ind.size()), GL_UNSIGNED_INT, (void*)0);
				}
				else {
					glDrawArrays(GL_LINES, 0, vert.size() / 3);
				}
				vert.clear();
				ind.clear();
				if (_style->useEBO) {
					glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, ebo);
					//delete ebo; 
				}
				if (_style->antiAlias) {
					//glDisable(GL_MULTISAMPLE);
					//glfwSetWindowAttrib(win, GLFW_SAMPLES, _style->antiAliasLevel);
				}
				if (_style->isAlpha) {
					glDisable(GL_BLEND);
				}
				//glDeleteBuffers(GL_ARRAY_BUFFER, &vbo);
			}
			void drawFromMem(Shader& sh = shaders::uniColor) {
				sh.use();
				if (_style->isColorNorml)
				{
					glUniform4f(glGetUniformLocation(sh.ID, "col"), _style->color.x, _style->color.y, _style->color.z, _style->color.w);
				}
				else {
					glUniform4f(glGetUniformLocation(sh.ID, "col"), _style->color.x / 255.f, _style->color.y / 255.f, _style->color.z / 255.f, _style->color.w / 255.f);

				}
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);
				if (_style->useEBO) {
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
					glDrawElements(GL_LINES, static_cast<unsigned int>(ind.size()), GL_UNSIGNED_INT, (void*)0);
				}
				else {
					glDrawArrays(GL_LINES, 0, vert.size() / 3);
				}
			}
			canvas(glm::vec2 size, style *stl = styles::defualt) 
				:_style(stl), _size(size)
			{
				prev = glm::vec2(0);
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);
				if (_style->useEBO) {
					ebo = new unsigned int;
					glGenBuffers(1, ebo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
				}
			}
			~canvas() {
				glDeleteBuffers(1, &vbo);
				glDeleteVertexArrays(1, &vao);
				if (_style->useEBO) {
					glDeleteBuffers(1, ebo);
					//delete ebo; 
				}
			}
		};
	}
}