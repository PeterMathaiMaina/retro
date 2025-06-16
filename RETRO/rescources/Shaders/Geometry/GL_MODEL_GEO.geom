#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in DATA {
    vec3 FragPos;
    vec3 Normal;
} gs_in[];

out vec3 lineColor;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    for (int i = 0; i < 3; ++i) {
        vec3 start = gs_in[i].FragPos;
        vec3 end = start + normalize(gs_in[i].Normal) * 0.1;

        lineColor = vec3(0.1, 0.7, 0.0);

        gl_Position = u_Projection * u_View * vec4(start, 1.0);
        EmitVertex();

        gl_Position = u_Projection * u_View * vec4(end, 1.0);
        EmitVertex();

        EndPrimitive();
    }
}
