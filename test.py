import numpy as np
import mollia_window
import zengl

wnd = mollia_window.main_window()
ctx = zengl.context()

image = ctx.image(wnd.size, 'rgba8unorm', samples=4)
image.clear_value = (1.0, 1.0, 1.0, 1.0)

pixels = np.random.uniform(0.0, 1.0, (256, 256, 4))
pixels.transpose(1, 0, 2)[:, :, 2] = np.linspace(0.0, 1.0, 256)

texture = ctx.image((256, 256), 'rgba8unorm', (pixels * 255).astype('u1').tobytes())


wnd.ui.update({
    'callbacks': {
        'print': lambda: print('Hello World!'),
        'console_execute': lambda line: print(line),
        'top_view': lambda: zengl.inspect(texture)['texture'],
    },
    'variables': {
        'is_foo': {
            'value': False,
        },
        'is_bar': {
            'value': True,
        },
        'temperature_1': {
            'value': 32.2,
            'min': -14.0,
            'max': 99.8,
            'type': 'float',
        },
        'temperature_2': {
            'value': 32.2,
            'min': -14.0,
            'max': 99.8,
            'type': 'float',
        },
        'temperature_3': {
            'value': 32.2,
            'min': -14.0,
            'max': 99.8,
            'type': 'float',
        },
        'frame_1': {
            'value': 25,
            'min': 0,
            'max': 100,
            'type': 'int',
        },
        'frame_2': {
            'value': 25,
            'min': 0,
            'max': 100,
            'type': 'int',
        },
        'frame_3': {
            'value': 25,
            'min': 0,
            'max': 100,
            'type': 'int',
        },
        'vegetable': {
            'value': 'carrot',
            'options': ['carrot', 'potato', 'pepper'],
        },
    },
    'console': {
        'open': False,
        'lines': [
            'Sed ornare turpis urna, ac porta mauris elementum sodales.',
            'Pellentesque mi lacus, convallis eu arcu quis, feugiat varius nibh.',
            'Aliquam viverra malesuada aliquet. Sed eu neque tempor, fringilla enim sit amet, posuere purus.',
            'Nam venenatis ac velit ac eleifend. Curabitur ac purus ut ex placerat congue.',
            'Donec quis quam sit amet ipsum ullamcorper dignissim.',
            'Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.',
        ],
        'line': '',
    },
    'sidebar': {
        'open': True,
        'content': [
            {
                'type': 'header',
                'text': 'Settings 1',
                'open': True,
                'content': [
                    {
                        'type': 'tree',
                        'text': 'Settings Nested 1',
                        'open': True,
                        'content': [
                            {'type': 'checkbox', 'text': 'Foo Enabled', 'variable': 'is_foo'},
                            {'type': 'checkbox', 'text': 'Bar Mode', 'variable': 'is_bar'},
                            {'type': 'slider', 'text': 'Temperature 1', 'variable': 'temperature_1', 'format': '%.2f'},
                            {'type': 'slider', 'text': 'Frame 1', 'variable': 'frame_1', 'format': '%d'},
                            {'type': 'drag', 'text': 'Temperature 2', 'variable': 'temperature_2', 'format': '%.2f', 'speed': 1.0},
                            {'type': 'drag', 'text': 'Frame 2', 'variable': 'frame_2', 'format': '%d', 'speed': 1.0},
                            {'type': 'input', 'text': 'Temperature 3', 'variable': 'temperature_3', 'format': '%.2f', 'step': 0.5},
                            {'type': 'input', 'text': 'Frame 3', 'variable': 'frame_3', 'format': '%d', 'step': 1},
                            {'type': 'combo', 'text': 'Vegetable', 'variable': 'vegetable'},
                            {'type': 'button', 'text': 'Click Me', 'click': 'print'},
                            {'type': 'separator'},
                            {'type': 'text', 'text': 'Some Text'},
                            {'type': 'text', 'text': 'Some Text'},
                            {'type': 'text', 'text': 'Some Text'},
                            {'type': 'separator'},
                            {
                                'type': 'line',
                                'content': [
                                    {'type': 'button', 'text': 'Test 1', 'click': 'print'},
                                    {'type': 'button', 'text': 'Test 2', 'click': 'print'},
                                    {'type': 'button', 'text': 'Test 3', 'click': 'print'},
                                ],
                            },
                            {'type': 'separator'},
                        ],
                    },
                    {
                        'type': 'tree',
                        'text': 'Settings Nested 2',
                        'open': False,
                        'content': [
                            {'type': 'text', 'text': 'FooBar?'},
                            {'type': 'text', 'text': 'FooBar?'},
                            {'type': 'text', 'text': 'FooBar?'},
                            {'type': 'text', 'text': 'FooBar?'},
                        ],
                    },
                    {
                        'type': 'tree',
                        'text': 'Settings Nested 3',
                        'open': False,
                        'content': [
                            {'type': 'text', 'text': 'FooBar?'},
                            {'type': 'text', 'text': 'FooBar?'},
                            {'type': 'text', 'text': 'FooBar?'},
                            {'type': 'text', 'text': 'FooBar?'},
                        ],
                    },
                ],
            },
            {
                'type': 'header',
                'text': 'View 1',
                'open': False,
                'content': [
                    {'type': 'image', 'texture': 'top_view', 'width': 256.0, 'height': 256.0, 'flip': True },
                ],
            },
            {
                'type': 'header',
                'text': 'View 2',
                'open': False,
                'content': [
                    {'type': 'image', 'texture': 'top_view', 'width': 256.0, 'height': 256.0, 'flip': True },
                ],
            },
            {
                'type': 'header',
                'text': 'View 3',
                'open': False,
                'content': [
                    {'type': 'image', 'texture': 'top_view', 'width': 256.0, 'height': 256.0, 'flip': True },
                ],
            },
            {
                'type': 'header',
                'text': 'Views',
                'open': True,
                'content': [
                    {
                        'type': 'table',
                        'columns': 2,
                        'content': [
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0, 'flip': True },
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0, 'flip': True },
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0, 'flip': False },
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0, 'flip': False },
                        ]
                    },
                ],
            },
        ],
    },
    'tooltip': None,
})

while mollia_window.update():
    wnd.demo()

    if wnd.key_down('mouse1'):
        image.clear_value = (0.0, 0.0, 0.0, 1.0)
    else:
        image.clear_value = (1.0, 1.0, 1.0, 1.0)
    image.clear()
    image.blit()
