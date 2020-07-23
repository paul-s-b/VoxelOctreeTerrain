#ifndef CHUNK_H_
#define CHUNK_H_

#include "ShadingProgram.h"
#include "FreeCamera.h"
#include "Texture.h"
#include "CLW.h"
#include "AABB.h"
#include <stdlib.h>
#include <math.h>
#include <cstdlib>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <ext.hpp>

//change uint8_t to uint16_t if number of blocks in enum exceeds 255
enum BLOCK : uint8_t { FILLED, AIR };
enum Direction : uint8_t { North = 1, East = 2, South = 4, West = 8, Up = 16, Down = 32 };

// chunkSize should be power of 2 up to 64
const cl_int chunkSize = 64;
// 3D array i is arbitrary size 96 x 96 x 96
// allows for LOD up to level 16

class Chunk {
protected:
	static constexpr const char* _vertexPath = "src/shaders/voxel.vert";
	static constexpr const char* _fragPath = "src/shaders/voxel.frag";

	static ShadingProgram* _program;
	static GLuint _perspectiveID;
	static GLuint _lookAtID;
	static GLuint _VPID;
	static GLuint _posID;
	static GLuint _camposID;

	// IDs to the OpenGL array buffers
	GLuint _trianglesID;
	GLuint _normalsID;
	GLuint _IndiceID;

	// vertex array object
	GLuint _VAO;

	glm::ivec3 _pos;

	virtual void _loadArrayBuffers();
	virtual void _makeVAO();

private:

	int _nrOfIndices;
	int _castValue;
	cl_int _LOD;

	// array of vertices
	std::vector<glm::vec3> _triangles;

	// 1 normal per vertex
	std::vector<glm::vec3> _normals;

	// 3 vertices
	std::vector<glm::ivec3> _indices;

	// AABB of chunk
	AABB _aabb;

	// adds rectangle to the _triangles, _uvs, _normals.
	void _addRectangle(glm::vec3 center, glm::vec3 height, glm::vec3 width);

	// fill the _triangles, _uvs and _normals
	void _createMesh(glm::ivec3 pos, int landmap_flags[68 * 68 * 68], cl_int LOD);

	// openCL wrapper
	CLW _clw;

public:

	// contains no OpenGL calls and so can be called on any thread
	Chunk(glm::ivec3 pos, cl_int LOD);
	explicit Chunk(int _castValue) : _castValue{ _castValue }, _aabb({ 0, 0, 0 }){};

	// call this method to initialize shading program and potentially
	// other things in future
	static void init();

	// the stuff that should have been in the constructor but contained
	// OpenGL functions and so didn't allow creation of Chunk object in
	// seperate threads. Call Load() only in the main thread.
	void load();

	// must be called on main thread.
	void unload();

	// returns the position of the chunk
	glm::ivec3 pos() const;
	// returns the level of detail
	cl_int lod() const;
	AABB getAABB() const;

	// render vector of chunks
	static void render(const CameraData& cam_data, const std::vector<Chunk*>& chunks);

};


#endif // CHUNK_H_
