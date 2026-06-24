//#include "lib.h"
//
//namespace libname {
//	namespace defaults {
//		static texture white1px;
//		static shader_prog default_sh = {};
//		static const char* vertex_src =
//			_OPNEGL_SHADER_VERSION "\n"
//			_OPNEGL_SHADER_PRECISION "\n"
//			"in vec2 apos;\n"
//			"in vec4 acol;\n"
//			"in vec2 acor;\n"
//			"out vec4 color;\n"
//			"out vec2 coord;\n"
//			"void main() {\n"
//			"	gl_Position = vec4(apos, 0, 1);\n"
//			"	color = acol;\n"
//			"	coord = acor;\n"
//			"}";
//		static const char* fragment_src =
//			_OPNEGL_SHADER_VERSION "\n"
//			_OPNEGL_SHADER_PRECISION "\n"
//			"out vec4 frag_color;\n"
//			"in vec4 color;"
//			"in vec2 coord;"
//			"uniform sampler2D sampler;"
//			"void main() {\n"
//			"	frag_color = color * texture2D(sampler, coord);"
//			//"	frag_color = color;"
//			"}";
//	}
//	namespace inter {
//		float pos2scr_coordsX(float x, float w)
//		{
//			return (x / w) * 2 - 1;
//		}
//		float pos2scr_coordsy(float y, float h)
//		{
//			return -((y / h) * 2 - 1);
//		}
//		glm::vec2 pos2scr_coords(glm::vec2 v, glm::vec2 s)
//		{
//			return { (v.x / s.x) * 2 - 1, -((v.y / s.y) * 2 - 1) };
//		}
//		uint32_t creatshader(const char* src, uint32_t type)
//		{
//			uint32_t id = glCreateShader(type);
//			glShaderSource(id, 1, &src, 0);
//			glCompileShader(id);
//			int res = 0;
//			glGetShaderiv(id, GL_COMPILE_STATUS, &res);
//			if (!res) {
//				char* message = 0;
//				int l = 0;
//				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);
//				message = new char[l];
//				glGetShaderInfoLog(id, l, &l, message);
//				message[l - 1] = 0;
//				std::cout << message << "\n";
//				delete[] message;
//			}
//			return id;
//		}
//		glm::vec2 retateoround(glm::vec2 v, glm::vec2 p, float rad)
//		{
//			//p.y = -p.y;
//			float s = glm::sin(rad);
//			float c = glm::cos(rad);
//			glm::vec2 vec = v - p;
//			//glm::vec2 veds = { vec.x * c - vec.y * s, vec.x * s + vec.y * c };
//			return glm::vec2(vec.x * c - vec.y * s, vec.x * s + vec.y * c) + p;
//		}
//		stbtt_aligned_quad getfontglyphquad(const font f, const char c)
//		{
//			stbtt_aligned_quad quad = { 0 };
//
//			float x = 0;
//			float y = 0;
//
//			stbtt_GetPackedQuad(f.pakedCHbuff,
//				f.size.x, f.size.y, c - ' ', &x, &y, &quad, 1);
//
//
//			return quad;
//		}
//
//		glm::vec2 gettextsize(const char* text, const font& f, const float& size, const float& spacing, const float& line_space)
//		{
//			return glm::vec2();
//		}
//
//	}
//
//	bool velidateshader(uint32_t id) {
//		int res = 0;
//		glGetProgramiv(id, GL_LINK_STATUS, &res);
//		if (res != GL_TRUE) {
//			char* message = 0;
//			int l = 0;
//			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);
//			message = new char[l];
//			glGetProgramInfoLog(id, l, &l, message);
//			std::cout << message << "\n";
//			delete[] message;
//		}
//		glValidateProgram(id);
//		return 0;
//	}
//
//	shader_prog creatshaderprogram(const char* vertex, const char* fragment) {
//		shader_prog shader;
//		auto vert = inter::creatshader(vertex, GL_VERTEX_SHADER);
//		auto frag = inter::creatshader(fragment, GL_FRAGMENT_SHADER);
//		shader.id = glCreateProgram();
//		glAttachShader(shader.id, vert);
//		glAttachShader(shader.id, frag);
//		glBindAttribLocation(shader.id, 0, "apos");
//		glBindAttribLocation(shader.id, 1, "acol");
//		glBindAttribLocation(shader.id, 2, "acor");
//		glLinkProgram(shader.id);
//		glDeleteShader(vert);
//		glDeleteShader(frag);
//		velidateshader(shader.id);
//		shader.sampler = glGetUniformLocation(shader.id, "sampler");
//		return shader;
//	}
//
//	bool init()
//	{
//		stbi_set_flip_vertically_on_load(1);
//
//		defaults::default_sh = creatshaderprogram(defaults::vertex_src, defaults::fragment_src);
//		const unsigned char buff[] = {
//			0xff,
//			0xff,
//			0xff,
//			0xff
//		};
//		defaults::white1px.loadfrompixledata(buff, { 1, 1, 4 });
//		return true;
//	}
//
//	void cleanup()
//	{
//		defaults::default_sh.clean();
//	}
//#pragma region renderer
//
//
//	void renderer::creat(uint32_t fbo, size_t count)
//	{
//		enableGLfeatures();
//
//		vertecies.reserve(count);
//		colors.reserve(count);
//		shape_info.reserve(count);
//		texcoords.reserve(count);
//		indecies.reserve(count);
//		glGenVertexArrays(1, &vao);
//		glBindVertexArray(vao);
//		glGenBuffers(buff_t::buff_c, buffs);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[buff_t::ebo]);
//
//		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, (void*)0);
//
//
//		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, (void*)0);
//
//		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, (void*)0);
//
//		reset();
//	}
//
//	void renderer::cleanup()
//	{
//		glDeleteBuffers(buff_t::buff_c, buffs);
//		glDeleteVertexArrays(1, &vao);
//	}
//
//	void renderer::reset()
//	{
//		current_sh = defaults::default_sh;
//	}
//
//	void renderer::enableGLfeatures()
//	{
//		glEnable(GL_CULL_FACE);
//		glFrontFace(GL_CW);
//		glCullFace(GL_BACK);
//		glEnable(GL_BLEND);
//		glDisable(GL_DEPTH_TEST);
//		glBlendEquation(GL_FUNC_ADD);
//		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//	}
//
//	void renderer::cleardrawdata()
//	{
//		vertecies.clear();
//		colors.clear();
//		shape_info.clear();
//		texcoords.clear();
//		indecies.clear();
//	}
//
//	void renderer::clear(glm::vec4 color)
//	{
//		glClearColor(color.x, color.y, color.z, color.w);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//	}
//
//	void renderer::flush(bool clear_draw_data)
//	{
//		glBindVertexArray(vao);
//		current_sh.bind();
//		float ct = glfwGetTime();
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[buff_t::ebo]);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(uint32_t), indecies.data(), _OPNEGL_BUFFER_USAGE);
//		std::cout << "2nd in ms: " << (glfwGetTime() - ct) * 1000 << "\n";
//
//		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
//		glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);
//
//		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
//		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), _OPNEGL_BUFFER_USAGE);
//
//		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
//		glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), _OPNEGL_BUFFER_USAGE);
//		{
//			uint32_t pos = 0;
//			uint32_t how_much = 0;
//			uint32_t id = shape_info[0].texture;
//			glBindTexture(GL_TEXTURE_2D, id);
//			for (int i = 0; i < shape_info.size(); i++) {
//				how_much += shape_info[i].ind4shape;
//				if ((shape_info[i].texture != id) || (i == (shape_info.size() - 1))) {
//					glDrawElements(GL_TRIANGLES, how_much, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * pos));
//					pos += how_much;
//					how_much = 0;
//					id = shape_info[i].texture;
//					glBindTexture(GL_TEXTURE_2D, id);
//				}
//			}
//			if(how_much)
//				glDrawElements(GL_TRIANGLES, how_much, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * (pos)));
//
//			glBindVertexArray(0);
//		}
//		if (clear_draw_data) {
//			cleardrawdata();
//		}
//	}
//
//	//void renderer::flush(bool clear_draw_data)
//	//{
//	//	enableGLfeatures();
//	//	glBindVertexArray(vao);
//	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[buff_t::ebo]);
//	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(uint32_t), indecies.data(), _OPNEGL_BUFFER_USAGE);
//
//	//	current_sh.bind();
//	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
//	//	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);
//
//	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
//	//	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), _OPNEGL_BUFFER_USAGE);
//
//	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
//	//	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), _OPNEGL_BUFFER_USAGE);
//	//	{
//	//		uint32_t pos = 0;
//	//		uint32_t id = shape_info[0].texture;
//	//		glBindTexture(GL_TEXTURE_2D, id);
//	//		for (int i = 0; i < shape_info.size(); i++) {
//	//			if (shape_info[i].texture != id) {
//	//				glDrawElements(GL_TRIANGLES, 6 * (i - pos), GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * (pos * 6)));
//	//				pos = i;
//	//				id = shape_info[i].texture;
//	//				glBindTexture(GL_TEXTURE_2D, id);
//	//			}
//	//		}
//	//		glDrawElements(GL_TRIANGLES, 6 * (shape_info.size() - pos), GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * (pos * 6)));
//
//	//		glBindVertexArray(0);
//	//	}
//	//	if (clear_draw_data) {
//	//		cleardrawdata();
//	//	}
//	//}
//
//	//void renderer::flush(bool clear_draw_data)
//	//{
//	//	enableGLfeatures();
//	//	glBindVertexArray(vao);
//	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[buff_t::ebo]);
//	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(uint32_t), indecies.data(), _OPNEGL_BUFFER_USAGE);
//
//	//	current_sh.bind();
//	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
//	//	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);
//
//	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
//	//	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), _OPNEGL_BUFFER_USAGE);
//	//	{
//	//		uint32_t pos = 0;
//	//		//uint32_t id = 
//	//		for (int i = 0; i < 1; i++) {
//	//			glDrawElements(GL_TRIANGLES, indecies.size(), GL_UNSIGNED_INT, 0);
//	//		}
//	//	}
//	//	glBindVertexArray(0);
//	//	if (clear_draw_data) {
//	//		cleardrawdata();
//	//	}
//	//}
//
//	void renderer::updatecontex(glm::vec2 s)
//	{
//		if ((scr_size.x != s.x) || (scr_size.y != s.y)) {
//			scr_size = s;
//			glViewport(0, 0, scr_size.x, scr_size.y);
//		}
//	}
//
//	void renderer::setshader(shader_prog sh)
//	{
//		current_sh = sh;
//	}
//
//	void renderer::renderrectangle(const glm::vec4& rect, const glm::vec4& color, const texture& tex, const glm::vec4& tex_coords, const glm::vec2& oringin, const float& rad)
//	{
//		uint32_t s = vertecies.size();
//		glm::vec2 originv = glm::vec2(rect.x, rect.y) + oringin;
//		if (rad)
//		{
//			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x , rect.y }, originv, rad), scr_size));
//			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x + rect.z , rect.y }, originv, rad), scr_size));
//			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x + rect.z , rect.y + rect.w }, originv, rad), scr_size));
//			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x , rect.y + rect.w }, originv, rad), scr_size));
//		}
//		else {
//			vertecies.push_back(inter::pos2scr_coords({ rect.x , rect.y }, scr_size));
//			vertecies.push_back(inter::pos2scr_coords({ rect.x + rect.z , rect.y }, scr_size));
//			vertecies.push_back(inter::pos2scr_coords({ rect.x + rect.z , rect.y + rect.w }, scr_size));
//			vertecies.push_back(inter::pos2scr_coords({ rect.x , rect.y + rect.w }, scr_size));
//		}
//		//texcoords.push_back({ (tex_coords.x / tex.w), 1 - (tex_coords.y / tex.h) });
//		//texcoords.push_back({ ((tex_coords.x + tex_coords.z) / tex.w),  1 - (tex_coords.y / tex.h) });
//		//texcoords.push_back({ ((tex_coords.x + tex_coords.z) / tex.w), 1 - ((tex_coords.y + tex_coords.w) / tex.h) });
//		//texcoords.push_back({ (tex_coords.x / tex.w),  1 - ((tex_coords.y + tex_coords.w) / tex.h) });
//		texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
//		texcoords.push_back({ ((tex_coords.x + tex_coords.z)),  1 - (tex_coords.y) });
//		texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });
//		texcoords.push_back({ (tex_coords.x),  1 - ((tex_coords.y + tex_coords.w)) });
//		colors.push_back(color);
//		colors.push_back(color);
//		colors.push_back(color);
//		colors.push_back(color);
//		shape_info.push_back({ 6, tex.id });
//		indecies.push_back(0 + s); indecies.push_back(1 + s); indecies.push_back(2 + s);
//		indecies.push_back(3 + s); indecies.push_back(0 + s); indecies.push_back(2 + s);
//
//	}
//	void renderer::renderrectangle(const glm::vec4& rect, const glm::vec4& color, const glm::vec2& oringin, const float& rad)
//	{
//		renderrectangle(rect, color, defaults::white1px, { 0, 0, 1, 1 }, oringin, rad);
//	}
//	void renderer::rendercircle(const glm::vec3& circle, const glm::vec4& color, const texture& tex, uint32_t seg)
//	{
//		uint32_t num = 0;
//		uint32_t s = vertecies.size();
//		vertecies.push_back(inter::pos2scr_coords({ circle.x, circle.y }, scr_size));
//		colors.push_back(color);
//		texcoords.push_back({ 0, 0 });
//		//num++;
//		for (int i = 0; i < seg; i++) {
//			vertecies.push_back(inter::pos2scr_coords({ glm::sin((i * 6.283184f) / seg) * circle.z + circle.x, glm::cos((i * 6.283184f) / seg) * circle.z + circle.y }, scr_size));
//			texcoords.push_back({ 0, 0 });
//			colors.push_back(color);
//		}
//		//shapes.push_back(shape::circle);
//		for (int i = 0; i < seg - 1; i++) {
//			indecies.push_back((i + 2) + s);
//			indecies.push_back((i + 1) + s);
//			indecies.push_back(s);
//			num += 3;
//		}
//		indecies.push_back(1 + s);
//		indecies.push_back(seg + s);
//		indecies.push_back(s);
//		num += 3;
//		shape_info.push_back({ num, tex.id });
//	}
//	void renderer::rendercircle(const glm::vec3& circle, const glm::vec4& color, uint32_t seg)
//	{
//		rendercircle(circle, color, defaults::white1px, seg);
//	}
//	void renderer::rendertext(const glm::vec2& pos, const char* text, const font& f, const glm::vec4& color, const float& size, const float& spacing, const float& line_spacing, bool show_incenter)
//	{
//		const uint32_t length = strlen(text);
//		glm::vec4 rect;
//		rect.x = pos.x;
//		float line_pos = pos.y;
//		for (uint32_t i = 0; i < length; i++) {
//			if (text[i] == '\n')
//			{
//				rect.x = pos.x;
//				line_pos += (f.max_hight + line_spacing) * size;
//			}
//			else if (text[i] == '\t')
//			{
//				const stbtt_aligned_quad quad = inter::getfontglyphquad(f, '_');
//				auto x = quad.x1 - quad.x0;
//
//				rect.x += x * size * 3 + spacing * size;
//			}
//			else if (text[i] == ' ')
//			{
//				const stbtt_aligned_quad quad = inter::getfontglyphquad(f, '_');
//				auto x = quad.x1 - quad.x0;
//				rect.x += x * size + spacing * size;
//			}
//			else if (text[i] >= ' ' && text[i] <= '~') {
//				const stbtt_aligned_quad quad = inter::getfontglyphquad(f, text[i]);
//				rect.z = size * (quad.x1 - quad.x0);
//				rect.w = size * (quad.y1 - quad.y0);
//				rect.y = line_pos + quad.y0 * size;
//				//std::cout << quad.s0 << "	" << quad.t0 << "	" << quad.s1 - quad.s0 << "	" << quad.t1 - quad.t0 << "\n";
//				renderrectangle({ rect.x + pos.x, rect.y + pos.y , rect.z, rect.w }, color, f.text, {quad.s0, quad.t0, quad.s1 - quad.s0, quad.t1 - quad.t0 });
//				rect.x += rect.z + spacing * size;
//			}
//		}
//	}
//#pragma endregion
//
//
//	uint32_t texture::loadfrompixledata(const unsigned char* data, glm::ivec3 dem, bool pixlated, bool use_mipmap)
//	{
//		if (data) {
//			glActiveTexture(GL_TEXTURE0);
//			glGenTextures(1, &id);
//			glBindTexture(GL_TEXTURE_2D, id);
//			//GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER      GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			if (pixlated) {
//				if (use_mipmap)
//				{
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
//				}
//				else
//				{
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//				}
//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//			}
//			else
//			{
//				if (use_mipmap)
//				{
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//				}
//				else
//				{
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//				}
//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			}
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dem.x, dem.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//			glGenerateMipmap(GL_TEXTURE_2D);
//		}
//		return id;
//	}
//	uint32_t texture::loadfromfile(const char* filename, bool pixlated, bool use_mipmap)
//	{
//		unsigned char* data = stbi_load(filename, &w, &h, &n, 4);
//		loadfrompixledata(data, { w, h, n }, pixlated, use_mipmap);
//		stbi_image_free(data);
//		return id;
//	}
//	glm::vec4 texture::pos2texcoords(glm::vec4 pos)
//	{
//		//texcoords.push_back({ (tex_coords.x / tex.w), 1 - (tex_coords.y / tex.h) });
//		//texcoords.push_back({ ((tex_coords.x + tex_coords.z) / tex.w),  1 - (tex_coords.y / tex.h) });
//		//texcoords.push_back({ ((tex_coords.x + tex_coords.z) / tex.w), 1 - ((tex_coords.y + tex_coords.w) / tex.h) });
//		//texcoords.push_back({ (tex_coords.x / tex.w),  1 - ((tex_coords.y + tex_coords.w) / tex.h) });
//		return glm::vec4((pos.x / w), (pos.y / h), ((pos.z - pos.x) / w), ((pos.w - pos.y) / h));
//	}
//	void texture::cleanup()
//	{
//		glDeleteTextures(1, &id);
//		*this = {};
//	}
//	void font::creatfromttf(const unsigned char* data, bool pixlated, bool usemip)
//	{
//		size.x = 2000,
//		size.y = 2000,
//		max_hight = 0,
//		buff_size = ('~' - ' ');
//		const uint32_t fontMonochromeBufferSize = size.x * size.y;
//		const uint32_t fontRgbaBufferSize = size.x * size.y * 4;
//		unsigned char* monochrome = new unsigned char[fontMonochromeBufferSize];
//		unsigned char* rgba = new unsigned char[fontRgbaBufferSize];
//
//		pakedCHbuff = new stbtt_packedchar[buff_size]{};
//
//		stbtt_pack_context stbtt_context;
//		stbtt_PackBegin(&stbtt_context, monochrome, size.x, size.y, 0, 2, NULL);
//		stbtt_PackSetOversampling(&stbtt_context, 2, 2);
//		stbtt_PackFontRange(&stbtt_context, data, 0, 65, ' ', '~' - ' ', pakedCHbuff);
//		stbtt_PackEnd(&stbtt_context);
//
//		//for (uint32_t i = 0; i < fontMonochromeBufferSize; i++) {
//		//	rgba[(i * 4)] = monochrome[i];
//		//	rgba[(i * 4) + 1] = monochrome[i];
//		//	rgba[(i * 4) + 2] = monochrome[i];
//
//		//	if (monochrome[i] > 1)
//		//	{
//		//		rgba[(i * 4) + 3] = 255;
//		//	}
//		//	else
//		//	{
//		//		rgba[(i * 4) + 3] = 0;
//		//	}
//		//}
//		for (uint32_t y = 0; y < size.y; ++y) {
//			for (uint32_t x = 0; x < size.x; ++x) {
//				uint32_t i = x + y * size.x;
//				uint32_t flipped_y = size.y - 1 - y;
//				uint32_t flipped_i = x + flipped_y * size.x;
//
//				rgba[(flipped_i * 4)] = monochrome[i];
//				rgba[(flipped_i * 4) + 1] = monochrome[i];
//				rgba[(flipped_i * 4) + 2] = monochrome[i];
//				rgba[(flipped_i * 4) + 3] = monochrome[i];
//
//				//if (monochrome[i] > 1) {
//				//	rgba[(flipped_i * 4) + 3] = 255;
//				//}
//				//else {
//				//	rgba[(flipped_i * 4) + 3] = 0;
//				//}
//			}
//		}
//		delete[] monochrome;
//		text.loadfrompixledata(rgba, { size.x, size.y, 4 }, pixlated, usemip);
//		delete[] rgba;
//		for (char c = ' '; c <= '~'; c++)
//		{
//			const stbtt_aligned_quad  q = inter::getfontglyphquad(*this, c);
//			const float               m = q.y1 - q.y0;
//
//			if (m > max_hight && m < 1.e+8f)
//			{
//				max_hight = m;
//			}
//		}
//	}
//	void font::creatfromfile(const char* file, bool pixlated, bool usemip)
//	{
//		std::ifstream fileFont(file, std::ios::binary);
//
//		if (!fileFont.is_open())
//		{
//			std::cout << "font not loading\n";
//			return;
//		}
//
//		int fileSize = 0;
//		fileFont.seekg(0, std::ios::end);
//		fileSize = (int)fileFont.tellg();
//		fileFont.seekg(0, std::ios::beg);
//		unsigned char* fileData = new unsigned char[fileSize];
//		fileFont.read((char*)fileData, fileSize);
//		fileFont.close();
//
//		creatfromttf(fileData, pixlated, usemip);
//
//		delete[] fileData;
//	}
//	void font::cleanup()
//	{
//		text.cleanup();
//		*this = {};
//	}
//}
//

