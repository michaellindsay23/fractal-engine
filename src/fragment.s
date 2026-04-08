#version 330 core
precision highp float;
varying vec2 v_texCoord; // Assume range 0.0 to 1.0

void main() {
    // Center and scale coordinates
    vec2 uv = v_texCoord * 2.0 - 1.0;
    
    // Convert to integer coordinates for bitwise Sierpinski logic
    // The Sierpinski triangle can be represented as: (x & y) == 0
    ivec2 p = ivec2(v_texCoord * 512.0);
    
    float color = 0.0;
    if ((p.x & p.y) == 0) {
        color = 1.0;
    }

    gl_FragColor = vec4(vec3(color), 1.0);
}
