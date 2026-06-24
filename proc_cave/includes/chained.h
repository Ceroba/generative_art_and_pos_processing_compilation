#pragma once
#include "gbl.h"
#define PI 3.1415926535897932
class chain {
public:
	bool animation_mode = false;
	std::vector<glm::vec2> joints;
	float spacing;	std::vector<float> angles;

	float angle_constraint;
	chain() = default;
	chain(glm::vec2 origin, uint32 n_joints, float _spacing, float _angle_constraint = PI / 8) {
		spacing = _spacing;
		angle_constraint = _angle_constraint;
		angles.resize(n_joints);
		joints.push_back(origin);
		for (uint32 i = 1; i < n_joints; i++) {
			joints.push_back(joints[i - 1] + glm::vec2(0, -1) * spacing);
		}
	}
	void render(vx::renderer & renderer){
		for (auto i : joints) {
			renderer.renderrectangle({ i.x, i.y, 32, 32 }, { 1, 1, 1, 1 });
		}
	}
	void update() {
		for (uint32 i = 1; i < joints.size(); i++) {
			joints[i] = joints[i - 1] + glm::normalize(joints[i] - joints[i - 1]) * spacing;
		}
	}
	void updatereversed() {
		for (int i = joints.size() - 2; i > -1; i--) {
			joints[i] = joints[i + 1] + glm::normalize(joints[i] - joints[i + 1]) * spacing;
		}
	}
	void update_constrain() {
		angles[0] = utl::math::heading(glm::normalize(joints[0] - joints[1]));
		for (int i = 1; i < joints.size(); i++) {
			float cur = utl::math::heading(glm::normalize(joints[i - 1] - joints[i]));
			angles[i] = glm::clamp(cur, angles[i - 1] - angle_constraint, angles[i - 1] + angle_constraint);
			joints[i] = utl::math::lerp((joints[i - 1] - glm::vec2{ glm::cos(angles[i]), glm::sin(angles[i]) }* spacing), joints[i - 1] + glm::normalize(joints[i] - joints[i - 1]) * spacing, 0.25);
		}
	}
	void updatefab(glm::vec2 pos, glm::vec2 tar) {
		if (glm::length(tar - pos) > spacing * (joints.size() - 1))
			return;
		for (int i = 0; i < 3; i++) {
			joints[joints.size() - 1] = tar;
			updatereversed();
			joints[0] = pos;
			update();


		}
	}
	void resolve(const glm::vec2& pos) {
		joints[0] = pos;
		update();
	}
};
class snake_body {
public:
	chain ch;
	//snake_body() = default;
	snake_body(glm::vec2 origin = glm::vec2(512, 512)) {
		ch = chain(origin, 64, 32);
	}
	void update() {
		ch.update_constrain();
	}
	void updatereversed() {
		ch.updatereversed();
	}
	void render(vx::renderer& renderer) {
		//renderer.renderCircleOutline(ch.joints[1], { 1, 1, 1, 1 }, 48 * 1.25 + 32, 8, 64);

		for (uint32 i = ch.joints.size() - 1; i > 2; i--) {
			renderer.rendercircle({ ch.joints[i].x, ch.joints[i].y,  ((ch.joints.size()) - i) * 1.25 + 16 }, utl::math::lerp({ 0, 255, 128, 255 }, { 230, 230, 255, 255 }, (float)i / ch.joints.size()));
			//renderer.renderCircleOutline(ch.joints[i], { 1, 1, 1, 1 }, ((ch.joints.size()) - i) * 1.25 + 16, 8, 64);
		}
		renderer.rendercircle({ ch.joints[0].x, ch.joints[0].y,  48 * 1.25 + 12 }, { 0, 255, 128, 255 });
		renderer.rendercircle({ ch.joints[1].x, ch.joints[1].y,  48 * 1.25 + 32 }, { 0, 255, 128, 255 });
		//ch.render(renderer);
	}
};
class fish_body {
public:
	chain ch;
	//snake_body() = default;
	static constexpr float bodyWidth[] = { 68, 81, 84, 83, 77, 64, 51, 38, 32, 24 };

