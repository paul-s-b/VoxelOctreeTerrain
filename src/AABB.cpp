#include "AABB.h"


AABB::AABB(const glm::vec3& dimension) noexcept
:   m_dimensions (dimension)
{ }

void AABB::update(const glm::vec3& position) noexcept
{
    m_min = position;
    m_max = m_min + m_dimensions;
}

bool AABB::isCollidingWith(const AABB& other) noexcept
{
    return  (m_min.x <= other.m_max.x && m_max.x >= m_min.x) &&
            (m_min.y <= other.m_max.y && m_max.y >= m_min.y) &&
            (m_min.z <= other.m_max.z && m_max.z >= m_min.z);
}


glm::vec3 AABB::getVN(const glm::vec3& normal) const noexcept
{
    glm::vec3 res = m_min;

    if (normal.x < 0)
    {
        res.x += m_dimensions.x;
    }
    if (normal.y < 0)
    {
        res.y += m_dimensions.y;
    }
    if (normal.z < 0)
    {
        res.z += m_dimensions.z;
    }

    return res;
}

glm::vec3 AABB::getVP(const glm::vec3& normal) const noexcept
{
    glm::vec3 res = m_min;

    if (normal.x > 0)
    {
        res.x += m_dimensions.x;
    }
    if (normal.y > 0)
    {
        res.y += m_dimensions.y;
    }
    if (normal.z > 0)
    {
        res.z += m_dimensions.z;
    }

    return res;
}
