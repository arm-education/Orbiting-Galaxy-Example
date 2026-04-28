#include "simulation.hpp"

#include <cmath>
#include <random>

static std::mt19937 rng(0x12345678u);
static std::uniform_real_distribution<float> uniform01(0.0f, 1.0f);
static std::normal_distribution<float> normal01(0.0f, 1.0f);

static float lcg_float() { return uniform01(rng); }
static float lcg_gauss() { return normal01(rng); }

void update_positions(ParticlesSoA& p, int n, float dt) {
    for (int i = 0; i < n; ++i) {
        p.x[i] += p.vx[i] * dt;
        p.y[i] += p.vy[i] * dt;
        p.z[i] += p.vz[i] * dt;
    }
}

void init_galaxy(ParticlesSoA& p, int n) {
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

        p.x[i] = r * cosf(theta);
        p.y[i] = r * sinf(theta);
        p.z[i] = lcg_gauss() * z_scale;

        p.vx[i] = -v0 * sinf(theta);
        p.vy[i] = v0 * cosf(theta);
        p.vz[i] = 0.0f;

        p.mass[i] = 1.0f;
        p.charge[i] = 0.5f;
        p.temperature[i] = 300.0f;
        p.pressure[i] = 101325.0f;
        p.energy[i] = 0.0f;
        p.density[i] = 1.0f;
        p.spin_x[i] = 0.0f;
        p.spin_y[i] = 0.0f;
        p.spin_z[i] = 0.0f;
    }
}
