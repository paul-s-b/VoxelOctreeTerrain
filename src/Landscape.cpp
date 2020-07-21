#include "Landscape.hpp"

LandscapeManager::LandscapeManager()
	: _center(0, 0, 0)
{
	_octree = new Octree(glm::vec3(0, 0, 0), glm::vec3(pow(2, 16), pow(2, 16), pow(2, 16)));

	// initialize octree with the position of the camera
	_octree->insert(glm::vec3(0, 0, 0));

	for(auto i: _octree->leafNodes)
		_chunkLoader.Add(i.first, i.second);
	std::cout << "Generating: " << _octree->leafNodes.size() << " chunks...\n";
}

void LandscapeManager::updateCenter(glm::ivec3 newCenter)
{
	_chunkLoader.Clear();

	// reconstruct octree
	tempNodes = _octree->leafNodes;
	
	// update camera position in octree
	delete _octree;
	_octree = new Octree(glm::vec3(0, 0, 0), glm::vec3(pow(2, 16), pow(2, 16), pow(2, 16)));
	_octree->leafNodes.clear();
	_octree->insert(newCenter);

	for (auto i : _octree->leafNodes) {
		if (tempNodes.find(i.first) == tempNodes.end())
			_chunkLoader.Add(i.first, i.second);
		else 
			tempNodes.erase(tempNodes.find(i.first));
	}

	std::cout << "Generating: " << tempNodes.size() << " chunks...\n";

	//for (auto i : tempNodes) {
	//	_chunks.at(i.first)->Unload();
	//	delete _chunks.at(i.first);
	//	_chunks.erase(i.first);
	//}
	//tempNodes.clear();

	_center = newCenter;
}

LandscapeManager::~LandscapeManager()
{
	for(auto i : _chunks) {
		i.second->Unload();
		delete i.second;
	}
	delete _octree;
}

void LandscapeManager::Render(const CameraData& cam_data, FreeCamera camera)
{
	if (_chunkLoader.getChunkLoaderSize() == 0)
	{
		for (auto i : tempNodes) {
			_chunks.at(i.first)->Unload();
			delete _chunks.at(i.first);
			_chunks.erase(i.first);
		}
		tempNodes.clear();
	}
	// only update the center if the chunks have finished loading
	if (_octree->leafNodes.size() == _chunks.size() && tempNodes.empty()) {

		glm::ivec3 newCenter = glm::round(glm::vec3(cam_data.position.x, cam_data.position.y, cam_data.position.z) / chunkSize);
		if (abs(newCenter.x - _center.x) >= 1 || abs(newCenter.y - _center.y) >= 1 || abs(newCenter.z - _center.z) >= 1)
			updateCenter(newCenter);
	}
	//std::cout << _center.x << " " << _center.y << " " << _center.z << "\n";
	std::vector<Chunk*> renderList;

	for (auto i : _octree->leafNodes) {
		Chunk* c = _chunkLoader.Get(i.first);
		if (c) {
			c->Load();
			_chunks.insert(std::make_pair(c->Pos(), c));
		}
	}

	for (auto i : _chunks)
		if (camera.getFrustum().boxInFrustum(i.second->GetAABB()))
			renderList.push_back(i.second);

	Chunk::Render(cam_data, renderList);
	_chunkLoader.DeleteDeadChunks();
}