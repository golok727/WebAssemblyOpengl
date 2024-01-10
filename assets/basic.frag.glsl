#version 300 es
               
precision mediump float; 
out vec4 color;

uniform vec4 u_Color;
void main() {
  color = u_Color;
}
