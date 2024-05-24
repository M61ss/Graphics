# Window

The entire GLFW windowing system guide is available at the [GLFW's official documentation](http://www.glfw.org/docs/latest/window.html).

## Initialization

You need to initialize the window using `glfwInit()` this way:

```c++
if (glfwInit() == GLFW_FALSE)
{
	return -1;
}
```

As you can see, it returns `GLFW_FALSE` on fail. Otherwise, it returns `GLFW_TRUE`.

Remember to terminate GLFW every time before exiting the program with the function `glfwTerminate()` (see the section below).
\
If `glfwInit()` fails, there is no need to run `glfwTerminate()` because, if the initialization fails, there is nothing to terminate or clean, OpenGL performs all operations automatically in this case.

### Setting up GLFW version

You need to set the version of GLFW (that is the same of OpenGL) that you are going to use into your program. If the user doesn't have the correct version of GLFW, it fails to run.
\
You have to insert a version that must to be 3.3 or upper:

```c++
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
```

The first argument is the option that we want to configure. All these options have the prefix "GLFW\_" and the complete list is available at [this link](http://www.glfw.org/docs/latest/window.html#window_hints).
\
The second argument is an integer that sets the value of our option, so version 3.3 is reported into the function as 3.
\
Both major and minor versions must be specified because there could be cases in which they don't match.

Since we want to use the core version of GLFW, it is useful to deactivate functions that aren't core. Functions that aren't core are all obsolete and deprecated. To do that, simply type:

```c++
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

The first argument is the option we want to configure.
\
The second argument is the new option's value.
\
The majority of OpenGL's functions works all this way, i.e. putting the option to be modified in first position and new option's values in next, so I won't repeat subsequently.

### Setting up GLAD

GLAD manages function pointers for OpenGL, so it is logical to initialize also GLAD at the start of the program:

```c++
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
}
```

We pass a `GLADloadproc` to the function to load the address of the OpenGL function pointers which is OS-specific. GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.

## Creating a window

### Setting up window's settings

The standard procedure to create a window is:

```c++
GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
// Error check if the window fails to create
if (window == NULL)
{
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
}
glfwMakeContextCurrent(window);
```

`glfwCreateWindow()` arguments:

- **width**
- **height**
- window's **name**
- ignore that for now
- ignore that for now

Now we need to tell to OpenGL the size of the rendering window:

```c++
glViewport(0, 0, 800, 600);
```

`glViewport()`:

- **start x** coord
- **start y** coord
- **end x** coord
- **end y** coord

_Note_: we can set also smaller OpenGL viewport's dimensions than the GLFW's one. It is clear that a piece of GLFW's viewport won't be shown.

If we want to get the viewport resizable, then we need to implement this **callback function**:

```c++
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
```

This function simply resizes `window` with new `width` and `height`.

Then, we need to assign the callback function to our window. We can do that this way:

```c++
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```

`window` is the window that we want to get resizable and `framebuffer_size_callback` is a pointer to the callback function that we want to assign to that window. This way we are specifying the behavior of `window` when the user resizes it via GUI.

There are also many callbacks functions we can set to **register our own functions**, but only after we have created the window and before the render loop is started.

### Render loop

To keep the window updated we need to run a loop which listens for events continuously:

```c++
while(!glfwWindowShouldClose(window))
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

At the start the loop checks if the window has been instructed to be closed with the function `glfwWindowShouldClose()`.
\
The `glfwSwapBuffers()` will swap the color buffer of the `window` (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.

---

_Notice_:
\
All rendering machines keep in memory two buffers: the **front buffer**, for the final output image, and the **back buffer**, which store the frame that is modified from rendering functions and will be shown at next iteration of the render loop.
\
Every operation executed into the loop is applied on the back buffer that will be shown at te next start of the loop. It is possible thanks to `glfwSwapBuffers()`, which is a function that swap front and back buffers every time it get called. 
\
So, buffers are swapped at every iteration so that the front buffer become the back one and vice versa.

---

Then `glfwPollEvents()` checks if any events are triggered. To manage events it calls callback functions.

## Input

To manage inputs of a GLFW window we will create a function which we call every time a new iteration of the render loop starts. Inside that we should put behaviors for every input that we want to manage. 

```c++
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
```

For example, here we used `glfwGetKey()`. This function returns whether the specified button (`GLFW_KEY_ESCAPE`) is currently being pressed or, generalizing, it returns the current status of a key. 

Finally, the render loop should look like this:

```c++
while (!glfwWindowShouldClose(window))
{
	processInput(window);

	// ... rendering operations ...

	glfwSwapBuffers(window);
	glfwPollEvents();
}
```

## Rendering operations



## Cleanup before close

Before exiting the program, it is important to call `glfwTerminate()`. It closes all open windows, cursors, ecc., resets OpenGL settings that maybe have been changed running the program and frees memory that has maybe has been allocated. Summing up, it sets the library at an uninitialized state. For this reason, it is eventually possible to call `glfwInit()` again after calling `glfwTerminate()`.
