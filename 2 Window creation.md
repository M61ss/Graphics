# Creating a window in OpenGL

## Initialization

First of all you need to initialize the GLFW library. To do that call the function `glfwInit()` that returns `GLFW_TRUE` on success, `GLFW_FALSE` on fail.
\
Before exiting the program you need to close the library. You have to destroy all remaning windows or cursors that you previously created and restore settings which you may changed. You have to do that using `glfwTerminate()` that has no effect if `glfwInit()` failed.

```c++
// Initialize GLFW
if (glfwInit() == GLFW_FALSE) {
	return -1;
}

// Terminate GLFW before ending the program
glfwTerminate();
```
