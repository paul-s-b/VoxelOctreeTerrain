#ifndef LANDSCAPE_H_
#define LANDSCAPE_H_

#include "Chunk.hpp"
#include "FreeCamera.hpp"
#include "ChunkLoader.hpp"
#include "Octree.h"
#include <glm.hpp>

class LandscapeManager {
	void updateCenter(glm::ivec3 newCenter);

	Octree* _octree;
	glm::ivec3 _center;
	ChunkLoader _chunkLoader;
	std::unordered_map<glm::ivec3, Chunk*> _chunks;
	std::unordered_map<glm::ivec3, int> _tempNodes;

public:

	LandscapeManager();
	~LandscapeManager();

	void render(const CameraData& cam_data, FreeCamera camera);
};

#endif // !LANDSCAPE_H_