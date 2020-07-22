#include "Landscape.hpp"

LandscapeManager::LandscapeManager()
	: _center(0, 0, 0) {
	_octree = new Octree(glm::vec3(0, 0, 0), glm::vec3(pow(2, 16), pow(2, 16), pow(2, 16)));

	// initialize octree with the position of the camera
	_octree->insert(glm::vec3(0, 0, 0));

	for (auto i : _octree->leafNodes)
		_chunkLoader.add(i.first, i.second);
	std::cout << "Generating: " << _octree->leafNodes.size() << " chunks...\n";
}

void LandscapeManager::updateCenter(glm::ivec3 newCenter) {
	_chunkLoader.clear();

	// reconstruct octree
	_tempNodes = _octree->leafNodes;

	// update camera position in octree
	delete _octree;
	_octree = new Octree(glm::vec3(0, 0, 0), glm::vec3(pow(2, 16), pow(2, 16), pow(2, 16)));
	_octree->leafNodes.clear();
	_octree->insert(newCenter);

	for (auto i : _octree->leafNodes) {
		if (_tempNodes.find(i.first) == _tempNodes.end())
			_chunkLoader.add(i.first, i.second);
		else
			_tempNodes.erase(_tempNodes.find(i.first));
	}

	std::cout << "Generating: " << _tempNodes.size() << " chunks...\n";
	_center = newCenter;
}

LandscapeManager::~LandscapeManager() {
	for (auto i : _chunks) {
		i.second->unload();
		delete i.second;
	}
	delete _octree;
}

void LandscapeManager::render(const CameraData& cam_data, FreeCamera camera) {
	if (_chunkLoader.getChunkLoaderSize() == 0)
	{
		for (auto i : _tempNodes) {
			_chunks.at(i.first)->unload();
			delete _chunks.at(i.first);
			_chunks.erase(i.first);
		}
		_tempNodes.clear();
	}
	// only update the center if the chunks have finished loading
	if (_octree->leafNodes.size() == _chunks.size() && _tempNodes.empty()) {

		glm::ivec3 newCenter = glm::round(glm::vec3(cam_data.position.x, cam_data.position.y, cam_data.position.z) / chunkSize);
		if (abs(newCenter.x - _center.x) >= 1 || abs(newCenter.y - _center.y) >= 1 || abs(newCenter.z - _center.z) >= 1)
			updateCenter(newCenter);
	}

	std::vector<Chunk*> renderList;

	for (auto i : _octree->leafNodes) {
		Chunk* c = _chunkLoader.get(i.first);
		if (c) {
			c->load();
			_chunks.insert(std::make_pair(c->pos(), c));
		}
	}

	for (auto i : _chunks)
		if (camera.getFrustum().boxInFrustum(i.second->getAABB()))
			renderList.push_back(i.second);

	Chunk::render(cam_data, renderList);
	_chunkLoader.deleteDeadChunks();
}