#include "..\..\includes\engine\vortex.h"
#include <sstream>
namespace vx {
	namespace defaults {
		static texture white1px;
		static shader_prog default_sh = {};
		static const char* vertex_src =
			_OPNEGL_SHADER_VERSION "\n"
			_OPNEGL_SHADER_PRECISION "\n"
			"in vec2 apos;\n"
			"in uint acol;\n"
			"in vec2 acor;\n"
			"out vec4 color;\n"
			"out vec2 coord;\n"
			"vec4 upackcolor(uint _color) {"
			"float r = ((_color >> 24u) & 0xFFu) / 255.0f;"
			"float g = ((_color >> 16u) & 0xFFu) / 255.0f;"
			"float b = ((_color >> 8u) & 0xFFu) / 255.0f;"
			"float a = (_color & 0xFFu) / 255.0f;"
			"return vec4(r, g, b, a);"
			"}"
			"void main() {\n"
			"	gl_Position = vec4(apos, 0, 1);\n"
			"	color = upackcolor(acol);\n"
			"	coord = acor;\n"
			"}";
		static const char* fragment_src =
			_OPNEGL_SHADER_VERSION "\n"
			_OPNEGL_SHADER_PRECISION "\n"
			"out vec4 frag_color;\n"
			"in vec4 color;"
			"in vec2 coord;"
			"uniform sampler2D sampler;"
			"void main() {\n"
			"	frag_color = color * texture2D(sampler, coord);"
			//"	frag_color = color;"
			"}";
	}
	namespace inter {
		float pos2scr_coordsX(float x, float w)
		{
			return (x / w) * 2 - 1;
		}
		float pos2scr_coordsy(float y, float h)
		{
			return -((y / h) * 2 - 1);
		}
		glm::vec2 pos2scr_coords(glm::vec2 v, glm::vec2 s)
		{
			return { (v.x / s.x) * 2 - 1, -((v.y / s.y) * 2 - 1) };
		}
		uint32_t creatshader(const char* src, uint32_t type)
		{
			uint32_t id = glCreateShader(type);
			glShaderSource(id, 1, &src, 0);
			glCompileShader(id);
			int res = 0;
			glGetShaderiv(id, GL_COMPILE_STATUS, &res);
			if (!res) {
				char* message = 0;
				int l = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);
				message = new char[l];
				glGetShaderInfoLog(id, l, &l, message);
				message[l - 1] = 0;
				std::cout << message << "\n";
				delete[] message;
			}
			return id;
		}
		glm::vec2 scalearound(glm::vec2 v, glm::vec2 p, glm::vec2 s)
		{
			return ((v - p) * s + p);
		}
		glm::vec2 retateoround(glm::vec2 v, glm::vec2 p, float rad)
		{
			//p.y = -p.y;
			float s = glm::sin(rad);
			float c = glm::cos(rad);
			glm::vec2 vec = v - p;
			//glm::vec2 veds = { vec.x * c - vec.y * s, vec.x * s + vec.y * c };
			return glm::vec2(vec.x * c - vec.y * s, vec.x * s + vec.y * c) + p;
		}
		stbtt_aligned_quad getfontglyphquad(const font f, const char c)
		{
			stbtt_aligned_quad quad = { 0 };

			float x = 0;
			float y = 0;

			stbtt_GetPackedQuad(f.pakedCHbuff,
				f.size.x, f.size.y, c - ' ', &x, &y, &quad, 1);


			return quad;
		}

