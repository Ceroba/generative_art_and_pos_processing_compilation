//#pragma once
//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
//#include "glm\glm.hpp"
//#include "glm\gtc\type_ptr.hpp"
//#include "glm\gtc\matrix_transform.hpp"
//#include <iostream>
//#include "stb_image.h"
//#include "stb_image_write.h"
//#include "stb_truetype.h"
//#include <fstream>
//#include <ostream>
//#include <istream>
//#include <vector>
//#include "glm/detail/type_float.hpp"
//#define _OPNEGL_SHADER_VERSION "#version 330"
//#define _OPNEGL_SHADER_PRECISION "precision highp float;"
//#define _OPNEGL_BUFFER_USAGE GL_STREAM_DRAW
//#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consectetur adipiscing \nelit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n Ut enim ad minim veniam,\n quis nostrud exercitation ullamco laboris ni\nsi ut aliquip ex ea commodo consequat.\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
//namespace libname {
//	bool init();
//	void cleanup();
//
//
//
//#pragma region shaderes
//	struct shader_prog
//	{
//		uint32_t id = 0;
//		int sampler = 0;
//		void bind() { glUseProgram(id); }
//		void clean() { glDeleteProgram(id); }
//	};
//	bool velidateshader(uint32_t id);
//	shader_prog creatshaderprogram(const char* vertex, const char* fragment);
//	shader_prog creatshaderprogramfromfile(const char* vertex_file, const char* fragment_file);
//#pragma endregion
//#pragma region texture
//	struct texture
//	{
//		uint32_t id;
//		int w, h, n;
//		uint32_t loadfrompixledata(const unsigned char* data, glm::ivec3 dem, bool pixlated = true, bool use_mipmap = true);
//		uint32_t loadfromfile(const char* filename, bool pixlated = true, bool use_mipmap = true);
//		glm::vec4 pos2texcoords(glm::vec4 pos);
//		void cleanup();
//	};
//#pragma endregion
//#pragma region font
//	struct font {
//		texture text = {};
//		glm::ivec2 size = {};
//		stbtt_packedchar* pakedCHbuff = 0;
//		uint32_t buff_size = 0;
//		float max_hight = 0;
//
//		void creatfromttf(const unsigned char* data, bool pixlated = true, bool usemip = false);
//		void creatfromfile(const char* file, bool pixlated = true, bool usemip = false);
//
//		void cleanup();
//	};
//#pragma endregion
//
//#pragma region renderer
//
//	class renderer {
//	public:
//		enum buff_t : uint32_t
//		{
//			vbo,
//			ebo,
//			tpbo,
//			cbo,
//			buff_c
//		};
//		struct shape_i {
//			uint32_t ind4shape;
//			uint32_t texture;
//		};
//		shader_prog current_sh = {};
//		
//		std::vector<glm::vec2> vertecies;
//		std::vector<glm::vec2> texcoords;
//		std::vector<glm::vec4> colors;
//		std::vector<shape_i> shape_info;
//		std::vector<uint32_t> indecies;
//		uint32_t buffs[buff_t::buff_c];
//		uint32_t vao = 0;
//		glm::vec2 scr_size = {};
//		
//		renderer() = default;
//		void creat(uint32_t fbo = 0, size_t count = 1000);
//		void cleanup();
//		void reset();
//
//		void enableGLfeatures();
//		void cleardrawdata();
//		void clear(glm::vec4 color = { 1, 1, 1, 1 });
//		void flush(bool clear_draw_data = true);
//		void updatecontex(glm::vec2 s);
//
//		void setshader(shader_prog sh);
//
//		void renderrectangle(const glm::vec4& rect, const glm::vec4& color, const texture& tex, const glm::vec4 &tex_coords = {0, 0, 1, 1}, const glm::vec2& oringin = {}, const float& rad = 0);
//		void renderrectangle(const glm::vec4& rect, const glm::vec4 &color, const glm::vec2& oringin = {}, const float& rad = 0);
//		void rendercircle(const glm::vec3 &circle, const glm::vec4 &color, const texture& tex, uint32_t seg = 64u);
//		void rendercircle(const glm::vec3& circle, const glm::vec4& color, uint32_t seg = 64u);
//		void rendertext(const glm::vec2& pos, const char* text, const font& f, const glm::vec4& color, const float& size = 1.5, const float &spacing = 4, const float& line_spacing = 3, bool show_incenter = 1);
//	};
//#pragma endregion
//	namespace inter {
//		float pos2scr_coordsX(float x, float w);
//		float pos2scr_coordsy(float y, float h);
//		glm::vec2 pos2scr_coords(glm::vec2 v, glm::vec2 s);
//		uint32_t creatshader(const char* src, uint32_t type);
//		glm::vec2 retateoround(glm::vec2 v, glm::vec2 p, float rad);
//		stbtt_aligned_quad getfontglyphquad(const font f, const char c);
//		glm::vec2 gettextsize(const char* text, const font &f, const float &size = 1.5f, const float &spacing = 4, const float &line_space = 3);
//	}
//
//}

