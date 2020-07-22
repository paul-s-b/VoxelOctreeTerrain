#ifndef CLW_H_
#define CLW_H_

#include <CL/cl.hpp>
#include <iostream>
#include <vector>

class CLW {
public:
    CLW();
    ~CLW();
    static void init();
    void CLNoise(int landmap_flags[68 * 68 * 68],
        cl_float3 pos,
        cl_int LOD,
        cl_int chunkSize);
    void CLMesh(int landmap_flags[68 * 68 * 68],
        cl_float3 pos,
        cl_int LOD,
        cl_int chunkSize,
        cl_float3* trianges,
        cl_float2* uvs,
        cl_float3* normals,
        cl_float3* indices,
        cl_float _tex_atlas_width,
        cl_int* nrOfIndices);
    static cl::Buffer buffer_landmap;
    static cl::Kernel terrain_gen;
    static cl::CommandQueue queue;
};

#endif // CLW_H_