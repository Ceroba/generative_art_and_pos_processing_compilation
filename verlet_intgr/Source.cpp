#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include <SFML/System/Vector2.hpp>

#include <random>

struct Math
{
    static constexpr float PI = 3.1415936f;

    static sf::Vector2f dot(sf::Vector2f v1, sf::Vector2f v2)
    {
        return { v1.x * v2.x, v1.y * v2.y };
    }
};


class NumberGenerator
{
protected:
    std::random_device rd;
    std::mt19937 gen;

    NumberGenerator()
        : gen(rd())
    {}
};


template<typename T>
class RealNumberGenerator : public NumberGenerator
{
private:
    std::uniform_real_distribution<T> dis;

public:
    RealNumberGenerator()
        : NumberGenerator()
        , dis(0.0f, 1.0f)
    {}

    // random_device is not copyable
    RealNumberGenerator(const RealNumberGenerator<T>& right)
        : NumberGenerator()
        , dis(right.dis)
    {}

    float get()
    {
        return dis(gen);
    }

    float getUnder(T max)
    {
        return get() * max;
    }

    float getRange(T min, T max)
    {
        return min + get() * (max - min);
    }

    float getRange(T width)
    {
        return getRange(-width * 0.5f, width * 0.5f);
    }
};


template<typename T>
class RNG
{
private:
    static RealNumberGenerator<T> gen;

public:
    static T get()
    {
        return gen.get();
    }

    static float getUnder(T max)
    {
        return gen.getUnder(max);
    }

    static uint64_t getUintUnder(uint64_t max)
    {
        return static_cast<uint64_t>(gen.getUnder(static_cast<float>(max) + 1.0f));
    }

    static float getRange(T min, T max)
    {
        return gen.getRange(min, max);
    }

    static float getRange(T width)
    {
        return gen.getRange(width);
    }

    static float getFullRange(T width)
    {
        return gen.getRange(static_cast<T>(2.0f) * width);
    }

    static bool proba(float threshold)
    {
        return get() < threshold;
    }
};

using RNGf = RNG<float>;

template<typename T>
RealNumberGenerator<T> RNG<T>::gen = RealNumberGenerator<T>();


template<typename T>
class IntegerNumberGenerator : public NumberGenerator
{
public:
    IntegerNumberGenerator()
        : NumberGenerator()
    {}

    // random_device is not copyable
    IntegerNumberGenerator(const IntegerNumberGenerator<T>& right)
        : NumberGenerator()
    {}

    T getUnder(T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, max);
        return dist(gen);
    }

    T getRange(T min, T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(gen);
    }
};


template<typename T>
class RNGi
{
private:
    static IntegerNumberGenerator<T> gen;

public:
    static T getUnder(T max)
    {
        return gen.getUnder(max);
    }

    static T getRange(T min, T max)
    {
        return gen.getRange(min, max);
    }
};

template<typename T>
IntegerNumberGenerator<T> RNGi<T>::gen;

using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;
struct VerletObject
{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float        radius = 10.0f;
    sf::Color    color = sf::Color::White;

    VerletObject() = default;
    VerletObject(sf::Vector2f position_, float radius_)
        : position{ position_ }
        , position_last{ position_ }
        , acceleration{ 0.0f, 0.0f }
        , radius{ radius_ }
    {}

    void update(float dt)
    {
        const sf::Vector2f displacement = position - position_last;
        position_last = position;
        position = position + displacement + acceleration * (dt * dt);
        acceleration = {};
    }

    void accelerate(sf::Vector2f a)
    {
        acceleration += a;
    }

    void setVelocity(sf::Vector2f v, float dt)
    {
        position_last = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt)
    {
        position_last -= v * dt;
    }

    [[nodiscard]]
    sf::Vector2f getVelocity(float dt) const
    {
        return (position - position_last) / dt;
    }
};


class Solver
{
public:
    std::vector<VerletObject> m_objects;
    Solver() = default;

    //VerletObject& addObject(sf::Vector2f position, float radius)
    //{
    //    return (m_objects.emplace_back(position, radius));
    //}