#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include <fstream>
#include <ostream>
#include <array>
#include <istream>
#include <string>
#include <vector>
#include "glm/detail/type_float.hpp"
#define _OPNEGL_SHADER_VERSION "#version 330"
#define _OPNEGL_SHADER_PRECISION "precision highp float;"
#define _OPNEGL_BUFFER_USAGE GL_STREAM_DRAW
#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consectetur adipiscing \nelit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n Ut enim ad minim veniam,\n quis nostrud exercitation ullamco laboris ni\nsi ut aliquip ex ea commodo consequat.\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
namespace vx {
	bool init();
	void cleanup();



#pragma region shaderes
	struct shader_prog
	{
		uint32_t id = 0;
		int sampler = 0;
		void bind() { glUseProgram(id); }
		void clean() { glDeleteProgram(id); }
	};
	bool velidateshader(uint32_t id);
	shader_prog creatshaderprogram(const char* vertex, const char* fragment);
	shader_prog creatshaderprogramfromfile(const char* vertex_file, const char* fragment_file);
#pragma endregion
#pragma region camera
	struct camera
	{
		glm::vec2 pos = {};
		glm::vec2 scale = { 1, 1 };
		float rad = 0;
	};
#pragma endregion

#pragma region texture
	struct texture
	{
		uint32_t id;
		int w, h, n;
		uint32_t loadfrompixledata(const unsigned char* data, glm::ivec3 dem, bool pixlated = true, bool use_mipmap = true);
		uint32_t loadfromfile(const char* filename, bool pixlated = true, bool use_mipmap = true);
		glm::vec4 pos2texcoords(glm::vec4 pos);
		void cleanup();
	};
#pragma endregion
#pragma region font
	struct font {
		texture text = {};
		glm::ivec2 size = {};
		stbtt_packedchar* pakedCHbuff = 0;
		uint32_t buff_size = 0;
		float max_hight = 0;

		void creatfromttf(const unsigned char* data, bool pixlated = true, bool usemip = false);
		void creatfromfile(const char* file, bool pixlated = true, bool usemip = false);

		void cleanup();
	};
#pragma endregion

#pragma region renderer

	class renderer {
	public:
		enum buff_t : uint32_t
		{
			vbo,
			tpbo,
			cbo,
			buff_c
		};
		struct shape_i {
			uint32_t ind4shape;
			uint32_t texture;
		};
		shader_prog current_sh = {};
		camera current_cam = {};

		std::vector<shape_i> shape_info;
		std::vector<glm::vec2> vertecies;
		std::vector<glm::vec2> texcoords;
		std::vector<uint32_t> colors;
		uint32_t buffs[buff_t::buff_c];
		uint32_t vao = 0;
		glm::vec2 scr_size = {};

		renderer() = default;
		void creat(uint32_t fbo = 0, size_t count = 1000);
		void cleanup();
		void reset();

