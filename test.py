import mollia_window
import zengl

wnd = mollia_window.main_window((1800, 960))
ctx = zengl.context()

image = ctx.image(wnd.size, 'rgba8unorm', samples=4)
image.clear_value = (1.0, 1.0, 1.0, 1.0)

import os
texture = ctx.image((256, 256), 'rgba8unorm', os.urandom(256 * 256 * 4))

x = wnd.ui.variable(float, [1.0], [-5.0], [5.0], format='%.2f')
y = wnd.ui.variable(int, [3, 3, 3], [-5, -5, -5], [9, 9, 9])

x.update(1.5)

while mollia_window.update():
    with wnd.ui.window('Hello World!'):
        wnd.ui.text('Hello 1')
        wnd.ui.text('Hello 2')
        wnd.ui.button('Print', lambda: print('yes', x.value()))
        wnd.ui.button('Reset', lambda: y.update(1, 2, 3))
        wnd.ui.text('Hello 3')
        wnd.ui.slider('test x', x)
        wnd.ui.slider('test x', x)
        wnd.ui.slider('test y', y)

    wnd.ui.image(zengl.inspect(texture)['texture'], texture.size, 'Top View')

    # wnd.ui.demo()

    if wnd.key_down('mouse1'):
        image.clear_value = (0.0, 0.0, 0.0, 1.0)
    else:
        image.clear_value = (1.0, 1.0, 1.0, 1.0)
    image.clear()
    image.blit()
