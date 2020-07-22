#include "Frustum.h"

namespace Planes {
    enum
    {
        Near = 0,
        Far,
        Left,
        Right,
        Top,
        Bottom
    };
}

float Plane::distanceToPoint(const glm::vec3& point) const noexcept {
    return glm::dot(point, normal) + distance;
}

//calcuate matrix based on the projection * view matrix.
void Frustum::update(const glm::mat4& mat) noexcept {
    // left
    _planes[Planes::Left].normal.x     = mat[0][3] + mat[0][0];
    _planes[Planes::Left].normal.y     = mat[1][3] + mat[1][0];
    _planes[Planes::Left].normal.z     = mat[2][3] + mat[2][0];
    _planes[Planes::Left].distance     = mat[3][3] + mat[3][0];

    // right
    _planes[Planes::Right].normal.x    = mat[0][3] - mat[0][0];
    _planes[Planes::Right].normal.y    = mat[1][3] - mat[1][0];
    _planes[Planes::Right].normal.z    = mat[2][3] - mat[2][0];
    _planes[Planes::Right].distance    = mat[3][3] - mat[3][0];

    // bottom
    _planes[Planes::Bottom].normal.x   = mat[0][3] + mat[0][1];
    _planes[Planes::Bottom].normal.y   = mat[1][3] + mat[1][1];
    _planes[Planes::Bottom].normal.z   = mat[2][3] + mat[2][1];
    _planes[Planes::Bottom].distance   = mat[3][3] + mat[3][1];

    // top
    _planes[Planes::Top].normal.x      = mat[0][3] - mat[0][1];
    _planes[Planes::Top].normal.y      = mat[1][3] - mat[1][1];
    _planes[Planes::Top].normal.z      = mat[2][3] - mat[2][1];
    _planes[Planes::Top].distance      = mat[3][3] - mat[3][1];

    // near
    _planes[Planes::Near].normal.x     = mat[0][3] + mat[0][2];
    _planes[Planes::Near].normal.y     = mat[1][3] + mat[1][2];
    _planes[Planes::Near].normal.z     = mat[2][3] + mat[2][2];
    _planes[Planes::Near].distance     = mat[3][3] + mat[3][2];

    // far
    _planes[Planes::Far].normal.x      = mat[0][3] - mat[0][2];
    _planes[Planes::Far].normal.y      = mat[1][3] - mat[1][2];
    _planes[Planes::Far].normal.z      = mat[2][3] - mat[2][2];
    _planes[Planes::Far].distance      = mat[3][3] - mat[3][2];

    for (auto& plane : _planes)
    {
        float length = glm::length(plane.normal);
        plane.normal    /= length;
        plane.distance  /= length;
    }
}


bool Frustum::pointInFrustum(const glm::vec3& point) const noexcept {
    for (auto& plane : _planes)
    {
        if (plane.distanceToPoint(point) < 0)
        {
            return false;
        }
    }
    return true;
}


bool Frustum::boxInFrustum(const AABB& box) const noexcept {
    bool result = true;

    for (auto& plane : _planes)
    {
        if (plane.distanceToPoint(box.getVP(plane.normal)) < 0)
        {
            return false;
        }
        else if (plane.distanceToPoint(box.getVN(plane.normal)) < 0)
        {
            result = true;
        }
    }

    return result;
}

