import colorsys

import mollia_bullet
import mollia_window
import zengl

world = mollia_bullet.world()
world.gravity = (0.0, 0.0, -10.0)

ground = world.box(0.0, (10.0, 10.0, 10.0), (0.0, 0.0, -10.0), group=1, mask=1)

for i in range(10):
    box = world.box(1.0, (0.25, 0.25, 0.25), (0.0, 0.0, 2.0 + i), group=1, mask=1)
    box.color = color=colorsys.hls_to_rgb(i / 10, 0.5, 0.5)

window = mollia_window.main_window((600, 400))

ctx = zengl.context()

image = ctx.image(window.size, 'rgba8unorm', samples=4)
depth = ctx.image(window.size, 'depth24plus', samples=4)
image.clear_value = (0.95, 0.95, 0.95, 1.0)

uniform_buffer = ctx.buffer(size=64)
vertex_buffer = ctx.buffer(size=1024 * 1024)

pipeline = ctx.pipeline(
    vertex_shader='''
        #version 330

        layout (std140) uniform Common {
            mat4 mvp;
        };

        layout (location = 0) in vec3 in_vert;
        layout (location = 1) in vec3 in_norm;
        layout (location = 2) in vec3 in_color;

        out vec3 v_norm;
        out vec3 v_color;

        void main() {
            gl_Position = mvp * vec4(in_vert, 1.0);
            v_norm = in_norm;
            v_color = in_color;
        }
    ''',
    fragment_shader='''
        #version 330

        in vec3 v_norm;
        in vec3 v_color;

        layout (location = 0) out vec4 out_color;

        void main() {
            vec3 light = vec3(1.0, 2.0, 3.0);
            float lum = dot(normalize(light), normalize(v_norm)) * 0.5 + 0.5;
            out_color = vec4(v_color * lum, 1.0);
        }
    ''',
    layout=[
        {
            'name': 'Common',
            'binding': 0,
        },
    ],
    resources=[
        {
            'type': 'uniform_buffer',
            'binding': 0,
            'buffer': uniform_buffer,
        },
    ],
    framebuffer=[image, depth],
    topology='triangles',
    cull_face='back',
    vertex_buffers=zengl.bind(vertex_buffer, '3f 3f 3f', 0, 1, 2),
)

aspect = window.width / window.height
camera = zengl.camera((4.0, 5.0, 3.0), (0.0, 0.0, 0.0), aspect=aspect, fov=45.0)
uniform_buffer.write(camera)

while mollia_window.update():
    world.simulate()
    mesh = world.main_group.color_mesh()
    vertex_buffer.write(mesh)
    pipeline.vertex_count = len(mesh) // zengl.calcsize('3f 3f 3f')
    image.clear()
    depth.clear()
    pipeline.render()
    image.blit()
