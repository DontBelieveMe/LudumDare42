#version 300 es

precision highp float;
precision highp int;

in vec3 in_Position;    
//in vec2 in_TextureUV;

//out vec2 pass_TetureUV;
out vec4 pass_Color;

uniform mat4 u_Projection;
uniform mat4 u_ModelView;
//uniform vec4 u_Color;

void main()
{
//    const float SCALE = 10.0f;
    //pass_TetureUV = in_TextureUV;
//    pass_Color = u_Color;
    gl_Position = u_Projection * u_ModelView * vec4(in_Position.xy, 0.0, 1.0);
}