		glm::vec2 gettextsize(const char* text, const font& f, const float& size, const float& spacing, const float& line_space)
		{
			stbtt_aligned_quad quad;
			glm::vec2 position = {};

			const int text_length = (int)strlen(text);
			glm::vec4 rectangle = {};
			rectangle.x = position.x;
			float linePositionY = position.y;

			//This is the y position we render at because it advances when we encounter newlines
			float maxPos = 0;
			float maxPosY = 0;
			float bonusY = 0;

			for (int i = 0; i < text_length; i++)
			{
				if (text[i] == '\n')
				{
					rectangle.x = position.x;
					linePositionY += (f.max_hight + line_space) * size;
					bonusY += (f.max_hight + line_space) * size;
					maxPosY = 0;
				}
				else if (text[i] == '\t')
				{
					quad = inter::getfontglyphquad(f, '_');
					auto x = quad.x1 - quad.x0;

					rectangle.x += x * size * 3 + spacing * size;
				}
				else if (text[i] == ' ')
				{
					quad = inter::getfontglyphquad(f, '_');
					auto x = quad.x1 - quad.x0;

					rectangle.x += x * size + spacing * size;
				}
				else if (text[i] >= ' ' && text[i] <= '~')
				{
					quad = inter::getfontglyphquad(f, text[i]);

					rectangle.z = quad.x1 - quad.x0;
					rectangle.w = quad.y1 - quad.y0;

					rectangle.z *= size;
					rectangle.w *= size;

					rectangle.y = linePositionY + quad.y0 * size;

					rectangle.x += rectangle.z + spacing * size;

					maxPosY = std::max(maxPosY, rectangle.y);
					maxPos = std::max(maxPos, rectangle.x);
				}
			}

			maxPos = std::max(maxPos, rectangle.x);
			maxPosY = std::max(maxPosY, rectangle.y);

			float paddX = maxPos;

			float paddY = maxPosY;

			paddY += f.max_hight * size + bonusY;

			return glm::vec2{ paddX, paddY };

		}

	}

	bool velidateshader(uint32_t id) {
		int res = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &res);
		if (res != GL_TRUE) {
			char* message = 0;
			int l = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);
			message = new char[l];
			glGetProgramInfoLog(id, l, &l, message);
			std::cout << message << "\n";
			delete[] message;
		}
		glValidateProgram(id);
		return 0;
	}

	shader_prog creatshaderprogram(const char* vertex, const char* fragment) {
		shader_prog shader;
		auto vert = inter::creatshader(vertex, GL_VERTEX_SHADER);
		auto frag = inter::creatshader(fragment, GL_FRAGMENT_SHADER);
		shader.id = glCreateProgram();
		glAttachShader(shader.id, vert);
		glAttachShader(shader.id, frag);
		glBindAttribLocation(shader.id, 0, "apos");
		glBindAttribLocation(shader.id, 1, "acol");
		glBindAttribLocation(shader.id, 2, "acor");
		glLinkProgram(shader.id);
		glDeleteShader(vert);
		glDeleteShader(frag);
		velidateshader(shader.id);
		shader.sampler = glGetUniformLocation(shader.id, "sampler");
		return shader;
	}

	shader_prog creatshaderprogramfromfile(const char* vertex_file, const char* fragment_file)
	{
		std::string  vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertex_file);
			fShaderFile.open(fragment_file);
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

		return creatshaderprogram(vertexCode.c_str(), fragmentCode.c_str());
	}

	bool init()
	{
		stbi_set_flip_vertically_on_load(1);

		defaults::default_sh = creatshaderprogram(defaults::vertex_src, defaults::fragment_src);
		const unsigned char buff[] = {
			0xff,
			0xff,
			0xff,
			0xff
		};
		defaults::white1px.loadfrompixledata(buff, { 1, 1, 4 });
		return true;
	}

	void cleanup()
	{
		defaults::default_sh.clean();
		defaults::white1px.cleanup();
	}
