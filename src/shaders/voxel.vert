#version 410 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 VP;
uniform mat4 perspective;
uniform mat4 lookAt;
uniform vec3 pos;
uniform vec3 campos;

out	vec3 normal_v;
out vec3 dir_v;
out float dist_v;
out vec2 uv_v;
out vec3 color_v;
out vec3 FragPos;
out vec3 campos_v;

void	main()
{
    vec3 vertpos = vertex + vec3(pos.x, pos.y, pos.z);
    gl_Position = VP * vec4(vertpos, 1);
    normal_v = normal;
    dir_v = vec3(vertpos) - campos;
    dist_v = length(dir_v);
    dir_v = normalize(dir_v);
    FragPos = vertpos;
    color_v = color;

    campos_v = campos;
}
