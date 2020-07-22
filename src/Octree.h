#ifndef OCTREE_H_
#define OCTREE_H_

#include "Chunk.hpp"
#include <unordered_map>
#include <glm.hpp>
#include <gtx/hash.hpp>

class Octree {

	// Physical position/size. This implicitly defines the bounding 
	// box of this node
	glm::ivec3 _origin;        //! The physical center of this node
	glm::vec3 _halfDimension;  //! Half the width/height/depth of this node

	// The tree has up to eight _children and can additionally store
	// a point, though in many applications only, the leaves will store _data.
	Octree *_children[8]; //! Pointers to child octants

	public:
	static std::unordered_map<glm::ivec3, int> leafNodes;

	Octree(const glm::ivec3& _origin, const glm::vec3& _halfDimension);
	~Octree();

	Octree& operator=(const Octree& oct);
	friend bool operator==(const Octree& o1, const Octree& o2);
	friend bool operator!=(const Octree& o1, const Octree& o2);
	std::size_t operator()(const Octree& oct) const;
	Octree(const Octree& copy);


	double getDistanceToNode(const glm::vec3& p);
	glm::ivec3 getOrigin() const;
	glm::vec3 getHalfDimension() const;
	int getHalfSize() const;
	int getOctantContainingPoint(const glm::vec3& point) const;
	bool isLeafNode() const;
	void insert(const glm::vec3& camPos);

};

#endif // OCTREE_H_
