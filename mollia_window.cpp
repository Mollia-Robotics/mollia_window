#include <Python.h>
#include <structmember.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

struct MainWindow {
    PyObject_HEAD
    PyObject * size;
    PyObject * mouse;
    PyObject * mouse_wheel;
    PyObject * text;
};

PyTypeObject * MainWindow_type;

SDL_Window * window;
bool closed;
int mouse_x;
int mouse_y;
int mouse_wheel;
bool key_down[280];
bool prev_key_down[280];
char text[1024];

PyObject * keys;
PyObject * empty_str;

MainWindow * meth_main_window(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"size", "title", NULL};

    int width = 1280;
    int height = 720;
    const char * title = "Mollia Window";

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|(II)s", keywords, &width, &height, &title)) {
        return NULL;
    }

    if (window) {
        PyErr_Format(PyExc_RuntimeError, "main window already exists");
        return NULL;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    unsigned char pixels[1024] = {
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff, 0xd4, 0x00, 0x00, 0xff,
        0xd4, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
    };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 32, 64, 0xff, 0xff00, 0xff0000, 0xff000000);
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO & io = ImGui::GetIO();
    io.IniFilename = NULL;

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    MainWindow * res = PyObject_New(MainWindow, MainWindow_type);
    res->size = Py_BuildValue("(II)", width, height);
    res->mouse = Py_BuildValue("(ii)", 0, 0);
    res->mouse_wheel = PyLong_FromLong(0);

    Py_INCREF(empty_str);
    res->text = empty_str;

    Py_INCREF(res);
    PyModule_AddObject(self, "wnd", (PyObject *)res);

    return res;
}

PyObject * meth_update(PyObject * self) {
    return PyObject_CallMethod(PyObject_GetAttrString(self, "wnd"), "update", NULL);
}

