/***
#include "SolverGPU.cuh"
#include "Common.cuh"
#include "Common.hpp"
#include "Core/Timestep.h"

__device__ __constant__ SimParams deviceSimParams;
SimParams hostSimParams;

__device__ inline void AtomicAdd(glm::vec3 *address, int index, glm::vec3 val, int reorder) {
    const int r1 = reorder % 3;
    const int r2 = (reorder + 1) % 3;
    const int r3 = (reorder + 2) % 3;
    atomicAdd(&(address[index].x) + r1, val[r1]);
    atomicAdd(&(address[index].x) + r2, val[r2]);
    atomicAdd(&(address[index].x) + r3, val[r3]);
}

void SetSimParams(SimParams* hostParams)
{
    ScopedTimerGPU timer("Solver_SetParams");
    checkCudaErrors(cudaMemcpyToSymbolAsync(d_params, hostParams, sizeof(VtSimParams)));
    h_params = *hostParams;
}
***/