#pragma region renderer


	void renderer::creat(uint32_t fbo, size_t count)
	{
		enableGLfeatures();

		//vertecies.reserve(count);
		//colors.reserve(count);
		//shape_info.reserve(count);
		//texcoords.reserve(count);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(buff_t::buff_c, buffs);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, 0, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, (void*)0);

		reset();
	}

	void renderer::cleanup()
	{
		glDeleteBuffers(buff_t::buff_c, buffs);
		glDeleteVertexArrays(1, &vao);
	}

	void renderer::reset()
	{
		current_sh = defaults::default_sh;
	}

	void renderer::enableGLfeatures()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	void renderer::cleardrawdata()
	{
		vertecies.clear();
		colors.clear();
		shape_info.clear();
		texcoords.clear();
	}

	void renderer::clear(glm::vec4 color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void renderer::flush(bool clear_draw_data)
	{
		if (!shape_info.size())
			return;
		glBindVertexArray(vao);
		current_sh.bind();
		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
		glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(uint32_t), colors.data(), _OPNEGL_BUFFER_USAGE);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
		glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), _OPNEGL_BUFFER_USAGE);
		//float ct = glfwGetTime();
		{
			uint32_t pos = 0;
			uint32_t how_much = 0;
			uint32_t id = shape_info[0].texture;
			glBindTexture(GL_TEXTURE_2D, id);
			for (int i = 0; i < shape_info.size(); i++) {
				if ((shape_info[i].texture != id)) {
					glDrawArrays(GL_TRIANGLES, pos, how_much);
					pos += how_much;
					how_much = 0;
					id = shape_info[i].texture;
					glBindTexture(GL_TEXTURE_2D, id);
				}
				how_much += shape_info[i].ind4shape;
			}
			if (how_much)
				glDrawArrays(GL_TRIANGLES, pos, how_much);

			glBindVertexArray(0);
		}
		//std::cout << "2nd in ms: " << (glfwGetTime() - ct) * 1000 << "\n";
		if (clear_draw_data) {
			cleardrawdata();
		}
	}
	void renderer::flush1(){
		glBindVertexArray(vao);
		current_sh.bind();
		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
		glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(uint32_t), colors.data(), _OPNEGL_BUFFER_USAGE);

		glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
		glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), _OPNEGL_BUFFER_USAGE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//std::cout << "2nd in ms: " << (glfwGetTime() - ct) * 1000 << "\n";
		cleardrawdata();
	}

	//void renderer::flush(int verride) {
	//	glBindVertexArray(vao);
	//	current_sh.bind();
	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
	//	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);

	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
	//	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(uint32_t), colors.data(), _OPNEGL_BUFFER_USAGE);

	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
	//	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), _OPNEGL_BUFFER_USAGE);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glBindVertexArray(0);
	//	//std::cout << "2nd in ms: " << (glfwGetTime() - ct) * 1000 << "\n";
	//	cleardrawdata();
	//}

	//void renderer::flush(bool clear_draw_data)
	//{
	//	enableGLfeatures();
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[buff_t::ebo]);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(uint32_t), indecies.data(), _OPNEGL_BUFFER_USAGE);

	//	current_sh.bind();
	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
	//	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);

	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
	//	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), _OPNEGL_BUFFER_USAGE);

	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::tpbo]);
	//	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), _OPNEGL_BUFFER_USAGE);
	//	{
	//		uint32_t pos = 0;
	//		uint32_t id = shape_info[0].texture;
	//		glBindTexture(GL_TEXTURE_2D, id);
	//		for (int i = 0; i < shape_info.size(); i++) {
	//			if (shape_info[i].texture != id) {
	//				glDrawElements(GL_TRIANGLES, 6 * (i - pos), GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * (pos * 6)));
	//				pos = i;
	//				id = shape_info[i].texture;
	//				glBindTexture(GL_TEXTURE_2D, id);
	//			}
	//		}
	//		glDrawElements(GL_TRIANGLES, 6 * (shape_info.size() - pos), GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * (pos * 6)));

	//		glBindVertexArray(0);
	//	}
	//	if (clear_draw_data) {
	//		cleardrawdata();
	//	}
	//}

	//void renderer::flush(bool clear_draw_data)
	//{
	//	enableGLfeatures();
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[buff_t::ebo]);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(uint32_t), indecies.data(), _OPNEGL_BUFFER_USAGE);

	//	current_sh.bind();
	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::vbo]);
	//	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec2), vertecies.data(), _OPNEGL_BUFFER_USAGE);

	//	glBindBuffer(GL_ARRAY_BUFFER, buffs[buff_t::cbo]);
	//	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), _OPNEGL_BUFFER_USAGE);
	//	{
	//		uint32_t pos = 0;
	//		//uint32_t id = 
	//		for (int i = 0; i < 1; i++) {
	//			glDrawElements(GL_TRIANGLES, indecies.size(), GL_UNSIGNED_INT, 0);
	//		}
	//	}
	//	glBindVertexArray(0);
	//	if (clear_draw_data) {
	//		cleardrawdata();
	//	}
	//}

	void renderer::updatecontex(glm::vec2 s)
	{
		if ((scr_size.x != s.x) || (scr_size.y != s.y)) {
			scr_size = s;
			glViewport(0, 0, scr_size.x, scr_size.y);
		}
	}

	void renderer::setshader(shader_prog sh)
	{
		current_sh = sh;
	}

	void renderer::renderrectangle2scr(const glm::vec4& rect, const glm::uvec4& color, const texture& tex, const glm::vec4& tex_coords, const glm::vec2& oringin, const float& rad)
	{
		glm::vec2 originv = glm::vec2(rect.x, rect.y) + oringin;
		glm::vec2 v0 = { rect.x , rect.y }; //top-left
		glm::vec2 v1 = { rect.x + rect.z , rect.y }; //top-right
		glm::vec2 v2 = { rect.x + rect.z , rect.y + rect.w }; //bottom-right
		glm::vec2 v3 = { rect.x , rect.y + rect.w };//bottom-left

		if (rad)
		{
			v0 = inter::retateoround(v0, originv, rad);
			v1 = inter::retateoround(v1, originv, rad);
			v2 = inter::retateoround(v2, originv, rad);
			v3 = inter::retateoround(v3, originv, rad);
		}

		v0 = inter::pos2scr_coords(v0, scr_size);
		v1 = inter::pos2scr_coords(v1, scr_size);
		v2 = inter::pos2scr_coords(v2, scr_size);
		v3 = inter::pos2scr_coords(v3, scr_size);

		std::array<glm::vec2, 6> a0 = { v0, v1, v2, v3, v0, v2 };
		std::array<glm::vec2, 6> a1 = { glm::vec2{ (tex_coords.x), 1.f - (tex_coords.y) }, glm::vec2{ ((tex_coords.x + tex_coords.z)),  1 - (tex_coords.y) }, glm::vec2{ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) }, { (tex_coords.x),  1 - ((tex_coords.y + tex_coords.w)) }, { (tex_coords.x), 1 - (tex_coords.y) }, glm::vec2{ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) } };
		vertecies.insert(vertecies.end(), a0.begin(), a0.end());
		texcoords.insert(texcoords.end(), a1.begin(), a1.end());
		//vertecies.push_back(v0);
		//vertecies.push_back(v1);
		//vertecies.push_back(v2);

		//vertecies.push_back(v3);
		//vertecies.push_back(v0);
		//vertecies.push_back(v2);

		//texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z)),  1 - (tex_coords.y) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });

		//texcoords.push_back({ (tex_coords.x),  1 - ((tex_coords.y + tex_coords.w)) });
		//texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });

		uint32_t _color;
		_color = (color.x << 24) | (color.y << 16) | (color.z << 8) | color.w;

		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);
		shape_info.push_back({ 6, tex.id });

		//vertecies.push_back(v0);
		//vertecies.push_back(v1);
		//vertecies.push_back(v2);

		//vertecies.push_back(v3);
		//vertecies.push_back(v0);
		//vertecies.push_back(v2);

		//texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z)),  1 - (tex_coords.y) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });

		//texcoords.push_back({ (tex_coords.x),  1 - ((tex_coords.y + tex_coords.w)) });
		//texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });
		//uint32_t _color = 0;
		//_color = (color.x << 24) | (color.y << 16) | (color.z << 8) | color.w;
		//colors.push_back(_color);
		//colors.push_back(_color);
		//colors.push_back(_color);
		//colors.push_back(_color);
		//colors.push_back(_color);
		//colors.push_back(_color);

		shape_info.push_back({ 6, tex.id });
	}

	void renderer::renderrectangle2scr(const glm::vec4& rect, const glm::uvec4& color, const glm::vec2& oringin, const float& rad)
	{
		renderrectangle2scr(rect, color, defaults::white1px, { 0, 0, 1, 1 }, oringin, rad);
	}

	void renderer::renderrectangle(const glm::vec4& rect, const glm::uvec4& color, const texture& tex, const glm::vec4& tex_coords, const glm::vec2& oringin, const float& rad)
	{
		//uint32_t s = vertecies.size();
		glm::vec2 cam_center = scr_size / 2.f;
		glm::vec2 originv = glm::vec2(rect.x, rect.y) + oringin;
		glm::vec2 v0 = { rect.x , rect.y }; //top-left
		glm::vec2 v1 = { rect.x + rect.z , rect.y }; //top-right
		glm::vec2 v2 = { rect.x + rect.z , rect.y + rect.w }; //bottom-right
		glm::vec2 v3 = { rect.x , rect.y + rect.w };//bottom-left
		/*if (rad)
		{
			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x , rect.y }, originv, rad), scr_size));
			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x + rect.z , rect.y }, originv, rad), scr_size));
			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x + rect.z , rect.y + rect.w }, originv, rad), scr_size));

			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x , rect.y + rect.w }, originv, rad), scr_size));
			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x , rect.y }, originv, rad), scr_size));
			vertecies.push_back(inter::pos2scr_coords(inter::retateoround({ rect.x + rect.z , rect.y + rect.w }, originv, rad), scr_size));

		}
		else {
			vertecies.push_back(inter::pos2scr_coords({ rect.x , rect.y }, scr_size));
			vertecies.push_back(inter::pos2scr_coords({ rect.x + rect.z , rect.y }, scr_size));
			vertecies.push_back(inter::pos2scr_coords({ rect.x + rect.z , rect.y + rect.w }, scr_size));

			vertecies.push_back(inter::pos2scr_coords({ rect.x , rect.y + rect.w }, scr_size));
			vertecies.push_back(inter::pos2scr_coords({ rect.x , rect.y }, scr_size));
			vertecies.push_back(inter::pos2scr_coords({ rect.x + rect.z , rect.y + rect.w }, scr_size));
		}*/
		if (rad)
		{
			v0 = inter::retateoround(v0, originv, rad);
			v1 = inter::retateoround(v1, originv, rad);
			v2 = inter::retateoround(v2, originv, rad);
			v3 = inter::retateoround(v3, originv, rad);
		}

		v0 -= current_cam.pos;
		v1 -= current_cam.pos;
		v2 -= current_cam.pos;
		v3 -= current_cam.pos;

		if (current_cam.rad) {
			v0 = inter::retateoround(v0, cam_center, current_cam.rad);
			v1 = inter::retateoround(v1, cam_center, current_cam.rad);
			v2 = inter::retateoround(v2, cam_center, current_cam.rad);
			v3 = inter::retateoround(v3, cam_center, current_cam.rad);
		}

		v0 = inter::scalearound(v0, cam_center, current_cam.scale);
		v1 = inter::scalearound(v1, cam_center, current_cam.scale);
		v2 = inter::scalearound(v2, cam_center, current_cam.scale);
		v3 = inter::scalearound(v3, cam_center, current_cam.scale);

		v0 = inter::pos2scr_coords(v0, scr_size);
		v1 = inter::pos2scr_coords(v1, scr_size);
		v2 = inter::pos2scr_coords(v2, scr_size);
		v3 = inter::pos2scr_coords(v3, scr_size);
		bool in0 = true;
		bool in1 = true;
		bool in2 = true;
		bool in3 = true;
		if (glm::abs(v0.x) > 1)
			in0 = false;
		else if (glm::abs(v0.y) > 1)
			in0 = false;
		if (glm::abs(v1.x) > 1)
			in1 = false;
		else if (glm::abs(v1.y) > 1)
			in1 = false;
		if (glm::abs(v2.x) > 1)
			in2 = false;
		else if (glm::abs(v2.y) > 1)
			in2 = false;
		if (glm::abs(v3.x) > 1)
			in3 = false;
		else if (glm::abs(v3.y) > 1)
			in3 = false;

		if (in0 || in1 || in2 || in3) {
			std::array<glm::vec2, 6> a0 = { v0, v1, v2, v3, v0, v2 };
			std::array<glm::vec2, 6> a1 = { glm::vec2{ (tex_coords.x), 1.f - (tex_coords.y) }, glm::vec2{ ((tex_coords.x + tex_coords.z)),  1 - (tex_coords.y) }, glm::vec2{ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) }, { (tex_coords.x),  1 - ((tex_coords.y + tex_coords.w)) }, { (tex_coords.x), 1 - (tex_coords.y) }, glm::vec2{ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) } };
			vertecies.insert(vertecies.end(), a0.begin(), a0.end());
			texcoords.insert(texcoords.end(), a1.begin(), a1.end());
			//vertecies.push_back(v0);
			//vertecies.push_back(v1);
			//vertecies.push_back(v2);

			//vertecies.push_back(v3);
			//vertecies.push_back(v0);
			//vertecies.push_back(v2);

			//texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
			//texcoords.push_back({ ((tex_coords.x + tex_coords.z)),  1 - (tex_coords.y) });
			//texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });

			//texcoords.push_back({ (tex_coords.x),  1 - ((tex_coords.y + tex_coords.w)) });
			//texcoords.push_back({ (tex_coords.x), 1 - (tex_coords.y) });
			//texcoords.push_back({ ((tex_coords.x + tex_coords.z)), 1 - ((tex_coords.y + tex_coords.w)) });

			uint32_t _color;
			_color = (color.x << 24) | (color.y << 16) | (color.z << 8) | color.w;

			colors.push_back(_color);
			colors.push_back(_color);
			colors.push_back(_color);
			colors.push_back(_color);
			colors.push_back(_color);
			colors.push_back(_color);
			shape_info.push_back({ 6, tex.id });
		}
		//indecies.push_back(0 + s); indecies.push_back(1 + s); indecies.push_back(2 + s);
		//indecies.push_back(3 + s); indecies.push_back(0 + s); indecies.push_back(2 + s);

	}
	void renderer::renderrectangle(const glm::vec4& rect, const glm::uvec4& color, const glm::vec2& oringin, const float& rad)
	{
		renderrectangle(rect, color, defaults::white1px, { 0, 0, 1, 1 }, oringin, rad);
	}
	void renderer::rendercircle2scr(const glm::vec3& circle, const glm::uvec4& color, const texture& tex, uint32_t seg)
	{
		uint32_t num = 0;
		glm::vec2 v0;
		uint32_t _color = 0;
		_color = (color.x << 24) | (color.y << 16) | (color.z << 8) | color.w;
		for (int i = 0; i < seg; i++) {
			v0 = { glm::sin(((i) * 6.283184f) / seg) * circle.z + circle.x , glm::cos(((i) * 6.283184f) / seg) * circle.z + circle.y };
			vertecies.push_back(inter::pos2scr_coords(v0, scr_size));
			v0 = { glm::sin(((i - 1) * 6.283184f) / seg) * circle.z + circle.x , glm::cos(((i - 1) * 6.283184f) / seg) * circle.z + circle.y };
			vertecies.push_back(inter::pos2scr_coords(v0, scr_size));
			v0 = { circle.x  , circle.y };
			vertecies.push_back(inter::pos2scr_coords(v0, scr_size));
			texcoords.push_back({ 0, 0 });
			texcoords.push_back({ 0, 0 });
			texcoords.push_back({ 0, 0 });
			colors.push_back(_color);
			colors.push_back(_color);
			colors.push_back(_color);
			num += 3;
		}
		shape_info.push_back({ num, tex.id });
	}
	void renderer::rendercircle2scr(const glm::vec3& circle, const glm::uvec4& color, uint32_t seg)
	{
		rendercircle2scr(circle, color, defaults::white1px, seg);
	}
	void renderer::rendercircle(const glm::vec3& circle, const glm::uvec4& color, const texture& tex, uint32_t seg)
	{
		glm::vec2 cam_center = scr_size / 2.f;
		uint32_t num = 0;
		glm::vec2 v0;
		uint32_t _color = 0;
		_color = (color.x << 24) | (color.y << 16) | (color.z << 8) | color.w;
		//const uint32_t s = seg;
		//std::array<glm::vec2, s * 3>a0;
		for (int i = 0; i < seg; i++) {
			v0 = inter::retateoround({ glm::sin(((i) * 6.283184f) / seg) * circle.z + circle.x - current_cam.pos.x, glm::cos(((i) * 6.283184f) / seg) * circle.z + circle.y - current_cam.pos.y }, cam_center, current_cam.rad);
			vertecies.push_back(inter::pos2scr_coords(inter::scalearound(v0, cam_center, current_cam.scale), scr_size));
			v0 = inter::retateoround({ glm::sin(((i - 1) * 6.283184f) / seg) * circle.z + circle.x - current_cam.pos.x, glm::cos(((i - 1) * 6.283184f) / seg) * circle.z + circle.y - current_cam.pos.y }, cam_center, current_cam.rad);
			vertecies.push_back(inter::pos2scr_coords(inter::scalearound(v0, cam_center, current_cam.scale), scr_size));
			v0 = inter::retateoround({ circle.x - current_cam.pos.x, circle.y - current_cam.pos.y }, cam_center, current_cam.rad);
			vertecies.push_back(inter::pos2scr_coords(inter::scalearound(v0, cam_center, current_cam.scale), scr_size));
			texcoords.push_back({ 0, 0 });
			texcoords.push_back({ 0, 0 });
			texcoords.push_back({ 0, 0 });
			colors.push_back(_color);
			colors.push_back(_color);
			colors.push_back(_color);
			num += 3;
		}
		shape_info.push_back({ num, tex.id });
	}
	void renderer::rendercircle(const glm::vec3& circle, const glm::uvec4& color, uint32_t seg)
	{
		rendercircle(circle, color, defaults::white1px, seg);
	}
	void renderer::rendertext2scr(const glm::vec2& pos, const char* text, const font& f, const glm::uvec4& color, const float& size, const float& spacing, const float& line_spacing, bool show_incenter)
	{
		glm::vec2 _pos = pos;
		if (show_incenter)
		{
			auto textSize = inter::gettextsize(text, f, size, spacing, line_spacing);

			_pos.x -= textSize.x / 2.f;
			_pos.y += textSize.y / 2.f;
		}
		const uint32_t length = strlen(text);
		glm::vec4 rect;
		stbtt_aligned_quad quad;
		rect.x = _pos.x;
		float line_pos = _pos.y;
		for (uint32_t i = 0; i < length; i++) {
			if (text[i] == '\n')
			{
				rect.x = _pos.x;
				line_pos += (f.max_hight + line_spacing) * size;
			}
			else if (text[i] == '\t')
			{
				quad = inter::getfontglyphquad(f, '_');
				auto x = quad.x1 - quad.x0;

				rect.x += x * size * 3 + spacing * size;
			}
			else if (text[i] == ' ')
			{
				quad = inter::getfontglyphquad(f, '_');
				auto x = quad.x1 - quad.x0;
				rect.x += x * size + spacing * size;
			}
			else if (text[i] >= ' ' && text[i] <= '~') {
				quad = inter::getfontglyphquad(f, text[i]);
				rect.z = size * (quad.x1 - quad.x0);
				rect.w = size * (quad.y1 - quad.y0);
				rect.y = line_pos + quad.y0 * size;
				//std::cout << quad.s0 << "	" << quad.t0 << "	" << quad.s1 - quad.s0 << "	" << quad.t1 - quad.t0 << "\n";
				renderrectangle2scr(rect, color, f.text, { quad.s0, quad.t0, quad.s1 - quad.s0, quad.t1 - quad.t0 });
				rect.x += rect.z + spacing * size;
			}
		}
	}
	void renderer::rendertext(const glm::vec2& pos, const char* text, const font& f, const glm::uvec4& color, const float& size, const float& spacing, const float& line_spacing, bool show_incenter)
	{
		glm::vec2 _pos = pos;
		if (show_incenter)
		{
			auto textSize = inter::gettextsize(text, f, size, spacing, line_spacing);

			_pos.x -= textSize.x / 2.f;
			_pos.y += textSize.y / 2.f;
		}
		const uint32_t length = strlen(text);
		glm::vec4 rect;
		stbtt_aligned_quad quad;
		rect.x = _pos.x;
		float line_pos = _pos.y;
		for (uint32_t i = 0; i < length; i++) {
			if (text[i] == '\n')
			{
				rect.x = _pos.x;
				line_pos += (f.max_hight + line_spacing) * size;
			}
			else if (text[i] == '\t')
			{
				quad = inter::getfontglyphquad(f, '_');
				auto x = quad.x1 - quad.x0;

				rect.x += x * size * 3 + spacing * size;
			}
			else if (text[i] == ' ')
			{
				quad = inter::getfontglyphquad(f, '_');
				auto x = quad.x1 - quad.x0;
				rect.x += x * size + spacing * size;
			}
			else if (text[i] >= ' ' && text[i] <= '~') {
				quad = inter::getfontglyphquad(f, text[i]);
				rect.z = size * (quad.x1 - quad.x0);
				rect.w = size * (quad.y1 - quad.y0);
				rect.y = line_pos + quad.y0 * size;
				//std::cout << quad.s0 << "	" << quad.t0 << "	" << quad.s1 - quad.s0 << "	" << quad.t1 - quad.t0 << "\n";
				renderrectangle(rect, color, f.text, { quad.s0, quad.t0, quad.s1 - quad.s0, quad.t1 - quad.t0 });
				rect.x += rect.z + spacing * size;
			}
		}
	}