		void enableGLfeatures();
		void cleardrawdata();
		void clear(glm::vec4 color = { 1, 1, 1, 1 });
		void flush(bool clear_draw_data = true);
		void flush1();
		void updatecontex(glm::vec2 s);

		void setshader(shader_prog sh);
		void renderrectangle2scr(const glm::vec4& rect, const glm::uvec4& color, const texture& tex, const glm::vec4& tex_coords = { 0, 0, 1, 1 }, const glm::vec2& oringin = {}, const float& rad = 0);
		void renderrectangle2scr(const glm::vec4& rect, const glm::uvec4& color, const glm::vec2& oringin = {}, const float& rad = 0);
		void renderrectangle(const glm::vec4& rect, const glm::uvec4& color, const texture& tex, const glm::vec4& tex_coords = { 0, 0, 1, 1 }, const glm::vec2& oringin = {}, const float& rad = 0);
		void renderrectangle(const glm::vec4& rect, const glm::uvec4& color, const glm::vec2& oringin = {}, const float& rad = 0);
		void rendercircle2scr(const glm::vec3& circle, const glm::uvec4& color, const texture& tex, uint32_t seg = 64u);
		void rendercircle2scr(const glm::vec3& circle, const glm::uvec4& color, uint32_t seg = 64u);
		void rendercircle(const glm::vec3& circle, const glm::uvec4& color, const texture& tex, uint32_t seg = 64u);
		void rendercircle(const glm::vec3& circle, const glm::uvec4& color, uint32_t seg = 64u);
		void rendertext2scr(const glm::vec2& pos, const char* text, const font& f, const glm::uvec4& color, const float& size = 1.5, const float& spacing = 4, const float& line_spacing = 3, bool show_incenter = 1);
		void rendertext(const glm::vec2& pos, const char* text, const font& f, const glm::uvec4& color, const float& size = 1.5, const float& spacing = 4, const float& line_spacing = 3, bool show_incenter = 1);
	};
#pragma endregion
	namespace inter {
		float pos2scr_coordsX(float x, float w);
		float pos2scr_coordsy(float y, float h);
		glm::vec2 pos2scr_coords(glm::vec2 v, glm::vec2 s);
		uint32_t creatshader(const char* src, uint32_t type);
		glm::vec2 scalearound(glm::vec2 v, glm::vec2 p, glm::vec2 s);
		glm::vec2 retateoround(glm::vec2 v, glm::vec2 p, float rad);
		stbtt_aligned_quad getfontglyphquad(const font f, const char c);
		glm::vec2 gettextsize(const char* text, const font& f, const float& size = 1.5f, const float& spacing = 4, const float& line_space = 3);
	}

}
/*
#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include <fstream>
#include <ostream>
#include <istream>
#include <vector>
#include "glm/detail/type_float.hpp"
#define _OPNEGL_SHADER_VERSION "#version 330"
#define _OPNEGL_SHADER_PRECISION "precision highp float;"
#define _OPNEGL_BUFFER_USAGE GL_STREAM_DRAW
#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consectetur adipiscing \nelit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n Ut enim ad minim veniam,\n quis nostrud exercitation ullamco laboris ni\nsi ut aliquip ex ea commodo consequat.\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
namespace libname {
	bool init();
	void cleanup();



#pragma region shaderes
	struct shader_prog
	{
		uint32_t id = 0;
		int sampler = 0;
		void bind() { glUseProgram(id); }
		void clean() { glDeleteProgram(id); }
	};
	bool velidateshader(uint32_t id);
	shader_prog creatshaderprogram(const char* vertex, const char* fragment);
	shader_prog creatshaderprogramfromfile(const char* vertex_file, const char* fragment_file);
#pragma endregion
#pragma region camera
	struct camera
	{
		glm::vec2 pos = {};
		glm::vec2 scale = { 1, 1 };
		float rad = 0;
	};
#pragma endregion

#pragma region texture
	struct texture
	{
		uint32_t id;
		int w, h, n;
		uint32_t loadfrompixledata(const unsigned char* data, glm::ivec3 dem, bool pixlated = true, bool use_mipmap = true);
		uint32_t loadfromfile(const char* filename, bool pixlated = true, bool use_mipmap = true);
		glm::vec4 pos2texcoords(glm::vec4 pos);
		void cleanup();
	};
#pragma endregion
#pragma region font
	struct font {
		texture text = {};
		glm::ivec2 size = {};
		stbtt_packedchar* pakedCHbuff = 0;
		uint32_t buff_size = 0;
		float max_hight = 0;

		void creatfromttf(const unsigned char* data, bool pixlated = true, bool usemip = false);
		void creatfromfile(const char* file, bool pixlated = true, bool usemip = false);

		void cleanup();
	};
#pragma endregion

#pragma region renderer

	class renderer {
	public:
		enum buff_t : uint32_t
		{
			vbo,
			tpbo,
			cbo,
			buff_c
		};
		struct shape_i {
			uint32_t ind4shape;
			uint32_t texture;
		};
		shader_prog current_sh = {};
		camera current_cam = {};

		std::vector<shape_i> shape_info;
		std::vector<glm::vec2> vertecies;
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec4> colors;
		uint32_t buffs[buff_t::buff_c];
		uint32_t vao = 0;
		glm::vec2 scr_size = {};

		renderer() = default;
		void creat(uint32_t fbo = 0, size_t count = 1000);
		void cleanup();
		void reset();

		void enableGLfeatures();
		void cleardrawdata();
		void clear(glm::vec4 color = { 1, 1, 1, 1 });
		void flush(bool clear_draw_data = true);
		void updatecontex(glm::vec2 s);

		void setshader(shader_prog sh);
		void renderrectangle2scr(const glm::vec4& rect, const glm::vec4& color, const texture& tex, const glm::vec4& tex_coords = { 0, 0, 1, 1 }, const glm::vec2& oringin = {}, const float& rad = 0);
		void renderrectangle2scr(const glm::vec4& rect, const glm::vec4& color, const glm::vec2& oringin = {}, const float& rad = 0);
		void renderrectangle(const glm::vec4& rect, const glm::vec4& color, const texture& tex, const glm::vec4& tex_coords = { 0, 0, 1, 1 }, const glm::vec2& oringin = {}, const float& rad = 0);
		void renderrectangle(const glm::vec4& rect, const glm::vec4& color, const glm::vec2& oringin = {}, const float& rad = 0);
		void rendercircle2scr(const glm::vec3& circle, const glm::vec4& color, const texture& tex, uint32_t seg = 64u);
		void rendercircle2scr(const glm::vec3& circle, const glm::vec4& color, uint32_t seg = 64u);
		void rendercircle(const glm::vec3& circle, const glm::vec4& color, const texture& tex, uint32_t seg = 64u);
		void rendercircle(const glm::vec3& circle, const glm::vec4& color, uint32_t seg = 64u);
		void rendertext2scr(const glm::vec2& pos, const char* text, const font& f, const glm::vec4& color, const float& size = 1.5, const float& spacing = 4, const float& line_spacing = 3, bool show_incenter = 1);
		void rendertext(const glm::vec2& pos, const char* text, const font& f, const glm::vec4& color, const float& size = 1.5, const float& spacing = 4, const float& line_spacing = 3, bool show_incenter = 1);
	};
#pragma endregion
	namespace inter {
		float pos2scr_coordsX(float x, float w);
		float pos2scr_coordsy(float y, float h);
		glm::vec2 pos2scr_coords(glm::vec2 v, glm::vec2 s);
		uint32_t creatshader(const char* src, uint32_t type);
		glm::vec2 scalearound(glm::vec2 v, glm::vec2 p, glm::vec2 s);
		glm::vec2 retateoround(glm::vec2 v, glm::vec2 p, float rad);
		stbtt_aligned_quad getfontglyphquad(const font f, const char c);
		glm::vec2 gettextsize(const char* text, const font& f, const float& size = 1.5f, const float& spacing = 4, const float& line_space = 3);
	}

}*/