import moderngl as mgl
import mollia_window

wnd = mollia_window.main_window((1280, 720), imgui=True)
ctx = mgl.create_context()

while mollia_window.update():
    ctx.clear(0.0, 0.6, 1.0)
