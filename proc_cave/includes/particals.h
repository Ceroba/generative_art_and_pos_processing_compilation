#pragma once
#include "anima.h"
struct partical {
	glm::vec2 pos;
	glm::vec2 vel;
	glm::uvec4 color;
	glm::vec2 size;
	glm::vec2 final_size;
	float size_speed;
	float rotate;
	float life_time;
	float life_time_timer;
};
class partical_emitter {
	std::vector<partical> particals;
	int max_particals;
	float cool_down;
	float cool_down_timer;
	glm::vec4 rect;
	float gravity;
	partical temp;
public:
	partical_emitter() = default;
	partical_emitter(int _max_particals, float _cool_down, glm::vec4 _rect, float _gravity) {
		max_particals = _max_particals;
		cool_down = _cool_down;
		rect = _rect;
		gravity = _gravity;
		cool_down_timer = _cool_down;
		temp = {
				{50, 120},
				{},
				{255, 255, 255, 100},
				{20, 20},
				{0, 0},
				5,
				0,
				8,
				8
		};
		//for (int i = 0; i < 10; ++i) {
		//	particals.push_back({
		//		{i * 50 + 50, 120},
		//		{},
		//		{255, 255, 255, 100},
		//		{20, 20},
		//		{0, 0},
		//		5,
		//		0,
		//		8,
		//		8
		//						});
		//}
	}
	
	void update() {
		if (cool_down_timer < 0 && particals.size() <= max_particals) {
			//spawn
			particals.push_back({
				{utl::random::GetRandom(rect.x, rect.z), utl::random::GetRandom(rect.y, rect.w)},
				{utl::random::GetRandom(-5.0, 5.0), 0.0},
				{83, 74, 69, 128},
				{8, 8},
				{1, 1},
				2,
				0,
				8,
				8
			});
			cool_down_timer = cool_down;
		}
		else {
			cool_down_timer -= gbl::dt;
			//adveance timer
		}
		for (int i = 0; i < particals.size(); ++i) {
			if (particals[i].life_time_timer < 0) {
				//kill partical
				particals.erase(particals.begin() + i);
				--i;
			}
			else {
				particals[i].life_time_timer -= gbl::dt;
				//advance time
			}
		}
		for (auto& i : particals) {
			//update position scale and rotation
			i.size = utl::MoveTowards(i.size, i.final_size, i.size_speed * gbl::dt);
			i.vel.y += gravity * gbl::dt;
			i.pos += i.vel * gbl::dt;
		}
	}
	void render(vx::renderer& rend) {
		for (auto &i : particals) {
			rend.renderrectangle({ i.pos.x, i.pos.y , i.size.x , i.size.y }, i.color, {}, i.rotate);
		}
	}

};