#version 150
 
in vec4 v_color;
in vec4 v_position;
in vec2 v_texCoord0;

uniform sampler2DRect u_sampler2d;

out vec4 outputColor;
 
void main()
{
    vec4 color;
    color = texture(u_sampler2d, v_texCoord0);
    if (length(color.rgb) > 0.1)
        outputColor = vec4(color.rgb, 0.5);
    else
        outputColor = vec4(color.rgb, 0.1);
}
