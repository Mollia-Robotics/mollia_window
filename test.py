import mollia_window
import zengl

wnd = mollia_window.main_window()
ctx = zengl.context()

image = ctx.image((1280, 720), 'rgba8unorm', samples=4)
image.clear_value = (1.0, 1.0, 1.0, 1.0)

while mollia_window.update():
    mollia_window.imgui_demo()
    image.clear()
    image.blit()
