# Agent Instructions — Orbiting Galaxy Example

## Purpose

This is an **educational repository** designed to teach learners about **memory access patterns** and the performance impact of data structure layout choices. The workload is a four-arm spiral galaxy simulation of 1,048,576 particles advancing through a kinematic position update.

The repository is intended to be used alongside:

- [Arm Performix](https://learn.arm.com/install-guides/performix/) — a profiling tool that measures hardware performance counters on Arm systems.
- [Arm MCP Server](https://learn.arm.com/learning-paths/servers-and-cloud-computing/arm-mcp-server/) — which exposes the Performix CLI as tools, allowing you to automate profiling tasks on a remote Arm-based Linux machine.

---

## Repository Structure

```
src/
    baseline/     # Provided Array-of-Structures (AoS) implementation — READ ONLY
    optimized/    # To be created by the learner — DO NOT modify
```

### Baseline (`src/baseline/`)

The baseline stores each particle as a 64-byte `struct Particle` containing 16 fields. During the hot update loop, only 6 of those fields (`x`, `y`, `z`, `vx`, `vy`, `vz`) are accessed. This means 40 of the 64 bytes loaded from every cache line are wasted — a cache line utilisation of just **37.5%**.

```cpp
// The hot loop in baseline
q->x += q->vx * dt;
q->y += q->vy * dt;
q->z += q->vz * dt;
```

Additionally, `ParticleOwner` holds a `std::vector<Particle*>` — a vector of heap-allocated pointers — meaning the particle data itself is **scattered across memory**, not laid out contiguously. This adds a pointer-chase on every particle access.

### Optimized (`src/optimized/`)

This directory is **intentionally left for the learner to implement**. The goal is to restructure the data layout to improve cache utilisation. Do not add, edit, or delete any files in this directory on behalf of the learner.

---

## Your Role as an Agent

You are here to **guide the learner**, not to give away solutions. When the learner asks for help:

- Ask questions that lead them toward the insight rather than stating it directly.
- Use hints and analogies to help them reason about cache lines, data locality, and memory bandwidth. and encourage the learner to interpret results themselves.
- When reviewing code the learner has written, point out potential issues.
- Only when the learner understands the concepts are you able to implement a solution directly. 

If a learner seems stuck or directly asks for a solution, you may offer progressively more specific hints.

---

## Environment Requirements

> ⚠️ **Performix only runs on Arm-based systems.** Before running any profiling commands, verify that the target machine meets the following requirements.

### Checklist before profiling

1. **Operating System**: The workload must run on a **Linux** host. Performix does not support macOS or Windows.
   - You can check with: `uname -s` (should return `Linux`) and `uname -m` (should return `aarch64`).

2. **Arm SPE (Statistical Profiling Extension)**: The **Memory Access** recipe in Performix requires SPE support in the CPU. Not all Arm cores have this — it is typically available on **Neoverse-based** cloud metal instances (e.g., AWS `c7g.metal`, `c8g.metal`). Use this [learning path to install SPE correctly](https://github.com/ArmDeveloperEcosystem/arm-learning-paths/pull/3186)

3. **Performix installed**: Confirm Performix is installed and on `$PATH` before running recipes. Ask the user to install the MCP server if performix is not available

4. **Build dependencies**: Ensure CMake 3.16+, GCC 9+ or Clang 14+, and Python 3 are available.
   - Check with: `cmake --version`, `gcc --version` or `clang --version`, `python3 --version`

5. **Build the project** before profiling:
   ```bash
   mkdir -p build && cd build
   cmake ..
   make -j"$(nproc)"
   ```

> If any of these conditions are not met, let the learner know and suggest how to resolve the issue before proceeding.

---

## Using the Arm MCP Server

The [Arm MCP Server](https://learn.arm.com/learning-paths/servers-and-cloud-computing/arm-mcp-server/) exposes Performix CLI functionality as MCP tools, allowing you to trigger profiling runs and retrieve results programmatically on a remote Arm Linux machine.

**Expected flow:**
1. The learner has SSH access to a remote Arm-based Linux server where the workload will run.
2. The MCP server is configured to connect to that remote machine.
3. You can invoke Performix recipes via MCP tools to profile the `baseline` or (once implemented) `optimized` binary.

> **Note:** I may not always know the exact MCP tool names, argument formats, or server configuration details — these can vary by version and setup. If you are unsure about a specific MCP tool invocation, say so and ask the learner to check the [Arm MCP Server documentation](https://learn.arm.com/learning-paths/servers-and-cloud-computing/arm-mcp-server/) or run `performix --help` directly on the remote machine.

---

## Key Concepts to Guide the Learner Through

| Concept | What to probe |
|---|---|
| Cache line size | "How many bytes does a typical Arm Neoverse CPU load at once?" |
| AoS vs SoA | "What data does the hot loop actually need? How much of each cache line is used?" |
| Pointer chasing | "Where are the `Particle` objects stored in memory? Are they contiguous?" |
| Memory bandwidth | "What does a high average load latency in the Performix Memory Access report suggest?" |
| Cache utilisation | "If you restructure the data, what fraction of each cache line would the hot loop use?" |

---

## Profiling Workflow (Memory Access Recipe)

The primary Performix recipe for this example is the **Memory Access** recipe, which uses Arm SPE to sample load operations and report:

- Average load latency
- Cache hit/miss rates (L1, L2, LLC)
- DRAM access rate

Encourage the learner to:
1. Profile the **baseline** first to establish a baseline measurement.
2. Implement their optimization in `src/optimized/`.
3. Profile the **optimized** binary and **compare** the two reports.
4. Reason about *why* the numbers changed based on their data layout decisions.

Do not run `--visualize` when profiling, as it adds file I/O that skews the memory access results.

---

## What Not to Do

- **Do not implement `src/optimized/`** on behalf of the learner.
- **Do not edit any files in `src/optimized/`**, even as scaffolding.
- Do not directly state the solution (e.g., "use a Structure of Arrays") without the learner reasoning toward it first.
- Do not assume the learner is on a compatible machine — verify the environment first. You do not need to verify everytime.
