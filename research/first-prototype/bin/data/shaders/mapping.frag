#version 150
 
in vec2 a_texCoord0;

in vec4 v_color;
in vec4 v_position;
in vec2 v_texCoord0;

uniform sampler2DRect u_sampler2d;

out vec4 outputColor;
 
void main()
{
    outputColor = texture(u_sampler2d, a_texCoord0);
}
