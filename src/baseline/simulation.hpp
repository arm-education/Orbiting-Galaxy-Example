#pragma once

#include "particle.hpp"

void update_positions(Particle* const* p, int n, float dt);
void init_galaxy(ParticleOwner& p, int n);
