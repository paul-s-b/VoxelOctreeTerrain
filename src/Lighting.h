#pragma once
#include <glm.hpp>

class Light
{

public:
	Light(glm::vec3 ambient,
          glm::vec3 diffuse,
          glm::vec3 specular, 
          glm::vec3 color);
	~Light();

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;

};

class PointLight : public Light
{

public:
	PointLight(glm::vec3 ambient, 
               glm::vec3 diffuse, 
               glm::vec3 specular, 
               glm::vec3 color,
               glm::vec3 position,
               float constant = 1.0,
               float linear = 0.09,
               float quadratic = 0.032);
	~PointLight();

    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

};

class DirLight : public Light
{

public:
	DirLight(glm::vec3 ambient, 
             glm::vec3 diffuse, 
             glm::vec3 specular, 
             glm::vec3 color,
             glm::vec3 direction);
	~DirLight();

    glm::vec3 direction;

};