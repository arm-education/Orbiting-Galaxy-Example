#pragma once

#include <vector>

struct ParticlesSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass, charge, temperature;
    std::vector<float> pressure, energy, density;
    std::vector<float> spin_x, spin_y, spin_z;

    explicit ParticlesSoA(int n)
        : x(n), y(n), z(n),
          vx(n), vy(n), vz(n),
          mass(n), charge(n), temperature(n),
          pressure(n), energy(n), density(n),
          spin_x(n), spin_y(n), spin_z(n) {}

    int size() const { return static_cast<int>(x.size()); }
};
