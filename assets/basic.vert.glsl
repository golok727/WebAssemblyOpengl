#version 300 es
                          
layout(location = 0) in vec3 aPos;

uniform mat4 u_MVP;
void main() {
    vec4 position = vec4(aPos, 1.0);
    gl_Position = u_MVP * position;
}
