mollia_window
=============

main_window
-----------

There can be only one main window per application.
This window type was designed to render real-time simulations with OpenGL.
The main window acceps user input and allows registering commands in the menubar.
The child windows can be shown using the menu.

child_window
------------

This window type requires the main window to be present.
The child window accepts user input and can be closed safely as it can be restored from the main window.
The updates are executed on the ui-thread, independently from the Python's GIL.
The child window content is a pre-rendered image, this helps to avoid Python invocations from a non Python thread.

preview_window
--------------

A preview window is similar to a child window with two exceptions.
It lifts the requirement for the main window, and it cannot accept user input.
The preview window is an output only window, whose updates are entirely live on the ui-thread.
This window type was designed to extract visual information from headless running code.

Compared to similar window implementations?
-------------------------------------------

There is a great variety of window libraries for Python that can be used as a render target for OpenGL.
This library was tailor-made for research and development. Code required for presenting on the screen is minimal.
Implementing a window off the Python thread is usually not a goal for other libraries.
Long running tasks updating the window rarely may result in unresponsive windows.
Having a large number of child windows alongside the main window can result in poor rendering performance.
Closing the window should not affect headlessly running code.
These are the main issues addressed by mollia_window.
