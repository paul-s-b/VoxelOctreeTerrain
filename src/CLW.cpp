#include "CLW.h"

cl::Buffer CLW::buffer_landmap;
cl::Kernel CLW::terrain_gen;
cl::CommandQueue CLW::queue;

void CLW::init() {
    std::vector<cl::Platform> all_platforms;

    cl::Platform::get(&all_platforms);

    if (all_platforms.size() == 0) {
        std::cout << " No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> all_devices;
    
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() == 0) {
        std::cout << " No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

    cl::Context context({ default_device });
    cl::Program::Sources sources;

    std::string noise_kernel_code =
    "__constant int perm[512] = {151,160,137,91,90,15,"
    "    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,"
    "    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,"
    "    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,"
    "    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,"
    "    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,"
    "    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,"
    "    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,"
    "    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,"
    "    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,"
    "    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,"
    "    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,"
    "    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,"
    "    151,160,137,91,90,15,"
    "    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,"
    "    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,"
    "    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,"
    "    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,"
    "    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,"
    "    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,"
    "    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,"
    "    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,"
    "    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,"
    "    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,"
    "    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,"
    "    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180"
    "};"
    "    "
    "static float  grad2( int hash, float x, float y ) {"
    "    int h = hash & 7;      /* Convert low 3 bits of hash code */"
    "    float u = h<4 ? x : y;  /* into 8 simple gradient directions, */"
    "    float v = h<4 ? y : x;  /* and compute the dot product with (x,y). */"
    "    return ((h&1)? -u : u) + ((h&2)? -2.0f*v : 2.0f*v);"
    "}"
    "/* 2D simplex noise */"
    "static float _slang_library_noise2 (float x, float y)"
    "{"
    "    "
    "    float n0;"
    "    float n1;"
    "    float n2; /* Noise contributions from the three corners */"
    "    "
    "    /* Skew the input space to determine which simplex cell we're in */"
    "    float s = (x+y)*0.366025403f; /* Hairy factor for 2D */"
    "    float xs = x + s;"
    "    float ys = y + s;"
    "    int i = floor(xs);"
    "    int j = floor(ys);"
    "    "
    "    float t = (float)(i+j)*0.211324865f;"
    "    float X0 = i-t; /* Unskew the cell origin back to (x,y) space */"
    "    float Y0 = j-t;"
    "    float x0 = x-X0; /* The x,y distances from the cell origin */"
    "    float y0 = y-Y0;"
    "    "
    "    float x1, y1, x2, y2;"
    "    int ii, jj;"
    "    float t0, t1, t2;"
    "    "
    "    /* For the 2D case, the simplex shape is an equilateral triangle. */"
    "    /* Determine which simplex we are in. */"
    "    int i1, j1; /* Offsets for second (middle) corner of simplex in (i,j) coords */"
    "    if(x0>y0) {i1=1; j1=0;} /* lower triangle, XY order: (0,0)->(1,0)->(1,1) */"
    "    else {i1=0; j1=1;}      /* upper triangle, YX order: (0,0)->(0,1)->(1,1) */"
    "    "
    "    /* A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and */"
    "    /* a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where */"
    "    /* c = (3-sqrt(3))/6 */"
    "    "
    "    x1 = x0 - i1 + 0.211324865f; /* Offsets for middle corner in (x,y) unskewed coords */"
    "    y1 = y0 - j1 + 0.211324865f;"
    "    x2 = x0 - 1.0f + 2.0f * 0.211324865f; /* Offsets for last corner in (x,y) unskewed coords */"
    "    y2 = y0 - 1.0f + 2.0f * 0.211324865f;"
    "    "
    "    /* Wrap the integer indices at 256, to avoid indexing perm[] out of bounds */"
    "    ii = i % 256;"
    "    jj = j % 256;"
    "    "
    "    /* Calculate the contribution from the three corners */"
    "    t0 = 0.5f - x0*x0-y0*y0;"
    "    if(t0 < 0.0f) n0 = 0.0f;"
    "    else {"
    "        t0 *= t0;"
    "        n0 = t0 * t0 * grad2(perm[ii+perm[jj]], x0, y0);"
    "    }"
    "    "
    "    t1 = 0.5f - x1*x1-y1*y1;"
    "    if(t1 < 0.0f) n1 = 0.0f;"
    "    else {"
    "        t1 *= t1;"
    "        n1 = t1 * t1 * grad2(perm[ii+i1+perm[jj+j1]], x1, y1);"
    "    }"
    "    "
    "    t2 = 0.5f - x2*x2-y2*y2;"
    "    if(t2 < 0.0f) n2 = 0.0f;"
    "    else {"
    "        t2 *= t2;"
    "        n2 = t2 * t2 * grad2(perm[ii+1+perm[jj+1]], x2, y2);"
    "    }"
    "    "
    "    /* Add contributions from each corner to get the final noise value. */"
    "    /* The result is scaled to return values in the interval [-1,1]. */"
    "    return 40.0f * (n0 + n1 + n2); /* TODO: The scale factor is preliminary! */"
    "}" // https://github.com/terrybroad/Simplex_Noise_OpenCL/blob/master/Simplex_Noise.cl
    "void kernel terrain_gen(global int* landmap_flags, const float3 pos, const int LOD, const int chunkSize) {"
    "    const uint n = get_global_id(0);"
    "    const uint x = n%(chunkSize+2);"
    "    const uint y = (n/(chunkSize+2))%(chunkSize+2);"
    "    const uint z = n/((chunkSize+2)*(chunkSize+2));"
    "    enum BLOCK { FILLED, AIR };"
    "    uint frequency = 10000;"
    "    const float noise_1 = (0.6-fabs(0.1-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency,   ((z*LOD)+(chunkSize*pos.z))/frequency)))"
    "                  + 0.5 * (0.6-fabs(0.1-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency/2, ((z*LOD)+(chunkSize*pos.z))/frequency/2)))"
    "                 + 0.25 * (0.6-fabs(0.1-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency*4, ((z*LOD)+(chunkSize*pos.z))/frequency*4)))"
    "                 + 0.25 * (0.6-fabs(0.1-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency*8, ((z*LOD)+(chunkSize*pos.z))/frequency*8)))"
    "                + 0.125 * (0.6-fabs(0.1-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency*16,((z*LOD)+(chunkSize*pos.z))/frequency*16)));"
    "    frequency = 500;"
    "    const float noise_2 = (0.5-fabs(0.1-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency,   ((z*LOD)+(chunkSize*pos.z))/frequency)))"
    "                  + 0.5 * (0.5-fabs(0.2-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency/2, ((z*LOD)+(chunkSize*pos.z))/frequency/2)))"
    "                 + 0.25 * (0.5-fabs(0.2-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency*4, ((z*LOD)+(chunkSize*pos.z))/frequency*4)))"
    "                 + 0.25 * (0.5-fabs(0.2-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency*8, ((z*LOD)+(chunkSize*pos.z))/frequency*8)))"
    "                + 0.125 * (0.5-fabs(0.2-_slang_library_noise2(((x*LOD)+(chunkSize*pos.x))/frequency*16,((z*LOD)+(chunkSize*pos.z))/frequency*16)));"
    "    const float noise_3 = _slang_library_noise2( ((x*LOD)+(chunkSize*pos.x))/frequency/30, ((z*LOD)+(chunkSize*pos.z))/frequency/30 );"
    "    const float noise_4 = _slang_library_noise2( ((x*LOD)+(chunkSize*pos.x))/frequency/400, ((z*LOD)+(chunkSize*pos.z))/frequency/400 );"
    "    "
    "    const float yVar = 1000*noise_1*noise_1*noise_1*noise_1*noise_1*noise_1+(100*noise_2*noise_2)-(800*noise_3*noise_3)-(6000*noise_4*noise_4);"
    "    landmap_flags[n] = (yVar>((y*LOD)+(chunkSize*pos.y))) ? FILLED : AIR;"
    "    if(yVar >= -200) {"
    "        landmap_flags[n] = (-200<(y*LOD+(chunkSize * pos.y))) ? AIR : FILLED;"
    "        if (landmap_flags[n] == AIR)"
    "            landmap_flags[n] = (yVar-100>((y*LOD)+(chunkSize*pos.y))) ? FILLED : AIR;"
    "    }"
    "}";

    sources.push_back({ noise_kernel_code.c_str(), noise_kernel_code.length() });

    cl::Program program(context, sources);

    if (program.build({ default_device }) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }

    // could be 68 minumum, also try 256 or 512
    const int n = 68 * 68 * 68;

    // write buffers for noise generation
    CLW::buffer_landmap = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * n);
    CLW::queue = cl::CommandQueue(context, default_device, CL_QUEUE_PROFILING_ENABLE);

    int *landmap_flags = new int[n];
    queue.enqueueWriteBuffer(buffer_landmap, CL_TRUE, 0, sizeof(int) * n, landmap_flags);
    CLW::terrain_gen = cl::Kernel(program, "terrain_gen");
    delete[] landmap_flags;
}

void CLW::CLNoise(int* landmap_flags, cl_float3 pos, cl_int LOD, cl_int chunkSize) {
    const int n = 68 * 68 * 68;

    terrain_gen.setArg(0, buffer_landmap);
    terrain_gen.setArg(1, pos);
    terrain_gen.setArg(2, LOD);
    terrain_gen.setArg(3, chunkSize);

    queue.enqueueNDRangeKernel(terrain_gen, cl::NullRange, cl::NDRange(n), cl::NDRange(32));
    queue.enqueueReadBuffer(buffer_landmap, CL_TRUE, 0, sizeof(int) * n, landmap_flags);

    queue.finish();
}