#ifndef CLW_H_
#define CLW_H_

#include <CL/cl.hpp>
#include <iostream>
#include <vector>

class CLW {
public:
    static void init();
    void CLNoise(int *landmap_flags,
                 cl_float3 pos, cl_int LOD, cl_int chunkSize);
    void CLMesh(int landmap_flags[68 * 68 * 68], 
                cl_float3 triangle_flags[12 * 64 * 64 * 64], 
                cl_float3 normal_flags[12 * 64 * 64 * 64], 
                cl_float3 index_flags[6 * 64 * 64 * 64],
                cl_float3 pos, cl_int LOD, cl_int chunkSize);
    
    static cl::Buffer buffer_landmap;
    static cl::Kernel terrain_gen;
    
    static cl::Buffer buffer_triangles;
    static cl::Buffer buffer_normals;
    static cl::Buffer buffer_indices;
    static cl::Kernel mesh_gen;
    
    static cl::CommandQueue queue;
};

#endif // CLW_H_