	fish_body(glm::vec2 origin = glm::vec2(512, 512)) {
		ch = chain(origin, 12, 64);
	}
	void update() {
		ch.update();
	}
	void updatereversed() {
		ch.updatereversed();
	}
	void render(vx::renderer& renderer) {
		//renderer.renderCircleOutline(ch.joints[1], { 1, 1, 1, 1 }, 48 * 1.25 + 32, 8, 64);

		for (int i = ch.joints.size() - 1; i > -1; i--) {
			renderer.rendercircle({ ch.joints[i].x, ch.joints[i].y, bodyWidth[i]}, utl::math::lerp({0, 255, 255, 255}, {230, 230, 255, 255}, (float)i / ch.joints.size()));
			//renderer.renderCircleOutline(ch.joints[i], { 1, 1, 1, 1 }, ((ch.joints.size()) - i) * 1.25 + 16, 8, 64);
		}
		//ch.render(renderer);
	}
};
class leg {
public:
	chain ch;
	glm::vec2 pos;
	glm::vec2 target = { 0, 0 };
	leg(glm::vec2 origin = { 512, 512 }, uint8 seg = 2, float spacing = 10, float angleconstrain = PI / 2) {
		ch = chain(origin, seg, spacing, angleconstrain);
		pos = origin;
	}
	void update() {
		if (glm::length(target - pos) > ch.spacing * (ch.joints.size() - 1))
			return;
		for (int i = 0; i < 3; i++) {
			ch.joints[ch.joints.size() - 1] = target;
			ch.updatereversed();
			ch.joints[0] = pos;
			ch.update();


		}
	}
	void render(vx::renderer& r) {
		for (int i = 0; i < ch.joints.size(); i++) {
			r.rendercircle({ ch.joints[i].x, ch.joints[i].y, 10 }, { 255, 255, 255, 255 });
		}
	}

}; 
class dragon_body
{
public:
	chain ch;
	dragon_body(glm::vec2 origin = glm::vec2(120, 120)) {
		ch = chain(origin, 15, 6);
	}
	//snake_body() = default;
	//static constexpr float bodyWidth[] = { 68, 81, 84, 83, 77, 64, 51, 38, 32, 24, 68, 81, 84, 83, 77, 64, 51, 38, 32, 24,68, 81, 84, 83, 77, 64 };
	static constexpr float bodyWidth[] = { 68, 75, 81, 84, 83, 77, 70, 64, 51,40, 38, 32, 24};
	
	
	void update() {
		ch.update();
	}
	void updatereversed() {
		ch.updatereversed();
	}
	void render(vx::renderer& renderer, glm::vec2 target) {
		//renderer.renderCircleOutline(ch.joints[1], { 1, 1, 1, 1 }, 48 * 1.25 + 32, 8, 64);
		//renderer.renderrectangle({ ch.joints[3].x, ch.joints[3].y - 32, 16, 32 }, { 255, 224, 41, 128},{0, 32}, -0.4);
		//for (int i = ch.joints.size() - 1; i > -1; i--) {
		//	renderer.rendercircle({ ch.joints[i].x, ch.joints[i].y, bodyWidth[i] / 16 }, utl::math::lerp({ 255, 224, 41, 255 }, { 245, 141, 10, 255 }, (float)i / ch.joints.size()), 32);//230, 230, 255
		//	//renderer.renderCircleOutline(ch.joints[i], { 1, 1, 1, 1 }, ((ch.joints.size()) - i) * 1.25 + 16, 8, 64);
		//}
		//renderer.renderrectangle({ ch.joints[3].x, ch.joints[3].y - 32 , 16, 32 }, { 255, 224, 41, 128 }, {}, -0.2);
		for (int i = ch.joints.size() - 1; i > -1; i--) {
			renderer.renderrectangle({ ch.joints[i] - glm::vec2(6, (bodyWidth[i] / 16) / 2), 12, bodyWidth[i] / 16 }, utl::math::lerp({ 255, 224, 41, 255 }, { 245, 141, 10, 255 }, (float)i / ch.joints.size()), { 6, ((bodyWidth[i] / 16.f) / 2.f) }, i ? utl::math::heading(ch.joints[i - 1] - ch.joints[i]) : utl::math::heading(target-ch.joints[i]));//230, 230, 255
			//renderer.renderCircleOutline(ch.joints[i], { 1, 1, 1, 1 }, ((ch.joints.size()) - i) * 1.25 + 16, 8, 64);
		}

		//ch.render(renderer);
	}
	void render_mer(vx::renderer& renderer,float m = 200) {
		//renderer.renderrectangle({ ch.joints[3].x, ch.joints[3].y - 32, 16, 32 }, { 255, 224, 41, 128 }, { 0, 32 }, -0.4);
		//for (int i = ch.joints.size() - 1; i > -1; i--) {
		//	renderer.rendercircle({ ch.joints[i].x, m + (m - ch.joints[i].y), bodyWidth[i] / 16 }, utl::math::lerp({ 255, 224, 41, 255 }, { 245, 141, 10, 255 }, (float)i / ch.joints.size()), 32);//230, 230, 255
		//}
		//renderer.renderrectangle({ ch.joints[3].x, ch.joints[3].y - 32 , 16, 32 }, { 255, 224, 41, 128 }, {}, -0.2);
		//for (int i = ch.joints.size() - 1; i > -1; i--) {
		//	renderer.rendercircle({ ch.joints[i].x, m + (m - ch.joints[i].y), bodyWidth[i] / 16 }, utl::math::lerp({ 255, 224, 41, 255 }, { 245, 141, 10, 255 }, (float)i / ch.joints.size()), 32);//230, 230, 255
		//}
	}
private:

};

