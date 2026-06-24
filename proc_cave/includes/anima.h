#pragma once
#include"chained.h"
bool point2aabb(glm::vec2 p, glm::vec4 aabb) {
	return (p.x > aabb.x && p.x < aabb.x + aabb.z && p.y > aabb.y && p.y < aabb.y + aabb.w);
}
bool ray2aabb(glm::vec4 ray, glm::vec4 aabb, float len) {
	glm::vec4 box = { aabb.x - ray.x, aabb.y - ray.y, aabb.z, aabb.w };
	int step = 5;
	for (int i = 0; i < len; i += step) {
		if (point2aabb(glm::vec2(ray.x + ray.z * i, ray.y + ray.w * i), aabb))return true;
	}
	return false;
}
class dragon
{
	glm::vec2 vel;
	struct
	{
		float move_speed = 16.f;
		float wandering_distence = 30;
		float turning_speed = 3.14 / 3;
		float acc = 8;
	}stats;
	glm::vec2 pot_target;
	float wandering_angle;
	float target_angle;
	void handleinput() {
	}
	void behave(std::vector<glm::vec4> aabbs) {
		wandering_angle += utl::random::GetRandom(-0.05, 0.05);
		pot_target = body.ch.joints[0] + utl::math::from_angel(wandering_angle) * stats.wandering_distence;
		if (pot_target.x > 640 || pot_target.x < 0)
			wandering_angle += 3.14 / 2;

		if (pot_target.y > 295 || pot_target.y < 0)
			wandering_angle += 3.14 / 2;
		for (auto & i : aabbs)
			if (ray2aabb(glm::vec4(body.ch.joints[0], glm::normalize(pot_target - body.ch.joints[0])), i, stats.wandering_distence)) {
				wandering_angle += 3.14 / 2;
			}
		target_angle = utl::MoveTowards(target_angle, wandering_angle, stats.turning_speed * gbl::dt);
		//target += utl::math::from_angel(wandering_angle) * stats.wandering_distence;
		target = body.ch.joints[0] + utl::math::from_angel(target_angle)	* stats.wandering_distence;
		
			//vel = (target - body.ch.joints[0]) * stats.move_speed;
	}
public:
	dragon_body body;
	glm::vec2 target;
	dragon() {
		target = glm::vec2(0);
		vel = glm::vec2(0);
		wandering_angle = utl::random::GetRandom(-3.14999, 3.14999);
		wandering_angle += utl::random::GetRandom(-0.05, 0.05);
		target_angle = wandering_angle;
		target += utl::math::from_angel(wandering_angle) * stats.wandering_distence;
	}
	dragon(glm::vec2 pos) {

		target = glm::vec2(0);
		vel = glm::vec2(0);
		wandering_angle = utl::random::GetRandom(-3.14999, 3.14999);
		wandering_angle += utl::random::GetRandom(-0.05, 0.05);
		target_angle = wandering_angle;
		target += utl::math::from_angel(wandering_angle) * stats.wandering_distence;
		body = dragon_body(pos);
	}
	void update(std::vector<glm::vec4> aabbs) {
		handleinput();
		behave(aabbs);
		vel = utl::MoveTowards(vel, glm::normalize(target - body.ch.joints[0]) * stats.move_speed * gbl::dt, stats.acc * gbl::dt);
			//body.ch.joints[0] = utl::MoveTowards(body.ch.joints[0],target,stats.move_speed * gbl::dt);
		body.ch.joints[0] += vel;
		vel = { 0, 0 };
		body.update();
	}
	void render(vx::renderer& rend) {
		body.render(rend, target);
		//body.render_mer(rend);
	}
};

class snake {
	struct {
		bool finished;
		bool started;
		bool winded;
		float cooldown = 1;
		float attak = 10;
		float used = -1;
		float windup = -1;
		float unleach = -1;
	}attack0;
	glm::vec2 vel;
	bool canattack;
	struct
	{
		float move_speed = 600.f;
	}stats;
	snake_body body;
	void handleinput() {
		auto s = inputhandler.GetJoystickState(GLFW_JOYSTICK_1);
		if (s.pres)
		{
			if (s.isgpad) {
				if (s.state.buttons[GLFW_GAMEPAD_BUTTON_X]) {
					attack0.started = true;
					attack0.used = gbl::time;
				}
				glm::vec2 j = glm::vec2{ s.state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], s.state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] };
				vel = (glm::length(j) > 1 ? glm::normalize(j) : j) * stats.move_speed * gbl::dt;
			}
		}
	}
	void update_animation() {
		if (attack0.started) {

		}
	}
public:
	void update() {
		handleinput();
		body.ch.joints[0] = inputhandler.GetCursorPos(); vel = {0, 0};
		body.update();
	}
	void render(vx::renderer & rend) {
		body.render(rend);
	}
};