#include "Chunk.h"

ShadingProgram* Chunk::_program;
GLuint Chunk::_perspectiveID;
GLuint Chunk::_lookAtID;
GLuint Chunk::_posID;
GLuint Chunk::_VPID;
GLuint Chunk::_camposID;

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

Chunk::Chunk(glm::ivec3 pos, cl_int LOD) 
	: _nrOfIndices(0), _pos(pos), _LOD(LOD), _aabb( {chunkSize * LOD, chunkSize * LOD, chunkSize * LOD } ) {
	_aabb.update({ (pos.x - (LOD / 2.0)) * chunkSize,
				   (pos.y - (LOD / 2.0)) * chunkSize,
				   (pos.z - (LOD / 2.0)) * chunkSize });

	int *_landmap_flags = new int[68 * 68 * 68];
	cl_float3 clpos = { pos.x - (LOD / 2.0), pos.y - (LOD / 2.0), pos.z - (LOD / 2.0) };
	_clw.CLNoise(_landmap_flags, clpos, LOD, chunkSize);
	_createMesh(pos, _landmap_flags, LOD);

	delete[] _landmap_flags;
}

void Chunk::unload() {
	glDeleteBuffers(1, &_trianglesID);
	glDeleteBuffers(1, &_normalsID);
	glDeleteBuffers(1, &_IndiceID);
	glDeleteVertexArrays(1, &_VAO);
}

void Chunk::init() {
	_program = new ShadingProgram(_vertexPath, _fragPath);
	_perspectiveID = glGetUniformLocation(_program->ID(), "perspective");
	_lookAtID = glGetUniformLocation(_program->ID(), "lookAt");
	_VPID = glGetUniformLocation(_program->ID(), "VP");
	_posID = glGetUniformLocation(_program->ID(), "pos");
	_camposID = glGetUniformLocation(_program->ID(), "campos");
}

void Chunk::load() {
	_loadArrayBuffers();
	_makeVAO();
}

void Chunk::_loadArrayBuffers() {
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_trianglesID);
	glGenBuffers(1, &_normalsID);
	glGenBuffers(1, &_IndiceID);

	glBindBuffer(GL_ARRAY_BUFFER, _trianglesID);
	glBufferData(GL_ARRAY_BUFFER,
		_triangles.size() * 3 * sizeof(GLfloat),
		_triangles.data(),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _normalsID);
	glBufferData(GL_ARRAY_BUFFER,
		_normals.size() * 3 * sizeof(GLfloat),
		_normals.data(),
		GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndiceID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		_indices.size() * 3 * sizeof(GLfloat),
		_indices.data(),
		GL_STATIC_DRAW);

}

void Chunk::_makeVAO() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _trianglesID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _normalsID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Chunk::_addRectangle(glm::vec3 center, glm::vec3 height, glm::vec3 width) {
	glm::vec3 corner1 = center - (height / 2.0) - (width / 2.0);
	glm::vec3 corner2 = center - (height / 2.0) + (width / 2.0);
	glm::vec3 corner3 = center + (height / 2.0) + (width / 2.0);
	glm::vec3 corner4 = center + (height / 2.0) - (width / 2.0);

	glm::vec3 normal = glm::cross(height, width);

	// triangle 1
	_triangles.push_back(corner3);
	_triangles.push_back(corner2);
	_triangles.push_back(corner1);

	_normals.push_back(normal);
	_normals.push_back(normal);
	_normals.push_back(normal);
	
	_indices.push_back(glm::ivec3(_nrOfIndices + 0, _nrOfIndices + 1, _nrOfIndices + 2));

	// triangle 2 

	_triangles.push_back(corner4);
	_normals.push_back(normal);

	_indices.push_back(glm::ivec3(_nrOfIndices + 2, _nrOfIndices + 3, _nrOfIndices + 0));
	_nrOfIndices += 4;

}