int sdl_key(int key) {
    switch (key) {
        case SDLK_TAB: return 9;
        case SDLK_LEFT: return 37;
        case SDLK_RIGHT: return 39;
        case SDLK_UP: return 38;
        case SDLK_DOWN: return 40;
        case SDLK_PAGEUP: return 33;
        case SDLK_PAGEDOWN: return 34;
        case SDLK_HOME: return 36;
        case SDLK_END: return 35;
        case SDLK_INSERT: return 45;
        case SDLK_DELETE: return 46;
        case SDLK_BACKSPACE: return 8;
        case SDLK_SPACE: return 32;
        case SDLK_RETURN: return 13;
        case SDLK_ESCAPE: return 27;
        case SDLK_QUOTE: return 222;
        case SDLK_COMMA: return 188;
        case SDLK_MINUS: return 189;
        case SDLK_PERIOD: return 190;
        case SDLK_SLASH: return 191;
        case SDLK_SEMICOLON: return 186;
        case SDLK_EQUALS: return 187;
        case SDLK_LEFTBRACKET: return 219;
        case SDLK_BACKSLASH: return 220;
        case SDLK_RIGHTBRACKET: return 221;
        case SDLK_BACKQUOTE: return 192;
        case SDLK_CAPSLOCK: return 20;
        case SDLK_SCROLLLOCK: return 145;
        case SDLK_NUMLOCKCLEAR: return 144;
        case SDLK_PRINTSCREEN: return 44;
        case SDLK_PAUSE: return 19;
        case SDLK_KP_0: return 96;
        case SDLK_KP_1: return 97;
        case SDLK_KP_2: return 98;
        case SDLK_KP_3: return 99;
        case SDLK_KP_4: return 100;
        case SDLK_KP_5: return 101;
        case SDLK_KP_6: return 102;
        case SDLK_KP_7: return 103;
        case SDLK_KP_8: return 104;
        case SDLK_KP_9: return 105;
        case SDLK_KP_PERIOD: return 110;
        case SDLK_KP_DIVIDE: return 111;
        case SDLK_KP_MULTIPLY: return 106;
        case SDLK_KP_MINUS: return 109;
        case SDLK_KP_PLUS: return 107;
        case SDLK_KP_ENTER: return 269;
        case SDLK_KP_EQUALS: return 160;
        case SDLK_LCTRL: return 162;
        case SDLK_LSHIFT: return 164;
        case SDLK_LALT: return 91;
        case SDLK_LGUI: return 161;
        case SDLK_RCTRL: return 163;
        case SDLK_RSHIFT: return 165;
        case SDLK_RALT: return 92;
        case SDLK_RGUI: return 93;
        case SDLK_0: return 48;
        case SDLK_1: return 49;
        case SDLK_2: return 50;
        case SDLK_3: return 51;
        case SDLK_4: return 52;
        case SDLK_5: return 53;
        case SDLK_6: return 54;
        case SDLK_7: return 55;
        case SDLK_8: return 56;
        case SDLK_9: return 57;
        case SDLK_a: return 65;
        case SDLK_b: return 66;
        case SDLK_c: return 67;
        case SDLK_d: return 68;
        case SDLK_e: return 69;
        case SDLK_f: return 70;
        case SDLK_g: return 71;
        case SDLK_h: return 72;
        case SDLK_i: return 73;
        case SDLK_j: return 74;
        case SDLK_k: return 75;
        case SDLK_l: return 76;
        case SDLK_m: return 77;
        case SDLK_n: return 78;
        case SDLK_o: return 79;
        case SDLK_p: return 80;
        case SDLK_q: return 81;
        case SDLK_r: return 82;
        case SDLK_s: return 83;
        case SDLK_t: return 84;
        case SDLK_u: return 85;
        case SDLK_v: return 86;
        case SDLK_w: return 87;
        case SDLK_x: return 88;
        case SDLK_y: return 89;
        case SDLK_z: return 90;
        case SDLK_F1: return 112;
        case SDLK_F2: return 113;
        case SDLK_F3: return 114;
        case SDLK_F4: return 115;
        case SDLK_F5: return 116;
        case SDLK_F6: return 117;
        case SDLK_F7: return 118;
        case SDLK_F8: return 119;
        case SDLK_F9: return 120;
        case SDLK_F10: return 121;
        case SDLK_F11: return 122;
        case SDLK_F12: return 123;
    }
    return 0;
}

PyObject * MainWindow_meth_update(MainWindow * self) {
    if (closed) {
        Py_RETURN_FALSE;
    }

    ImGui::Render();

    if (glIsEnabled(0x8DB9)) {
        glDisable(0x8DB9);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(0x8DB9);
    } else {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    SDL_GL_SwapWindow(window);
    memcpy(prev_key_down, key_down, sizeof(key_down));
    text[0] = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))) {
            closed = true;
            Py_RETURN_FALSE;
        }
        switch (event.type) {
            case SDL_TEXTINPUT: {
                ImGuiIO & io = ImGui::GetIO();
                if (!io.WantTextInput) {
                    strcat(text, event.text.text);
                }
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                ImGuiIO & io = ImGui::GetIO();
                if (!io.WantCaptureKeyboard) {
                    key_down[sdl_key(event.key.keysym.sym)] = event.key.state == SDL_PRESSED;
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                ImGuiIO & io = ImGui::GetIO();
                if (!io.WantCaptureMouse) {
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT: key_down[1] = event.button.state == SDL_PRESSED; break;
                        case SDL_BUTTON_RIGHT: key_down[2] = event.button.state == SDL_PRESSED; break;
                        case SDL_BUTTON_MIDDLE: key_down[4] = event.button.state == SDL_PRESSED; break;
                    }
                }
                break;
            }
            case SDL_MOUSEMOTION: {
                ImGuiIO & io = ImGui::GetIO();
                if (!io.WantCaptureMouse) {
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                }
                break;
            }
            case SDL_MOUSEWHEEL: {
                ImGuiIO & io = ImGui::GetIO();
                if (!io.WantCaptureMouse) {
                    mouse_wheel += event.wheel.y;
                }
                break;
            }
        }
    }

    Py_DECREF(self->mouse);
    Py_DECREF(self->mouse_wheel);
    Py_DECREF(self->text);

    if (text[0]) {
        self->text = PyUnicode_FromString(text);
    } else {
        Py_INCREF(empty_str);
        self->text = empty_str;
    }

    self->mouse = Py_BuildValue("(ii)", mouse_x, mouse_y);
    self->mouse_wheel = PyLong_FromLong(mouse_wheel);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    Py_RETURN_TRUE;
}

