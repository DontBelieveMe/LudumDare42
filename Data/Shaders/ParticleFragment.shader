#version 300 es

precision highp float;
precision highp int;

//in vec2 pass_TetureUV;
in vec4 pass_Color;
out vec4 out_Color;

//uniform sampler2D u_Sprite;

void main()
{
    out_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f); //pass_Color;
}