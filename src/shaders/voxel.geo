#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 color_v[];
out vec3 gColor;

void main()
{
  for(int i=0; i<3; i++)
  {
    gColor = color_v[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}