PyObject * MainWindow_meth_demoui(MainWindow * self) {
    ImGui::ShowDemoWindow(NULL);
    Py_RETURN_NONE;
}

int get_key(PyObject * key) {
    if (PyObject * key_code = PyDict_GetItem(keys, key)) {
        return PyLong_AsLong(key_code);
    }
    PyErr_Format(PyExc_ValueError, "no such key %R", key);
    return 0;
}

PyObject * MainWindow_meth_key_pressed(MainWindow * self, PyObject * arg) {
    if (int key = get_key(arg)) {
        if (key_down[key] && !prev_key_down[key]) {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    return NULL;
}

PyObject * MainWindow_meth_key_released(MainWindow * self, PyObject * arg) {
    if (int key = get_key(arg)) {
        if (!key_down[key] && prev_key_down[key]) {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    return NULL;
}

PyObject * MainWindow_meth_key_down(MainWindow * self, PyObject * arg) {
    if (int key = get_key(arg)) {
        if (key_down[key]) {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    return NULL;
}

PyObject * MainWindow_meth_key_up(MainWindow * self, PyObject * arg) {
    if (int key = get_key(arg)) {
        if (!key_down[key]) {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    return NULL;
}

void default_dealloc(PyObject * self) {
    Py_TYPE(self)->tp_free(self);
}

PyMethodDef MainWindow_methods[] = {
    {"update", (PyCFunction)MainWindow_meth_update, METH_NOARGS, NULL},
    {"key_pressed", (PyCFunction)MainWindow_meth_key_pressed, METH_O, NULL},
    {"key_released", (PyCFunction)MainWindow_meth_key_released, METH_O, NULL},
    {"key_down", (PyCFunction)MainWindow_meth_key_down, METH_O, NULL},
    {"key_up", (PyCFunction)MainWindow_meth_key_up, METH_O, NULL},
    {"demoui", (PyCFunction)MainWindow_meth_demoui, METH_NOARGS, NULL},
    {},
};

PyMemberDef MainWindow_members[] = {
    {"size", T_OBJECT, offsetof(MainWindow, size), READONLY, NULL},
    {"mouse", T_OBJECT, offsetof(MainWindow, mouse), READONLY, NULL},
    {"mouse_wheel", T_OBJECT, offsetof(MainWindow, mouse_wheel), READONLY, NULL},
    {"text", T_OBJECT, offsetof(MainWindow, text), READONLY, NULL},
    {},
};

PyType_Slot MainWindow_slots[] = {
    {Py_tp_methods, MainWindow_methods},
    {Py_tp_members, MainWindow_members},
    {Py_tp_dealloc, default_dealloc},
    {},
};

PyType_Spec MainWindow_spec = {"mollia_window.MainWindow", sizeof(MainWindow), 0, Py_TPFLAGS_DEFAULT, MainWindow_slots};

PyMethodDef module_methods[] = {
    {"main_window", (PyCFunction)meth_main_window, METH_VARARGS | METH_KEYWORDS, NULL},
    {"update", (PyCFunction)meth_update, METH_NOARGS, NULL},
    {},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "mollia_window", NULL, -1, module_methods};

void add_key(const char * name, int code) {
    PyObject * key_code = PyLong_FromLong(code);
    PyDict_SetItemString(keys, name, key_code);
    Py_DECREF(key_code);
}

extern "C" PyObject * PyInit_mollia_window() {
    PyObject * module = PyModule_Create(&module_def);
    MainWindow_type = (PyTypeObject *)PyType_FromSpec(&MainWindow_spec);
    empty_str = PyUnicode_FromString("");
    keys = PyDict_New();
    add_key("mouse1", 1);
    add_key("mouse2", 2);
    add_key("mouse3", 4);
    add_key("tab", 9);
    add_key("left_arrow", 37);
    add_key("right_arrow", 39);
    add_key("up_arrow", 38);
    add_key("down_arrow", 40);
    add_key("pageup", 33);
    add_key("pagedown", 34);
    add_key("home", 36);
    add_key("end", 35);
    add_key("insert", 45);
    add_key("delete", 46);
    add_key("backspace", 8);
    add_key("space", 32);
    add_key("enter", 13);
    add_key("escape", 27);
    add_key("apostrophe", 222);
    add_key("comma", 188);
    add_key("minus", 189);
    add_key("period", 190);
    add_key("slash", 191);
    add_key("semicolon", 186);
    add_key("equal", 187);
    add_key("left_bracket", 219);
    add_key("backslash", 220);
    add_key("right_bracket", 221);
    add_key("graveaccent", 192);
    add_key("capslock", 20);
    add_key("scrolllock", 145);
    add_key("numlock", 144);
    add_key("printscreen", 44);
    add_key("pause", 19);
    add_key("keypad_0", 96);
    add_key("keypad_1", 97);
    add_key("keypad_2", 98);
    add_key("keypad_3", 99);
    add_key("keypad_4", 100);
    add_key("keypad_5", 101);
    add_key("keypad_6", 102);
    add_key("keypad_7", 103);
    add_key("keypad_8", 104);
    add_key("keypad_9", 105);
    add_key("keypad_decimal", 110);
    add_key("keypad_divide", 111);
    add_key("keypad_multiply", 106);
    add_key("keypad_subtract", 109);
    add_key("keypad_add", 107);
    add_key("keypad_enter", 269);
    add_key("left_shift", 160);
    add_key("left_ctrl", 162);
    add_key("left_alt", 164);
    add_key("left_super", 91);
    add_key("right_shift", 161);
    add_key("right_ctrl", 163);
    add_key("right_alt", 165);
    add_key("right_super", 92);
    add_key("menu", 93);
    add_key("0", 48);
    add_key("1", 49);
    add_key("2", 50);
    add_key("3", 51);
    add_key("4", 52);
    add_key("5", 53);
    add_key("6", 54);
    add_key("7", 55);
    add_key("8", 56);
    add_key("9", 57);
    add_key("a", 65);
    add_key("b", 66);
    add_key("c", 67);
    add_key("d", 68);
    add_key("e", 69);
    add_key("f", 70);
    add_key("g", 71);
    add_key("h", 72);
    add_key("i", 73);
    add_key("j", 74);
    add_key("k", 75);
    add_key("l", 76);
    add_key("m", 77);
    add_key("n", 78);
    add_key("o", 79);
    add_key("p", 80);
    add_key("q", 81);
    add_key("r", 82);
    add_key("s", 83);
    add_key("t", 84);
    add_key("u", 85);
    add_key("v", 86);
    add_key("w", 87);
    add_key("x", 88);
    add_key("y", 89);
    add_key("z", 90);
    add_key("f1", 112);
    add_key("f2", 113);
    add_key("f3", 114);
    add_key("f4", 115);
    add_key("f5", 116);
    add_key("f6", 117);
    add_key("f7", 118);
    add_key("f8", 119);
    add_key("f9", 120);
    add_key("f10", 121);
    add_key("f11", 122);
    add_key("f12", 123);

    Py_INCREF(keys);
    PyModule_AddObject(module, "keys", keys);

    Py_INCREF(MainWindow_type);
    PyModule_AddObject(module, "MainWindow", (PyObject *)MainWindow_type);

    Py_INCREF(Py_None);
    PyModule_AddObject(module, "wnd", Py_None);
    return module;
}
