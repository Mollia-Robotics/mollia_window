# mollia_window

**mollia_window** is a Python module tailor-made for research and development. Code required for presenting on the screen is minimal. Implementing a window off the Python thread is usually not a goal for other windowing libraries. Long running tasks updating the window rarely may result in unresponsive windows. Having a large number of child windows alongside the main window can result in poor rendering performance. Closing the window should not affect headlessly running code. These are the main issues addressed by mollia_window.
 
