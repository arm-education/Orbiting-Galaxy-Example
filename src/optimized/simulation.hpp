#pragma once

#include "particle.hpp"

void update_positions(ParticlesSoA& p, int n, float dt);
void init_galaxy(ParticlesSoA& p, int n);
