#shader vertex
#version 420 

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 420 

in vec2 v_TexCoord;
out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    vec4 texcolor = texture(u_Texture, v_TexCoord);
    color = u_Color;

    // color = texcolor * u_Color;
}