void Chunk::_createMesh(glm::ivec3 pos, int landmap_flags[68 * 68 * 68], cl_int LOD) {
	std::byte* faces = new std::byte[chunkSize * chunkSize * chunkSize];

	int index = 0;

	// a index conversion from a single index array to a 3d array
	// landmap_flags[x + y * (chunkSize + (2 * LOD)) + z * (chunkSize + (2 * LOD)) * (chunkSize + (2 * LOD))] 

	for (int x = 1; x < chunkSize + 1; x++) {
		for (int y = 1; y < chunkSize + 1; y++) {
			for (int z = 1; z < chunkSize + 1; z++) {
				//x + y * (chunkSize + (2 * LOD)) + z * (chunkSize + (2 * LOD)) * (chunkSize + (2 * LOD));
				faces[index] = (std::byte)0;
				if (landmap_flags[x + y * (chunkSize + 2) + z * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
				{
					index++;
					continue;
				}
				if (landmap_flags[x + y * (chunkSize + 2) + z * (chunkSize + 2) * (chunkSize + 2)] != BLOCK::AIR)
				{
					if (landmap_flags[(x - 1) + y * (chunkSize + 2) + z * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
						faces[index] |= (std::byte)Direction::South;					 
					if (landmap_flags[(x + 1) + y * (chunkSize + 2) + z * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
						faces[index] |= (std::byte)Direction::North;					 
					if (landmap_flags[x + (y - 1) * (chunkSize + 2) + z * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
						faces[index] |= (std::byte)Direction::Down;						 
					if (landmap_flags[x + (y + 1) * (chunkSize + 2) + z * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
						faces[index] |= (std::byte)Direction::Up;						 
					if (landmap_flags[x + y * (chunkSize + 2) + (z - 1) * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
						faces[index] |= (std::byte)Direction::West;						 
					if (landmap_flags[x + y * (chunkSize + 2) + (z + 1) * (chunkSize + 2) * (chunkSize + 2)] == BLOCK::AIR)
						faces[index] |= (std::byte)Direction::East;
				}


				index++;
			}
		}
	}
	index = 0;
	for (int x = LOD; x < (chunkSize * LOD) + LOD; x += LOD) {
		for (int y = LOD; y < (chunkSize * LOD) + LOD; y += LOD) {
			for (int z = LOD; z < (chunkSize * LOD) + LOD; z += LOD) {

				if (faces[index] == (std::byte)0)
				{
					index++;
					continue;
				}
	
				if ((faces[index] & (std::byte)Direction::North) != (std::byte)0)
				{
					_addRectangle(
						glm::vec3( x + ((chunkSize - 1) * pos.x) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2) + (float(LOD) / 2),
								   y + ((chunkSize - 1) * pos.y) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   z + ((chunkSize - 1) * pos.z) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2)),
						glm::vec3(0, LOD, 0),
						glm::vec3(0, 0, -LOD));
				}
				if ((faces[index] & (std::byte)Direction::East) != (std::byte)0)
				{
					_addRectangle(
						glm::vec3( x + ((chunkSize - 1) * pos.x) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   y + ((chunkSize - 1) * pos.y) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   z + ((chunkSize - 1) * pos.z) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2) + (float(LOD) / 2)),
						glm::vec3(0, LOD, 0),
						glm::vec3(LOD, 0, 0));
				}
				if ((faces[index] & (std::byte)Direction::South) != (std::byte)0)
				{
					_addRectangle(
						glm::vec3( x + ((chunkSize - 1) * pos.x) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2) - (float(LOD) / 2),
								   y + ((chunkSize - 1) * pos.y) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   z + ((chunkSize - 1) * pos.z) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2)),
						glm::vec3(0, LOD, 0),
						glm::vec3(0, 0, LOD));
				}
				if ((faces[index] & (std::byte)Direction::West) != (std::byte)0)
				{
					_addRectangle(
						glm::vec3( x + ((chunkSize - 1) * pos.x) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
							       y + ((chunkSize - 1) * pos.y) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   z + ((chunkSize - 1) * pos.z) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2) - (float(LOD) / 2)),
						glm::vec3(0, LOD, 0),
						glm::vec3(-LOD, 0, 0));
				}
				if ((faces[index] & (std::byte)Direction::Up) != (std::byte)0)
				{
					_addRectangle(
						glm::vec3( x + ((chunkSize - 1) * pos.x) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   y + ((chunkSize - 1) * pos.y) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2) + (float(LOD) / 2),
								   z + ((chunkSize - 1) * pos.z) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2)),
						glm::vec3(LOD, 0, 0),
						glm::vec3(0, 0, LOD));
				}
				if ((faces[index] & (std::byte)Direction::Down) != (std::byte)0)
				{
					_addRectangle(
						glm::vec3( x + ((chunkSize - 1) * pos.x) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2),
								   y + ((chunkSize - 1) * pos.y) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2) - (float(LOD) / 2),
								   z + ((chunkSize - 1) * pos.z) - ((chunkSize * LOD) / 2) - (float(LOD - 1) / 2)),
						glm::vec3(LOD, 0, 0),
						glm::vec3(0, 0, -LOD));
				}
				index++;
			}
		}
	}

	
	delete[]faces;
}

glm::ivec3 Chunk::pos() const {
	return _pos;
}

cl_int Chunk::lod() const {
	return _LOD;
}

AABB Chunk::getAABB() const {
	return _aabb;
}

void Chunk::render(const CameraData& cam_data, const std::vector<Chunk*>& chunks) {

	_program->use();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	for (auto chunk : chunks)
	{
		glBindVertexArray(chunk->_VAO);
		glUniform3f(_posID, float(chunk->_pos.x), float(chunk->_pos.y), float(chunk->_pos.z));
		glUniform3fv(_camposID, 1, &cam_data.position[0]);

		glUniformMatrix4fv(_VPID, 1, GL_FALSE,
			glm::value_ptr(cam_data.VP));
		glDrawElements(GL_TRIANGLES, chunk->_indices.size() * 3 * sizeof(GLfloat), GL_UNSIGNED_INT, 0);
	}
	glDisable(GL_CULL_FACE);
}