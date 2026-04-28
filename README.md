# Orbiting Galaxy Example

This project demonstrates the performance impact of data layout choices in memory-bound workloads. It uses a simulated four-arm spiral galaxy as the workload, where one million particles evolve over time according to a simple kinematic update. The baseline implementation is provided, and learners create the optimized implementation as part of the course.

<p align="center">
    <img src="./assets/galaxy_compressed.gif" alt="Galaxy simulation animation" />
</p>

<p align="center"><em>Galaxy evolution over 1,000 visualisation iterations. Inner particles orbit faster, causing the spiral arms to wind up over time. This GIF is lossy-compressed for repository size, so minor visual artifacts are expected.</em></p>

---

## Overview

The simulation advances 1,048,576 particles over 200 iterations. Each iteration applies the following update to every particle:

```cpp
p[i].x += p[i].vx * dt;
p[i].y += p[i].vy * dt;
p[i].z += p[i].vz * dt;
```

The algorithm is intentionally simple: three multiply-adds per particle with no branching and no inter-particle dependencies. The performance difference between the two implementations is driven entirely by how particle data is arranged in memory.

### Baseline: Array of Structures (`baseline`)

Each particle is stored as a 64-byte struct containing 16 fields. The hot update loop only reads and writes 6 of those fields (24 bytes). The remaining 40 bytes are loaded into cache on every access but never used, resulting in 37.5% cache line utilisation.

### Optimised: Structure of Arrays (`optimized`)

Position and velocity data are stored in separate contiguous arrays. The hot update loop touches only those arrays, so every byte loaded from cache is useful data, achieving 100% cache line utilisation. In this course, learners create this implementation in `src/optimized/`.

---

## Prerequisites

- An Arm Neoverse-based Cloud `metal` instance (e.g., AWS `c7g.metal`)
- GCC 9+ or Clang 14+
- CMake 3.16+
- [Arm Performix](https://developer.arm.com/servers-and-cloud-computing/arm-performix) installed and configured with SPE
- Python 3

---

## Build

```bash
mkdir -p build
cd build
cmake ..
make -j"$(nproc)"
```

---

## Run

To generate a visualisation of the galaxy simulation, pass the `--visualize` flag:

```bash
./build/baseline --visualize

python3 -m venv .venv
source .venv/bin/activate
pip install -r scripts/requirements.txt

python3 scripts/visualize.py galaxy_baseline.bin
```

The script reads position snapshots from `galaxy_baseline.bin` (or `galaxy_optimized.bin` if you run `./build/optimized --visualize`) and writes an animated GIF to `assets/`.

Do not use `--visualize` when profiling with Performix, as it adds file I/O that is not part of the workload under measurement.

---

## Profiling with Arm Performix

This example is intended to be used with the Performix **Memory Access** recipe:

1. **Memory Access** - measures cache hit rates and average load latency using the Arm Statistical Profiling Extension (SPE).

---

## Project Structure

```
src/
    baseline/             # Provided Array-of-Structures implementation
        baseline.cpp
        simulation.cpp
        simulation.hpp
        particle.hpp
        scopedTimer.hpp
        visualization.hpp
    optimized/            # Created by learner during the course
        optimized.cpp
        simulation.cpp
        simulation.hpp
        particle.hpp
        scopedTimer.hpp
        visualization.hpp
scripts/
    visualize.py          # Generates animated GIF from position snapshots
    gen_memory_layout_gif.py
    requirements.txt
assets/
    galaxy_compressed.gif # Example animation of the galaxy simulation
CMakeLists.txt
```

---

## License

This project is licensed under the Arm Education End User License Agreement for Teaching and Learning Content. It is provided for non-commercial educational purposes only. See [License.md](License.md) for the full terms.

For commercial use enquiries, contact education@arm.com.
