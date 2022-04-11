import mollia_window

wnd = mollia_window.main_window((600, 400))
child1 = mollia_window.child_window('child window 1', (400, 400), menubar=True)
child2 = mollia_window.child_window('child window 2', (400, 400), menubar=True)

wnd.command('main command 1', lambda: print('main command 1'))
child1.command('child 1 command 1', lambda: print('child 1 command 1'))
child1.command('child 1 command 2', lambda: print('child 1 command 2'))
wnd.command('main command 2', lambda: print('main command 2'))
child2.command('child 2 command 1', lambda: print('child 2 command 1'))

while mollia_window.update():
    pass
