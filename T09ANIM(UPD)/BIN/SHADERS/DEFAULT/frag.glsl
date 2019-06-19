/* DD6 */
#version 420

layout(location = 0) out vec4 OutColor;

uniform float Time;

in vec3 DrawColor;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec3 DrawPos;

uniform float Addon0;
uniform float Addon1;

uniform sampler2D TextureKd;

void main( void )
{
  /*
  if (Addon1 >= 20)
    discard;
  if (Addon1 / Addon0 > abs(sin(Time)))
    discard;
  */
  
  vec2 t = DrawTexCoord;
  vec3 c = texture(TextureKd, t).rgb;

  /*
  float s = 47;

  if (((int(t.x * s) ^ int(t.y * s)) & 1) == 1)
    c = vec3(1, 0, 0);
  else
    c = vec3(0, 1, 0);
  */
  float nl = dot(normalize(DrawNormal), normalize(vec3(1, 3, 2)));
  
  nl = max(0.1, nl);

  OutColor = vec4(c * nl, 1);
}