import mollia_window

wnd = mollia_window.main_window((600, 400))
child1 = mollia_window.child_window('child window 1', (400, 400))
child2 = mollia_window.child_window('child window 2', (400, 400))

while mollia_window.update():
    pass
