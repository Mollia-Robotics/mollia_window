#include <Python.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

SDL_Window * main_window;

PyObject * meth_main_window(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"size", NULL};

    int width = 1280;
    int height = 720;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|(II)", keywords, &width, &height)) {
        return NULL;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    const char * glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    main_window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(main_window);
    SDL_GL_MakeCurrent(main_window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO & io = ImGui::GetIO();
    io.IniFilename = NULL;

    ImGui_ImplSDL2_InitForOpenGL(main_window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    Py_RETURN_NONE;
}

PyObject * meth_imgui_demo(PyObject * self, PyObject * args, PyObject * kwargs) {
    ImGui::ShowDemoWindow(NULL);
    Py_RETURN_NONE;
}

PyObject * meth_update(PyObject * self, PyObject * args, PyObject * kwargs) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            Py_RETURN_FALSE;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(main_window)) {
            Py_RETURN_FALSE;
        }
    }

    ImGui::Render();

    const bool srgb = glIsEnabled(0x8DB9);

    if (srgb) {
        glDisable(0x8DB9);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (srgb) {
        glEnable(0x8DB9);
    }

    SDL_GL_SwapWindow(main_window);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    Py_RETURN_TRUE;
}

PyMethodDef module_methods[] = {
    {"main_window", (PyCFunction)meth_main_window, METH_VARARGS | METH_KEYWORDS, NULL},
    {"update", (PyCFunction)meth_update, METH_VARARGS | METH_KEYWORDS, NULL},
    {"imgui_demo", (PyCFunction)meth_imgui_demo, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "mollia_window", NULL, -1, module_methods};

extern "C" PyObject * PyInit_mollia_window() {
    PyObject * module = PyModule_Create(&module_def);
    return module;
}
