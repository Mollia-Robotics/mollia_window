import mollia_window
import zengl

wnd = mollia_window.main_window((1800, 960))
ctx = zengl.context()

image = ctx.image(wnd.size, 'rgba8unorm', samples=4)
image.clear_value = (1.0, 1.0, 1.0, 1.0)

import os
texture = ctx.image((256, 256), 'rgba8unorm', os.urandom(256 * 256 * 4))


def temp(line):
    wnd.ui.context['sidebar']['open'] = (line[0] == 'a')


wnd.ui.context.update({
    'callbacks': {
        'print': lambda: print('Hello World!'),
        'console_execute': temp,
        'top_view': lambda: zengl.inspect(texture)['texture'],
    },
    'variables': {
        'is_foobar': {
            'value': False,
        },
        'temperature': {
            'value': 32.2,
            'min': -14.0,
            'max': 99.8,
            'type': 'float',
        },
        'frame': {
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
        'history': ['hello()', 'world()'],
        'line': '',
    },
    'sidebar': {
        'open': True,
        'content': [
            {'type': 'text', 'text': 'Hello World!'},
            {'type': 'text', 'text': 'Hello World!'},
            {'type': 'text', 'text': 'Hello World!'},
            {'type': 'text', 'text': 'Hello World!'},
            {'type': 'text', 'text': 'Hello World!'},
            {'type': 'text', 'text': 'Hello World!'},
            {'type': 'button', 'text': 'Click Me', 'click': 'print'},
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
                            {'type': 'checkbox', 'text': 'FooBar', 'variable': 'is_foobar'},
                            # {'type': 'slider', 'text': 'Frame', 'variable': 'frame', 'format': '%d'},
                            # {'type': 'slider', 'text': 'Temperature', 'variable': 'temperature', 'format': '%.2f'},
                            {'type': 'drag', 'text': 'Frame', 'variable': 'frame', 'format': '%d', 'speed': 1.0},
                            {'type': 'drag', 'text': 'Temperature', 'variable': 'temperature', 'format': '%.2f', 'speed': 1.0},
                            {'type': 'combo', 'text': 'Vegetable', 'variable': 'vegetable'},
                            # {'type': 'text', 'text': 'FooBar?'},
                            # {'type': 'text', 'text': 'FooBar?'},
                            # {'type': 'text', 'text': 'FooBar?'},
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
                    {'type': 'image', 'texture': 'top_view', 'width': 256.0, 'height': 256.0 },
                ],
            },
            {
                'type': 'header',
                'text': 'View 2',
                'open': False,
                'content': [
                    {'type': 'image', 'texture': 'top_view', 'width': 256.0, 'height': 256.0 },
                ],
            },
            {
                'type': 'header',
                'text': 'View 3',
                'open': False,
                'content': [
                    {'type': 'image', 'texture': 'top_view', 'width': 256.0, 'height': 256.0 },
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
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0 },
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0 },
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0 },
                            {'type': 'image', 'texture': 'top_view', 'width': 190.0, 'height': 190.0 },
                        ]
                    },
                ],
            },
        ],
    },
})

while mollia_window.update():
    wnd.ui.demo()

    if wnd.key_down('mouse1'):
        image.clear_value = (0.0, 0.0, 0.0, 1.0)
    else:
        image.clear_value = (1.0, 1.0, 1.0, 1.0)
    image.clear()
    image.blit()
