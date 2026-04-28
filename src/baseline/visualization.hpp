#pragma once

#include <fstream>

#include "particle.hpp"

class BinaryWriter {
public:
    BinaryWriter(bool enabled,
                 const char* path,
                 int num_particles,
                 int iters,
                 int vis_stride,
                 int vis_interval)
        : enabled_(enabled),
          num_particles_(num_particles),
          vis_stride_(vis_stride),
          vis_interval_(vis_interval) {
        if (!enabled_) return;

        const int vis_n = num_particles_ / vis_stride_;
        const int vis_frames = 1 + iters / vis_interval_;

        fp_.open(path, std::ios::binary);
        fp_.write(reinterpret_cast<const char*>(&vis_n), sizeof(int));
        fp_.write(reinterpret_cast<const char*>(&vis_frames), sizeof(int));
    }

    bool enabled() const { return enabled_; }
    int interval() const { return vis_interval_; }

    void dump_frame(const ParticleOwner& particles) {
        if (!enabled_ || !fp_.is_open()) return;

        for (int j = 0; j < num_particles_; j += vis_stride_)
            fp_.write(reinterpret_cast<const char*>(&particles[j]->x), sizeof(float));
        for (int j = 0; j < num_particles_; j += vis_stride_)
            fp_.write(reinterpret_cast<const char*>(&particles[j]->y), sizeof(float));
        for (int j = 0; j < num_particles_; j += vis_stride_)
            fp_.write(reinterpret_cast<const char*>(&particles[j]->z), sizeof(float));
    }

private:
    bool enabled_;
    int num_particles_;
    int vis_stride_;
    int vis_interval_;
    std::ofstream fp_;
};
