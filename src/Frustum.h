#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include "AABB.h"
#include <glm.hpp>
#include <array>

struct Plane {
    float distance;
    glm::vec3 normal;

    float distanceToPoint(const glm::vec3& point) const noexcept;
};

class Frustum {
public:
    void update(const glm::mat4& projView) noexcept;

    bool pointInFrustum(const glm::vec3& point) const noexcept;
    bool boxInFrustum(const AABB& box) const noexcept;


private:
    std::array<Plane, 6> _planes;
};

#endif // FRUSTUM_H_