#pragma endregion


	uint32_t texture::loadfrompixledata(const unsigned char* data, glm::ivec3 dem, bool pixlated, bool use_mipmap)
	{
		if (data) {
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			//GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER      GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			if (pixlated) {
				if (use_mipmap)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				if (use_mipmap)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dem.x, dem.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		return id;
	}
	uint32_t texture::loadfromfile(const char* filename, bool pixlated, bool use_mipmap)
	{
		unsigned char* data = stbi_load(filename, &w, &h, &n, 4);
		loadfrompixledata(data, { w, h, n }, pixlated, use_mipmap);
		stbi_image_free(data);
		return id;
	}
	glm::vec4 texture::pos2texcoords(glm::vec4 pos)
	{
		//texcoords.push_back({ (tex_coords.x / tex.w), 1 - (tex_coords.y / tex.h) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z) / tex.w),  1 - (tex_coords.y / tex.h) });
		//texcoords.push_back({ ((tex_coords.x + tex_coords.z) / tex.w), 1 - ((tex_coords.y + tex_coords.w) / tex.h) });
		//texcoords.push_back({ (tex_coords.x / tex.w),  1 - ((tex_coords.y + tex_coords.w) / tex.h) });
		return glm::vec4((pos.x / w), (pos.y / h), ((pos.z - pos.x) / w), ((pos.w - pos.y) / h));
	}
	void texture::cleanup()
	{
		glDeleteTextures(1, &id);
		*this = {};
	}
	void font::creatfromttf(const unsigned char* data, bool pixlated, bool usemip)
	{
		size.x = 2000,
			size.y = 2000,
			max_hight = 0,
			buff_size = ('~' - ' ');
		const uint32_t fontMonochromeBufferSize = size.x * size.y;
		const uint32_t fontRgbaBufferSize = size.x * size.y * 4;
		unsigned char* monochrome = new unsigned char[fontMonochromeBufferSize];
		unsigned char* rgba = new unsigned char[fontRgbaBufferSize];

		pakedCHbuff = new stbtt_packedchar[buff_size]{};

		stbtt_pack_context stbtt_context;
		stbtt_PackBegin(&stbtt_context, monochrome, size.x, size.y, 0, 2, NULL);
		stbtt_PackSetOversampling(&stbtt_context, 2, 2);
		stbtt_PackFontRange(&stbtt_context, data, 0, 65, ' ', '~' - ' ', pakedCHbuff);
		stbtt_PackEnd(&stbtt_context);

		//for (uint32_t i = 0; i < fontMonochromeBufferSize; i++) {
		//	rgba[(i * 4)] = monochrome[i];
		//	rgba[(i * 4) + 1] = monochrome[i];
		//	rgba[(i * 4) + 2] = monochrome[i];

		//	if (monochrome[i] > 1)
		//	{
		//		rgba[(i * 4) + 3] = 255;
		//	}
		//	else
		//	{
		//		rgba[(i * 4) + 3] = 0;
		//	}
		//}
		for (uint32_t y = 0; y < size.y; ++y) {
			for (uint32_t x = 0; x < size.x; ++x) {
				uint32_t i = x + y * size.x;
				uint32_t flipped_y = size.y - 1 - y;
				uint32_t flipped_i = x + flipped_y * size.x;

				rgba[(flipped_i * 4)] = monochrome[i];
				rgba[(flipped_i * 4) + 1] = monochrome[i];
				rgba[(flipped_i * 4) + 2] = monochrome[i];
				rgba[(flipped_i * 4) + 3] = monochrome[i];

				//if (monochrome[i] > 1) {
				//	rgba[(flipped_i * 4) + 3] = 255;
				//}
				//else {
				//	rgba[(flipped_i * 4) + 3] = 0;
				//}
			}
		}
		delete[] monochrome;
		text.loadfrompixledata(rgba, { size.x, size.y, 4 }, pixlated, usemip);
		delete[] rgba;
		for (char c = ' '; c <= '~'; c++)
		{
			const stbtt_aligned_quad  q = inter::getfontglyphquad(*this, c);
			const float               m = q.y1 - q.y0;

			if (m > max_hight && m < 1.e+8f)
			{
				max_hight = m;
			}
		}
	}
	void font::creatfromfile(const char* file, bool pixlated, bool usemip)
	{
		std::ifstream fileFont(file, std::ios::binary);

		if (!fileFont.is_open())
		{
			std::cout << "font not loading\n";
			return;
		}

		int fileSize = 0;
		fileFont.seekg(0, std::ios::end);
		fileSize = (int)fileFont.tellg();
		fileFont.seekg(0, std::ios::beg);
		unsigned char* fileData = new unsigned char[fileSize];
		fileFont.read((char*)fileData, fileSize);
		fileFont.close();

		creatfromttf(fileData, pixlated, usemip);

		delete[] fileData;
	}
	void font::cleanup()
	{
		text.cleanup();
		*this = {};
	}
}

