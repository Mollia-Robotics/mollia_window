import io
import os
import shutil
import zipfile

import requests

os.makedirs('deps', exist_ok=True)

if not os.path.isdir('deps/imgui'):
    res = requests.get('https://github.com/ocornut/imgui/archive/refs/tags/v1.88.zip')
    if not res.ok:
        raise Exception(res.text)
    pack = zipfile.ZipFile(io.BytesIO(res.content))
    pack.extractall('deps')
    os.rename('deps/imgui-1.88', 'deps/imgui')

if not os.path.isdir('deps/SDL2'):
    res = requests.get('https://github.com/libsdl-org/SDL/releases/download/release-2.24.0/SDL2-devel-2.24.0-VC.zip')
    if not res.ok:
        raise Exception(res.text)

    pack = zipfile.ZipFile(io.BytesIO(res.content))
    pack.extractall('deps')
    os.rename('deps/SDL2-2.24.0', 'deps/SDL2')

if not os.path.isfile('SDL2.dll'):
    shutil.copy('deps/SDL2/lib/x64/SDL2.dll', 'SDL2.dll')
