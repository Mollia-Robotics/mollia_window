import os
import time

import mollia_window

preview = mollia_window.preview_window('preview', (400, 400))
preview.title = 'Preview Window'

for i in range(10):
    preview.update(os.urandom(400 * 400 * 3))
    time.sleep(0.5)
