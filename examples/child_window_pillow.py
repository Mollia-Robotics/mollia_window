from PIL import Image, ImageDraw

import mollia_window

wnd = mollia_window.main_window((600, 400), visible=False)
child = mollia_window.child_window('child window pillow', (400, 400))

img = Image.new('RGB', child.size, '#fff')
draw = ImageDraw.Draw(img)
draw.text((20, 20), 'Hello World', '#000')
child.update(img)

while mollia_window.update():
    if not child.visible:
        break