    void update()
    {
        m_time += m_frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{ m_sub_steps }; i--;) {
            applyGravity();
            checkCollisions(step_dt);
            applyConstraint();
            updateObjects(step_dt);
        }
    }    


    void setSimulationUpdateRate(uint32_t rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setConstraint(sf::Vector2f position, float radius)
    {
        m_constraint_center = position;
        m_constraint_radius = radius;
    }

    void setSubStepsCount(uint32_t sub_steps)
    {
        m_sub_steps = sub_steps;
    }

    void setObjectVelocity(VerletObject& object, sf::Vector2f v)
    {
        object.setVelocity(v, getStepDt());
    }

    [[nodiscard]]
    const std::vector<VerletObject>& getObjects() const
    {
        return m_objects;
    }

    [[nodiscard]]
    sf::Vector3f getConstraint() const
    {
        return { m_constraint_center.x, m_constraint_center.y, m_constraint_radius };
    }

    [[nodiscard]]
    uint64_t getObjectsCount() const
    {
        return m_objects.size();
    }

    [[nodiscard]]
    float getTime() const
    {
        return m_time;
    }

    [[nodiscard]]
    float getStepDt() const
    {
        return m_frame_dt / static_cast<float>(m_sub_steps);
    }

private:
    uint32_t m_sub_steps = 1;
    sf::Vector2f m_gravity = { 0.0f, 1000.0f };
    sf::Vector2f m_constraint_center;
    float m_constraint_radius = 100.0f;
    float m_time = 0.0f;
    float m_frame_dt = 0.0f;

    void applyGravity()
    {
        for (auto& obj : m_objects) {
            obj.accelerate(m_gravity);
        }
    }

    void checkCollisions(float dt)
    {
        const float    response_coef = 0.75f;
        const uint64_t objects_count = m_objects.size();
        for (uint64_t i{ 0 }; i < objects_count; ++i) {
            VerletObject& object_1 = m_objects[i];
            for (uint64_t k{ i + 1 }; k < objects_count; ++k) {
                VerletObject& object_2 = m_objects[k];
                const sf::Vector2f v = object_1.position - object_2.position;
                const float dist2 = v.x * v.x + v.y * v.y;
                const float min_dist = object_1.radius + object_2.radius;
                if (dist2 < min_dist * min_dist) {
                    const float dist = sqrt(dist2);
                    const sf::Vector2f n = v / dist;
                    const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                    const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                    const float delta = 0.5f * response_coef * (dist - min_dist);
                    object_1.position -= n * (mass_ratio_2 * delta);
                    object_2.position += n * (mass_ratio_1 * delta);
                }
            }
        }
    }

    void applyConstraint()
    {
        for (auto& obj : m_objects) {
            const sf::Vector2f v = m_constraint_center - obj.position;
            const float dist = sqrt(v.x * v.x + v.y * v.y);
            if (dist > (m_constraint_radius - obj.radius)) {
                const sf::Vector2f n = v / dist;
                obj.position = m_constraint_center - n * (m_constraint_radius - obj.radius);
            }
        }
    }

    void updateObjects(float dt)
    {
        for (auto& obj : m_objects) {
            obj.update(dt);
        }
    }
};

class Renderer
{
public:
    explicit
        Renderer(sf::RenderTarget& target)
        : m_target{ target }
    {

    }

    void render(const Solver& solver) const
    {
        const sf::Vector3f constraint = solver.getConstraint();
        sf::CircleShape constraint_background{ constraint.z };
        constraint_background.setOrigin(constraint.z, constraint.z);
        constraint_background.setFillColor(sf::Color::Black);
        constraint_background.setPosition(constraint.x, constraint.y);
        constraint_background.setPointCount(128);
        m_target.draw(constraint_background);
        sf::CircleShape circle{ 1.0f };
        circle.setPointCount(32);
        circle.setOrigin(1.0f, 1.0f);
        const auto& objects = solver.getObjects();
        for (const auto& obj : objects) {
            circle.setPosition(obj.position);
            circle.setScale(obj.radius, obj.radius);
            circle.setFillColor(obj.color);
            m_target.draw(circle);
        }
    }

private:
    sf::RenderTarget& m_target;
};
#include <iostream>
#include <SFML/Graphics.hpp>



static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * Math::PI);
    const float b = sin(t + 0.66f * 2.0f * Math::PI);
    return { static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b) };
}


int32_t main(int32_t, char* [])
{
    constexpr int32_t window_width = 1000;
    constexpr int32_t window_height = 1000;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Verlet", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Solver   solver;
    Renderer renderer{ window };

    solver.setConstraint({ static_cast<float>(window_width) * 0.5f, static_cast<float>(window_height) * 0.5f }, 450.0f);
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    const float object_spawn_delay = 0.025f;
    const float object_spawn_speed = 1200.0f;
    const sf::Vector2f object_spawn_position = { 500.0f, 200.0f };
    const float object_min_radius = 1.0f;
    const float object_max_radius = 20.0f;
    const uint32_t max_objects_count = 3000;
    const float max_angle = 1.0f;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        if (solver.getObjectsCount() < max_objects_count && clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            solver.m_objects.emplace_back(object_spawn_position, RNGf::getRange(object_min_radius, object_max_radius));
            VerletObject& object = solver.m_objects.back();
            const float t = solver.getTime();
            const float angle = max_angle * sin(t) + Math::PI * 0.5f;
            solver.setObjectVelocity(object, object_spawn_speed * sf::Vector2f{ cos(angle), sin(angle) });
            object.color = getRainbow(t);
        }
        //if (solver.getObjectsCount() < max_objects_count && clock.getElapsedTime().asSeconds() >= object_spawn_delay && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        //    clock.restart();
        //    solver.m_objects.emplace_back(object_spawn_position, RNGf::getRange(object_min_radius, object_max_radius));
        //    VerletObject& object = solver.m_objects.back();
        //    const float t = solver.getTime();
        //    const float angle = max_angle * sin(t) + Math::PI * 0.5f;
        //    solver.setObjectVelocity(object, object_spawn_speed * sf::Vector2f{ cos(angle), sin(angle) });
        //    object.color = getRainbow(t);
        //}
        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }

    return 0;
}