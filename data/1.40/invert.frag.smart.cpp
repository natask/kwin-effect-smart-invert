#version 140
uniform sampler2D sampler;
uniform vec4 modulation;
uniform float saturation;

in vec2 texcoord0;

out vec4 fragColor;

void main()
{
    vec4 tex = texture(sampler, texcoord0);

    if (saturation != 1.0) {
        vec3 desaturated = tex.rgb * vec3( 0.30, 0.59, 0.11 );
        desaturated = vec3( dot( desaturated, tex.rgb ));
        tex.rgb = tex.rgb * vec3( saturation ) + desaturated * vec3( 1.0 - saturation );
    }

    //invert smart wbolster 
    float maxColor = max (tex.r, max (tex.g, tex.b)); 
    float minColor = min (tex.r, min (tex.g, tex.b));
    float lightness = (maxColor + minColor) / 2.0; 
    float delta = (1.0 - lightness) - lightness; 

    tex.rgb = delta + tex.rgb;
    tex *= modulation;
    tex.rgb *= tex.a; 

    fragColor = tex;
}
