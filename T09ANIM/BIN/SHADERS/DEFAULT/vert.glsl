/* DD6 */
#version 420

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;

uniform mat4 MatrWVP;
uniform float Time;

out vec3 DrawColor;
out vec2 DrawTexCoord;
out vec3 DrawNormal;
out vec3 DrawPos;

void main( void )
{
  int n = gl_VertexID;
  float a = Time * 47 * acos(-1) / 180, c = cos(a), s = sin(a);
  mat4 m =
    mat4(vec4(c, 0, -s, 0),
         vec4(0, 1, 0, 0),
         vec4(s, 0, c, 0),
         vec4(0, 0, 0, 1));
 a = sin(Time * 2) * 2 * acos(-1) / 180, c = cos(a), s = sin(a);
  mat4 m1 =
    mat4(vec4(c, s, 0, 0),
         vec4(-s, c, 0, 0),
         vec4(0, 0, 1, 0),
         vec4(0, 0, 0, 1));
  gl_Position = MatrWVP *  vec4(InPosition / 0.3, 1);
  DrawColor = InNormal;
  DrawNormal = InNormal;
  DrawTexCoord = InTexCoord;
 
}
