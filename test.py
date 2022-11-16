import json

import numpy as np
import zengl

import mollia_window

wnd = mollia_window.main_window()
ctx = zengl.context()

image = ctx.image(wnd.size, 'rgba8unorm', samples=4)
image.clear_value = (1.0, 1.0, 1.0, 1.0)

pixels = np.random.uniform(0.0, 1.0, (256, 256, 4))
pixels.transpose(1, 0, 2)[:, :, 2] = np.linspace(0.0, 1.0, 256)

texture = ctx.image((256, 256), 'rgba8unorm', (pixels * 255).astype('u1').tobytes())

wnd.slider('alpha')
wnd.slider('beta')
wnd.increment('gamma')
wnd.increment('delta')

wnd.checkbox('wireframe')
wnd.combo('number', 'two', ['one', 'two', 'three'])
wnd.button('btn', lambda: print('yes'))

wnd.image('texture1', zengl.inspect(texture)['texture'], (100, 100), texture.size)
wnd.image('texture2', zengl.inspect(texture)['texture'], (400, 100), texture.size)
wnd.image('texture3', zengl.inspect(texture)['texture'], (700, 100), texture.size, flip=True)

while wnd.update():
    mx, my = wnd.mouse

    if wnd.key_down('mouse1'):
        image.clear_value = (0.0, 0.0, 0.0, 1.0)
    else:
        image.clear_value = (1.0, 1.0, 1.0, 1.0)

    if wnd.key_down('mouse2'):
        wnd.tooltip(f'({mx}, {my})')

    print('Hello World!', file=wnd.log)
    print('Hello World! 1', file=wnd.log)
    print('Hello World!', file=wnd.log)
    print(f'Hello World! {mx}', file=wnd.log)
    print('Hello World!', file=wnd.log)
    print(f'Hello World! {my}', file=wnd.log)
    print(json.dumps(wnd.config, indent=2), file=wnd.log)

    image.clear()
    image.blit()
