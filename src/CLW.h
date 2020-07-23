#ifndef CLW_H_
#define CLW_H_

/*#pragma OPENCL EXTENSION cl_khr_gl_sharing : enable (inside of kernel)*/

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
    static cl::Buffer buffer_landmap;
    static cl::Kernel terrain_gen;
    static cl::CommandQueue queue;
};

#endif // CLW_H_