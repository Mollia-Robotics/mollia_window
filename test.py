import mollia_window
import zengl

wnd = mollia_window.main_window()
ctx = zengl.context()

image = ctx.image(wnd.size, 'rgba8unorm', samples=4)
image.clear_value = (1.0, 1.0, 1.0, 1.0)

while mollia_window.update():
    wnd.demoui()
    if wnd.key_down('mouse1'):
        image.clear_value = (0.0, 0.0, 0.0, 1.0)
    else:
        image.clear_value = (1.0, 1.0, 1.0, 1.0)
    image.clear()
    image.blit()
