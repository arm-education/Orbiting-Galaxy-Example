#include "scopedTimer.hpp"
#include "particle.hpp"
#include "simulation.hpp"
#include "visualization.hpp"

#include <cstring>
#include <iostream>


int main(int argc, char* argv[]) {

    const int NUM_PARTICLES = 1 << 20; // 1,048,576
    const int DEFAULT_ITERS = 200;
    const int VIS_ITERS = 1000;
    const float dt = 0.005f; // time delta

    bool do_vis = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--visualize") == 0) {
            do_vis = true;
            break;
        }
    }

    const int iters = do_vis ? VIS_ITERS : DEFAULT_ITERS;

    const int vis_stride = 16;
    const int vis_interval = 10;
    ParticleOwner particles(NUM_PARTICLES);

    init_galaxy(particles, NUM_PARTICLES);

    BinaryWriter writer(do_vis,
                        "galaxy_baseline.bin",
                                 NUM_PARTICLES,
                                 iters,
                                 vis_stride,
                                 vis_interval);

    writer.dump_frame(particles);

    {
        ScopedTimer timer("Baseline");

        for (int iter = 0; iter < iters; ++iter) {
            update_positions(particles.data(), NUM_PARTICLES, dt);
    
            if (writer.enabled() && (iter + 1) % writer.interval() == 0)
                writer.dump_frame(particles);
        }
        
    }
    
    return 0;
}
