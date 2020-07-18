#version 140
uniform sampler2D sampler;
uniform vec4 modulation;
uniform float saturation;

in vec2 texcoord0;

out vec4 fragColor;

void main() {
  vec4 c = texture(sampler, texcoord0);
  c = vec4(c.a + c.r*0.3333 - c.g*0.6666 - c.b*0.6666, c.a - c.r*0.6666 + c.g*0.3333 - c.b*0.6666, c.a - c.r*0.6666 - c.g*0.6666 + c.b*0.3333, c.a);
  c *= modulation;
  fragColor = c;
}