class wing {
public:
	void render(vx::renderer &rend) {
		for (int i = 0; i < 30; ++i) {

			//rend.renderrectangle({ i * 8,100 + glm::sin(gbl::time * (3 - (i / 30.0) * 3) + (i / 30.0) * 6.28) * (i / 30.0) * 50/** (i / 30) * 100*/, 8, 8 }, { 255, 255, 0, 255 });
		}
	}
};

class verletobj {
public:
	glm::vec2 pos;
	glm::vec2 old_pos;
	glm::vec2 vel;
	glm::vec2 acc;
	float rad = 8;
	float old_dt= 8;
	bool statc = false;
	verletobj(glm::vec2 _pos, float _rad = 8) {
		pos = old_pos = _pos; vel = acc = {}; old_dt = 0.01666; rad = _rad;
	}
	verletobj() = default;
	void render(vx::renderer& rend) {
		rend.rendercircle({ pos, 8 }, { 255, 255, 255, 255 });
	}
	void updatepos(float dt) {
		//float dtr = dt / old_dt;
		if (statc)return;
		vel = (pos - old_pos);
		old_pos = pos;
		pos += vel * 0.99999f + acc * dt * dt;
		old_dt = dt;
		acc = {};
	}
	void updatepos() {
		vel = pos - old_pos;
		old_pos = pos;
		pos += vel + acc * gbl::dt * gbl::dt;
		acc = {};
	}
	void xlr8(glm::vec2 _acc) {
		acc += _acc;
	}
};


class link {
public:
	int obj1;
	int obj2;
	float len;
	link(int a, int b, float _len) :obj1(a), obj2(b){
		obj1 = a;
		obj2 = b;
		len = _len;
	}
	void update(std::vector<verletobj> &objs) {
		const glm::vec2 v = objs[obj1].pos - objs[obj2].pos;
		const float dist = glm::length(v);
		if (dist > len || dist < len) {
			const float delta = dist - len;
			if (!objs[obj1].statc)
			{
				objs[obj1].pos -= glm::normalize(v) * delta * 0.5f;
				objs[obj2].pos += glm::normalize(v) * delta * 0.5f;
			}
			else {
				objs[obj2].pos += glm::normalize(v) * delta;

			}
		}
	}
};

class absloutsolver {
public:
	std::vector<verletobj> particals;
	std::vector<link> links;
	glm::vec2 grav = { 0, 1000 };
	void update() {
		const int sub_steps = 8;
		float sub_dt = gbl::dt / (float)sub_steps;
		for (int i = 0; i < sub_steps; ++i) {

			applygrav();
			solvecol();
			updatelinks();
			//applyconst({ 200, 100, 100 });
			updatepos(sub_dt);
		}
	}
	void createchain(glm::vec2 o, float spacing, float rad, int seg) {
		int a = particals.size();
		for (auto i = 0; i < seg; ++i) {
			particals.push_back(verletobj(glm::vec2(0, i * rad * 2) + o, rad));
		}
		for (int i = a; i < particals.size() - 1; ++i) {
			links.push_back(link(i, i + 1, rad * 2));
		}
		particals[a].statc = true;
	}
	void updatelinks() {
		for (auto& i : links) {
			i.update(particals);
		}
	}
	void update(float dt) {
		applygrav();
		updatepos(dt);
		solvecol();
	}
	void updatepos() {
		for (auto& i : particals) {
			i.updatepos();
		}
	}
	void updatepos(float dt) {
		for (auto& i : particals) {
			i.updatepos(dt);
		}
	}
	void applygrav() {
		for (auto& i : particals) {
			i.xlr8(grav);
		}
	}
	void applyconst(const glm::vec3 &cir) {
		for (auto& i : particals) {
			const glm::vec2 v = glm::vec2(cir.x, cir.y) - i.pos ;
			if (glm::length(v) > cir.z - i.rad) {
				i.pos = glm::vec2(cir.x, cir.y) + glm::normalize(-v) * (cir.z - i.rad);
			}
		}
	}
	void solvecol() {
		for (int i = 0; i < particals.size(); ++i) {
			verletobj& obj1 = particals[i];
			for (int j = i + 1; j < particals.size(); ++j) {
				verletobj& obj2 = particals[j];
				glm::vec2 v = obj2.pos - obj1.pos;
				float rad = obj1.rad + obj2.rad;
				float dist = glm::length(v);
				if (dist < rad) {
					float delta = rad - dist;
					 glm::vec2 n = glm::normalize(v);
					obj2.pos += n * (delta * 0.5f);
					obj1.pos -= n * (delta * 0.5f);
				}
			}
		}
	}
	void render(vx::renderer& rend) {
		for (auto i : particals) {
			i.render(rend);
		}
	}
};

