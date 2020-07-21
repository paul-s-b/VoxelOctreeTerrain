#pragma once
#include "Chunk.hpp"
#include "FreeCamera.hpp"
#include "util_inc.hpp"
#include "ChunkLoader.hpp"
#include "Octree.h"

class LandscapeManager
{
	void updateCenter(glm::ivec3 newCenter);

	Octree *_octree;
	glm::ivec3 _center;
	ChunkLoader _chunkLoader;
	std::unordered_map<glm::ivec3, Chunk*> _chunks;
	std::unordered_map<glm::ivec3, int> tempNodes;

public:
	
	LandscapeManager();
	~LandscapeManager();

	void Render(const CameraData& cam_data, FreeCamera camera);
};