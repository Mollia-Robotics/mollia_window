import moderngl as mgl
import mollia_window

class Color:
    value = (0.0, 0.6, 1.0)

wnd = mollia_window.main_window((600, 400))
ctx = mgl.create_context()

wnd.command('blue', lambda: setattr(Color, 'value', (0.0, 0.6, 1.0)))
wnd.command('orange', lambda: setattr(Color, 'value', (1.0, 0.6, 0.0)))

while mollia_window.update():
    ctx.clear(*Color.value)