class string {
public:
	std::vector<verletobj> particals;
	glm::vec2 grav = { 0, 1000 };
	std::vector<link> links;
	string() = default;
	string(glm::vec2 o, float spacing, float rad, int seg) {
		int a = particals.size();
		for (auto i = 0; i < seg; ++i) {
			particals.push_back(verletobj(glm::vec2(0, i * rad * 2) + o, rad));
		}
		for (int i = a; i < particals.size() - 1; ++i) {
			links.push_back(link(i, i + 1, rad * 2));
		}
		particals[a].statc = true;
	}
	void createchain(glm::vec2 o, float spacing, float rad, int seg) {
		int a = particals.size();
		for (auto i = 0; i < seg; ++i) {
			particals.push_back(verletobj(glm::vec2(0, i * rad * 2) + o, rad));
		}
		for (int i = a; i < particals.size() - 1; ++i) {
			links.push_back(link(i, i + 1, rad * 2));
		}
		particals[a].statc = true;
	}
	void update() {
		const int sub_steps = 8;
		float sub_dt = gbl::dt / (float)sub_steps;
		for (int i = 0; i < sub_steps; ++i) {

			applygrav();
			//solvecol();
			updatelinks();
			//applyconst({ 200, 100, 100 });
			updatepos(sub_dt);
		}
	}
	void updatepos(float dt) {
		for (auto& i : particals) {
			i.updatepos(dt);
		}
	}
	void applygrav() {
		for (auto& i : particals) {
			i.xlr8(grav);
		}
	}
	void updatelinks() {
		for (auto& i : links) {
			i.update(particals);
		}
	}
	void render(vx::renderer& rend) {
		for (int i = 0; i < links.size(); ++i) {
			auto& l = links[i];
			auto& o1 = particals[l.obj1];
			auto& o2 = particals[l.obj2];
			glm::vec2 vec = o2.pos + (o1.pos - o2.pos) * 0.5f;
			float width = utl::math::lerp(4, 6, i / (float)links.size());
			rend.renderrectangle({ vec - glm::vec2(l.len / 2 + 0.5, width / 2), l.len + 1, width}, utl::math::lerp({195, 148, 246, 190}, {86, 255, 228, 201}, i / (float)links.size()), glm::vec2(l.len / 2 + 0.5, width / 2), utl::math::heading(o2.pos - o1.pos));
			//rend.renderrectangle({ vec - glm::vec2(l.len / 2 + 0.5, 2), l.len + 1, 4 }, { 0, 255, 219, 201}, glm::vec2(l.len / 2 + 0.5, 2), utl::math::heading(o2.pos - o1.pos));
		}
	}
};
struct forcefield {
	glm::vec2 force;
	glm::vec2 pos;
	float rad;
};
class vegetation {
public:
	std::vector<string> strs;
	std::vector<forcefield> ff;
	void create(const glm::vec2 & windoow_size) {
		for (int i = 0; i < 16; ++i) {
			strs.push_back(string({ utl::random::GetRandom(8.f, windoow_size.x - 8.f), 0 }, 10, 8, glm::floor(utl::random::GetRandom(5.f, 12.f))));
		}
	}
	void update() {

		for (auto& i : strs) {
			for (auto& k : ff) {
				for (auto& j : i.particals) {
					if (glm::length(j.pos - k.pos) < j.rad + k.rad) {
						j.xlr8(k.force);
					}
				}
			}
			i.update();
		}
	}
};
struct spring {
	int obj1, obj2;
	float rest_l;
	float k;
	void update(std::vector<verletobj>& objs) {
		verletobj& a = objs[obj1];
		verletobj& b = objs[obj2];
		glm::vec2 force = a.pos - b.pos;
		float f = (std::sqrt(force.x * force.x + force.y * force.y) - rest_l) * k;
		force = glm::normalize(force);
		force *= f;
		a.xlr8(force / 2.f);
		b.xlr8(-force / 2.f);
	}
};
class softbody {
public:
	std::vector<verletobj> objs;
	std::vector<spring> springs;
	void update() {
		for (auto& i : springs) {
			i.update(objs);
		}
		for (auto& i : objs) {
			i.updatepos();
		}
	}
};
class jellyfish_body {
};