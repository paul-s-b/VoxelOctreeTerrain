#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include <cstdint>
#include <glm.hpp>


struct AABB
{
    public:
        AABB(const glm::vec3& dimensions) noexcept;

        bool isCollidingWith (const AABB& other) noexcept;

        void update(const glm::vec3& position) noexcept;

        glm::vec3 getVN(const glm::vec3& normal) const noexcept;
        glm::vec3 getVP(const glm::vec3& normal) const noexcept;

        const glm::vec3& getDimensions() const noexcept { return m_dimensions; }

    private:
        glm::vec3 m_min;
        glm::vec3 m_max;
        glm::vec3 m_dimensions;
};

#endif // AABB_H_INCLUDED
