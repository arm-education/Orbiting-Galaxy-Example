#include "particle.hpp"
#include "simulation.hpp"

#include <gtest/gtest.h>

#include <cmath>

TEST(ParticleOwnerTest, AllocatesRequestedParticles) {
    ParticleOwner particles(4);

    EXPECT_EQ(4, particles.size());
    ASSERT_NE(nullptr, particles.data());

    for (int i = 0; i < particles.size(); ++i) {
        EXPECT_NE(nullptr, particles[i]);
        EXPECT_EQ(particles[i], particles.data()[i]);
    }

    EXPECT_NE(particles[0], particles[1]);
}

TEST(UpdatePositionsTest, AdvancesPositionsUsingVelocityAndDeltaTime) {
    Particle first{};
    first.x = 1.0f;
    first.y = -2.0f;
    first.z = 0.5f;
    first.vx = 4.0f;
    first.vy = -8.0f;
    first.vz = 2.0f;
    first.mass = 42.0f;

    Particle second{};
    second.x = -3.0f;
    second.y = 2.5f;
    second.z = 10.0f;
    second.vx = -2.0f;
    second.vy = 0.5f;
    second.vz = -4.0f;
    second.mass = 7.0f;

    Particle* particles[] = {&first, &second};

    update_positions(particles, 2, 0.25f);

    EXPECT_FLOAT_EQ(2.0f, first.x);
    EXPECT_FLOAT_EQ(-4.0f, first.y);
    EXPECT_FLOAT_EQ(1.0f, first.z);
    EXPECT_FLOAT_EQ(42.0f, first.mass);

    EXPECT_FLOAT_EQ(-3.5f, second.x);
    EXPECT_FLOAT_EQ(2.625f, second.y);
    EXPECT_FLOAT_EQ(9.0f, second.z);
    EXPECT_FLOAT_EQ(7.0f, second.mass);
}

TEST(UpdatePositionsTest, HandlesEmptyParticleRange) {
    update_positions(nullptr, 0, 1.0f);
}

TEST(InitGalaxyTest, InitializesExpectedParticleFields) {
    ParticleOwner particles(16);

    init_galaxy(particles, particles.size());

    for (int i = 0; i < particles.size(); ++i) {
        const Particle* particle = particles[i];
        const float radius =
            std::sqrt(particle->x * particle->x + particle->y * particle->y);

        EXPECT_TRUE(std::isfinite(particle->x));
        EXPECT_TRUE(std::isfinite(particle->y));
        EXPECT_TRUE(std::isfinite(particle->z));
        EXPECT_TRUE(std::isfinite(particle->vx));
        EXPECT_TRUE(std::isfinite(particle->vy));
        EXPECT_FLOAT_EQ(0.0f, particle->vz);

        EXPECT_GE(radius, 0.5f);
        EXPECT_LE(radius, 9.0f);

        EXPECT_FLOAT_EQ(1.0f, particle->mass);
        EXPECT_FLOAT_EQ(0.5f, particle->charge);
        EXPECT_FLOAT_EQ(300.0f, particle->temperature);
        EXPECT_FLOAT_EQ(101325.0f, particle->pressure);
        EXPECT_FLOAT_EQ(0.0f, particle->energy);
        EXPECT_FLOAT_EQ(1.0f, particle->density);
        EXPECT_FLOAT_EQ(0.0f, particle->spin_x);
        EXPECT_FLOAT_EQ(0.0f, particle->spin_y);
        EXPECT_FLOAT_EQ(0.0f, particle->spin_z);
        EXPECT_FLOAT_EQ(0.0f, particle->pad);
    }
}
