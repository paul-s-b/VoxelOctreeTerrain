#pragma once
#include <CL/cl.hpp>

class CLW
{
public:
    CLW();
    ~CLW();
    static void Init();
    void CLNoise(int landmap_flags[68 * 68 * 68],
                 cl_float3 pos, 
                 cl_int LOD, 
                 cl_int chunkSize);
    void CLMesh(int landmap_flags[68 * 68 * 68],
                cl_float3 pos, 
                cl_int LOD, 
                cl_int chunkSize, 
                cl_float3 *trianges, 
                cl_float2 *uvs, 
                cl_float3 *normals, 
                cl_float3 *indices,
                cl_float _tex_atlas_width,
                cl_int *nrOfIndices);

private:

    static cl::Buffer buffer_landmap;
    static cl::Kernel terrain_gen;
    static cl::CommandQueue queue;
};

