#include "ChunkLoader.hpp"

ChunkLoader::ChunkLoader()
{
	for (size_t i = 0; i < 4; i++)
	{
		_futures[i] = _promises[i].get_future();
		_threads[i] = new std::thread(&ChunkLoader::_chunkCreator, this, i);
		_threadClean[i] = false;
	}
}

ChunkLoader::~ChunkLoader(void)
{
	Clear();
	for (size_t i = 0; i < 4; i++)
		_promises[i].set_value();
	for (size_t i = 0; i < 4; i++)
		_threads[i]->join();
}

static std::tuple<int, int, int> map_key(const glm::ivec3& pos)
{
	return std::make_tuple(pos.x, pos.y, pos.z);
}

void	ChunkLoader::_chunkCreator(size_t threadNum)
{
	while (_futures[threadNum].wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		_mutex[0].lock();
		if (_threadClean[threadNum])
			_threadClean[threadNum] = false;
		if (_waitingChunks.empty())
		{
			_mutex[0].unlock();
			continue;
		}
		glm::ivec3 pos = _waitingChunks.front().pos;
		int LOD = _waitingChunks.front().LOD;
		_waitingChunks.pop();
		_mutex[0].unlock();

		Chunk *c = new Chunk(pos, LOD);

		_mutex[1].lock();
		if (_threadClean[threadNum])
			_chunksToFree.push_back(c);
		else
			_loadedChunks[map_key(pos)] = c;
		_mutex[1].unlock();
	}
}

void	ChunkLoader::Add(const glm::ivec3& pos, const int& LOD)
{
	_mutex[0].lock();
	_waitingChunks.push({ pos, LOD });
	_mutex[0].unlock();
}

Chunk	*ChunkLoader::Get(const glm::ivec3& pos)
{
	//std::cout << _loadedChunks.count(map_key(pos));
	_mutex[1].lock();
	if (_loadedChunks.count(map_key(pos)) == 0)
	{
		_mutex[1].unlock();
		return nullptr;
	}
	Chunk *c = _loadedChunks[map_key(pos)];
	_loadedChunks.erase(map_key(pos));
	_mutex[1].unlock();
	return c;
}

void	ChunkLoader::Clear(void)
{
	_mutex[1].lock();
	for (auto const& p : _loadedChunks)
		_chunksToFree.push_back(p.second);
	_loadedChunks.clear();
	_waitingChunks = std::queue<Pair>();
	for (int i = 0; i < 4; i++)
		_threadClean[i] = true;
	_mutex[1].unlock();
}

void	ChunkLoader::DeleteDeadChunks(void)
{
	_mutex[1].lock();
	if (_chunksToFree.empty())
	{
		_mutex[1].unlock();
		return;
	}
	Chunk* c = _chunksToFree.front();
	c->Unload();
	delete c;

	_chunksToFree.pop_front();
	_mutex[1].unlock();
}

int ChunkLoader::getChunkLoaderSize() const
{
	return _waitingChunks.size();
}