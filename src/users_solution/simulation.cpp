#include "simulation.hpp"

#include <cmath>
#include <random>

static std::mt19937 rng(0x12345678u);
static std::uniform_real_distribution<float> uniform01(0.0f, 1.0f);
static std::normal_distribution<float> normal01(0.0f, 1.0f);

static float lcg_float() { return uniform01(rng); }
static float lcg_gauss() { return normal01(rng); }

void update_positions(Particle* const* p, int n, float dt) {
    for (int i = 0; i < n; ++i) {
        Particle* q = p[i];
        q->x += q->vx * dt;
        q->y += q->vy * dt;
        q->z += q->vz * dt;
    }
}

void init_galaxy(ParticleOwner& p, int n) {
    const float PI = 3.14159265f;
    const float v0 = 2.0f;
    const float winding = 3.5f;
    const float r_min = 0.5f;
    const float r_scale = 2.2f;
    const float r_max = 9.0f;
    const float scatter = 0.30f;
    const float z_scale = 0.15f;

    for (int i = 0; i < n; ++i) {
        float arm_offset = (i % 4) * (PI / 2.0f);

        float r = r_min - r_scale * logf(lcg_float() + 1e-7f);
        if (r > r_max) r = r_min + (r_max - r_min) * lcg_float();

        float theta = arm_offset + winding * logf(r / r_min) + lcg_gauss() * scatter;

        p[i]->x = r * cosf(theta);
        p[i]->y = r * sinf(theta);
        p[i]->z = lcg_gauss() * z_scale;

        p[i]->vx = -v0 * sinf(theta);
        p[i]->vy = v0 * cosf(theta);
        p[i]->vz = 0.0f;

        p[i]->mass = 1.0f;
        p[i]->charge = 0.5f;
        p[i]->temperature = 300.0f;
        p[i]->pressure = 101325.0f;
        p[i]->energy = 0.0f;
        p[i]->density = 1.0f;
        p[i]->spin_x = 0.0f;
        p[i]->spin_y = 0.0f;
        p[i]->spin_z = 0.0f;
        p[i]->pad = 0.0f;
    }
}
