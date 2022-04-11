import subprocess

from setuptools import Extension, setup

commit = subprocess.check_output('git rev-parse --verify HEAD', shell=True).decode().strip()
if subprocess.check_output('git status --short', shell=True).decode().strip():
    commit += '-dirty'

ext = Extension(
    name='mollia_window',
    libraries=['User32', 'Gdi32', 'Shell32', 'OpenGL32'],
    sources=[
        './mollia_window/base_window.cpp',
        './mollia_window/child_window.cpp',
        './mollia_window/input.cpp',
        './mollia_window/main_window.cpp',
        './mollia_window/module.cpp',
        './mollia_window/pixels.cpp',
        './mollia_window/preview_window.cpp',
        './mollia_window/ui_thread.cpp',
    ],
    define_macros=[
        ('BUILD_COMMIT', commit),
    ],
    depends=[
        './mollia_window/base_window.hpp',
        './mollia_window/child_window.hpp',
        './mollia_window/input.hpp',
        './mollia_window/common.hpp',
        './mollia_window/main_window.hpp',
        './mollia_window/module.hpp',
        './mollia_window/pixels.hpp',
        './mollia_window/preview_window.hpp',
        './mollia_window/ui_thread.hpp',
        'setup.py',
    ],
    # extra_compile_args=['/Z7'],
    # extra_link_args=['/DEBUG:FULL'],
)

setup(
    name='mollia_window',
    version='1.0.0',
    install_requires=['pillow'],
    ext_modules=[ext],
)
