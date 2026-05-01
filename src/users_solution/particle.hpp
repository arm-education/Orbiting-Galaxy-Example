#pragma once

#include <vector>

// Learner note:
// Edit this data structure to experiment with memory layout.
// If you change `Particle` or `ParticleOwner`, also update associated function
// signatures and call sites in simulation/visualization code.
struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float mass, charge, temperature;
    float pressure, energy, density;
    float spin_x, spin_y, spin_z;
    float pad;
};

static_assert(sizeof(Particle) == 64, "Particle must remain 64 bytes.");

class ParticleOwner {
public:
    explicit ParticleOwner(int n) : particles_(n) {
        for (int i = 0; i < n; ++i) {
            particles_[i] = new Particle();
        }
    }

    ~ParticleOwner() {
        for (Particle* particle : particles_) {
            delete particle;
        }
    }

    ParticleOwner(const ParticleOwner&) = delete;
    ParticleOwner& operator=(const ParticleOwner&) = delete;
    ParticleOwner(ParticleOwner&&) = delete;
    ParticleOwner& operator=(ParticleOwner&&) = delete;

    int size() const { return static_cast<int>(particles_.size()); }
    Particle** data() { return particles_.data(); }
    Particle* operator[](int index) { return particles_[index]; }
    const Particle* operator[](int index) const { return particles_[index]; }

private:
    std::vector<Particle*> particles_;
};
