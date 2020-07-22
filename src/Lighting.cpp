#include "Lighting.h"

Light::Light(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular, 
             glm::vec3 color)
	: ambient(ambient), diffuse(diffuse), specular(specular), color(color) {

}

Light::~Light() {

}

PointLight::PointLight(glm::vec3 ambient, 
                       glm::vec3 diffuse, 
                       glm::vec3 specular, 
                       glm::vec3 color,
                       glm::vec3 position,
                       float constant,
                       float linear,
                       float quadratic)
	: Light::Light(ambient, diffuse, specular, color), 
      constant(constant), linear(linear), quadratic(quadratic), position(position) {

}

PointLight::~PointLight() {

}

DirLight::DirLight(glm::vec3 ambient,
                   glm::vec3 diffuse,
                   glm::vec3 specular,
                   glm::vec3 color,
                   glm::vec3 direction)
    : Light::Light(ambient, diffuse, specular, color), direction(direction) {

}

DirLight::~DirLight() {

}