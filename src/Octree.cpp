#include "Octree.h"

std::unordered_map<glm::ivec3, int> Octree::leafNodes;

Octree::Octree(const glm::ivec3& _origin, const glm::vec3& _halfDimension)
	: _origin(_origin), _halfDimension(_halfDimension) {
	// Initially, there are no _children
	for (int i = 0; i < 8; ++i)
		_children[i] = NULL;
}

Octree& Octree::operator=(const Octree& oct) {
	_origin = oct.getOrigin();
	_halfDimension = oct.getHalfDimension();

	return *this;
}

bool operator==(const Octree& o1, const Octree& o2) {
	return (o1._origin == o2._origin && o1._halfDimension == o2._halfDimension);
}

bool operator!=(const Octree& o1, const Octree& o2) {
	return !(o1 == o2);
}

std::size_t Octree::operator()(const Octree& oct) const {
	return 0; // Insert your hash here
}

Octree::Octree(const Octree& copy)
	: _origin(copy._origin), _halfDimension(copy._halfDimension) {
}

Octree::~Octree() {
	// Recursively destroy octants
	for (int i = 0; i < 8; ++i)
		delete _children[i];
}

double Octree::getDistanceToNode(const glm::vec3& p) {
	return sqrt(((p.x - _origin.x) * (p.x - _origin.x)) +
				((p.y - _origin.y) * (p.y - _origin.y)) +
				((p.z - _origin.z) * (p.z - _origin.z)));
}

// split or combine child nodes based on camera position
glm::ivec3 Octree::getOrigin() const
{
	return _origin;
}

glm::vec3 Octree::getHalfDimension() const {
	return _halfDimension;
}

int Octree::getHalfSize() const {
	return _halfDimension.x * 2;
}

// Determine which octant of the tree would contain 'point'
int Octree::getOctantContainingPoint(const glm::vec3& point) const {
	int oct = 0;
	if (point.x >= _origin.x) oct |= 4;
	if (point.y >= _origin.y) oct |= 2;
	if (point.z >= _origin.z) oct |= 1;
	return oct;
}

bool Octree::isLeafNode() const {
	 // We are a leaf iff we have no _children. Since we either have none, or 
	 // all eight, it is sufficient to just check the first.
	return _children[0] == NULL;
}

void Octree::insert(const glm::vec3& camPos) {

	// If this node doesn't have a _data point yet assigned 
	// and it is a leaf, then we're done!
	if (isLeafNode()) {

		// could be 1, but causes errors with decimals in the chunkLoader
		if (_halfDimension.x >= 2) {

			// Split the current node and create new empty trees for each
			// child octant.
			for (int i = 0; i < 8; ++i) {
				// Compute new bounding box for this child
				glm::vec3 newOrigin = _origin;
				newOrigin.x += _halfDimension.x * (i & 4 ? .5f : -.5f);
				newOrigin.y += _halfDimension.y * (i & 2 ? .5f : -.5f);
				newOrigin.z += _halfDimension.z * (i & 1 ? .5f : -.5f);
				_children[i] = new Octree(newOrigin, _halfDimension * .5f);

				// subdivide node further if it is within a threshhold from the camera
				// if it is too far from the camera it means the node will stay as a leaf node
				if (_children[i]->getDistanceToNode(camPos) < _children[i]->_halfDimension.x * 3.5)
					_children[i]->insert(camPos);
				else
					leafNodes[_children[i]->getOrigin()] = _children[i]->getHalfSize();
			}
		}	
		else // adds a leaf node if the dimension is at the minimum
			leafNodes[_origin] = this->getHalfSize();
	}
}
