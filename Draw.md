# Drawing on a window

In this tutorial we will learn how to draw on a window trying to draw a simple triangle.

> [!IMPORTANT] Graphic pipeline
> If you need information about how a graphic pipeline works, see [this document](GraphicPipeline.md).

## Vertex input

To start drawing something we have to first give OpenGL some input vertex data. OpenGL is a 3D graphics library so all coordinates that we specify in OpenGL are in 3D (x, y, z).
\
OpenGL doesn't simply transform all your 3D coordinates to 2D pixels on your screen; OpenGL only processes 3D coordinates when they're in a specific range between -1.0 and 1.0 on all 3 axes (x, y, z). All coordinates within this so called "_**normalized device coordinates**_" range will end up visible on your screen and all coordinates outside this region won't.

Because we want to render a single triangle we want to specify a total of three vertices with each vertex having a 3D position. We define them in normalized device coordinates, the visible region of OpenGL, in a `float` array:

```c++
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
```

> [!NOTE]
> Since we want to draw a 2D triangle, z coordinate must be 0 for every vertex. This way the **depth** of the triangle remains the same making it look like it's 2D.

> [!IMPORTANT] Normalized Device Coordinates (NDC)
> Once your vertex coordinates have been processed in the vertex shader, they should be in normalized device coordinates which is a small space where the x, y and z values vary from -1.0 to 1.0. Any coordinates that fall outside this range will be discarded/clipped and won't be visible on your screen.
> \
> Since NDC are different from screen-space coordinates, NDC will then be transformed to screen-space coordinates via the viewport transform using the data you provided with `glViewport()`. The resulting screen-space coordinates are then transformed to fragments as inputs to your fragment shader.

With the vertex data defined we'd like to send it as input to the first process of the graphics pipeline: the vertex shader. This is done in three steps:

1. **Creating memory** on the GPU where we store the vertex data.
2. **Configure** how OpenGL should interpret the memory.
3. **Specify how** to send the data to the graphics card. The vertex shader then processes as much vertices as we tell it to from its memory.

We manage this memory via so called "**_vertex buffer objects_**" (VBO) that can store a large number of vertices in the GPU's memory.
\
The advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card, and keep it there if there's enough memory left, without having to send data one vertex at a time. Sending data to the graphics card from the CPU is relatively slow, so wherever we can we try to send as much data as possible at once. Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices making it extremely fast.

Just like any object in OpenGL, this buffer has a unique ID corresponding to that buffer, so we can generate one with a buffer ID `VBO` using the `glGenBuffers()` function:

```c++
unsigned int VBO;
glGenBuffers(1, &VBO);
```

OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is `GL_ARRAY_BUFFER`, so we can **bind** buffers.

> [!WARNING]
> OpenGL allows us to bind to several buffers at once as long as they have a different buffer type.

We can bind the newly created buffer to the `GL_ARRAY_BUFFER` target with the glBindBuffer function:

```c++
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

From that point on any buffer calls we make (in this case `GL_ARRAY_BUFFER`), it will be used to configure the currently bound buffer, which in this case is `VBO`. Then we can make a call to the `glBufferData()` function that copies the previously defined vertex data into the buffer's memory:

```c++
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

This is a function specifically targeted to copy user-defined data into the currently bound buffer.

1. The first argument is the type of the buffer we want to copy data into: the vertex buffer object currently bound to the `GL_ARRAY_BUFFER` target.
2. The second argument specifies the size of the data (in bytes) we want to pass to the buffer; a simple sizeof of the vertex data suffices.
3. The third parameter is the actual data we want to send.
4. The fourth parameter specifies how we want the graphics card to manage the given data. This can take 3 forms:

- `GL_STREAM_DRAW`: the data is set only once and used by the GPU at most a few times.
- `GL_STATIC_DRAW`: the data is set only once and used many times.
- `GL_DYNAMIC_DRAW`: the data is changed a lot and used many times.

The position data of the triangle does not change, is used a lot, and stays the same for every render call so its usage type should best be `GL_STATIC_DRAW`. If, for instance, one would have a buffer with data that is likely to change frequently, a usage type of `GL_DYNAMIC_DRAW` ensures the graphics card will place the data in memory that allows for faster writes.

## Vertex shader

The **vertex shader** is only one of the shaders that are programmable in this way. Modern OpenGL requires that we at least set up a vertex and fragment shader if we want to do some rendering so we will briefly introduce shaders and configure two very simple shaders for drawing our first triangle.

The first thing we need to do is write the vertex shader in the shader language **GLSL** (OpenGL Shading Language) and then compile this shader so we can use it in our application. Below the source code of a very basic vertex shader in GLSL:

```GLSL
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}
```

> [!IMPORTANT] GLSL
> As you can see, GLSL looks similar to C. Each shader begins with a declaration of its version. Since OpenGL 3.3 and higher the version numbers of GLSL match the version of OpenGL (GLSL version 420 corresponds to OpenGL version 4.2 for example). We also explicitly mention we're using core profile functionality.
>
> Next we declare all the input vertex attributes in the vertex shader with the `in` keyword. Right now we only care about position data so we only need a single vertex attribute. GLSL has a vector datatype that contains 1 to 4 floats based on its postfix digit. Since each vertex has a 3D coordinate we create a `vec3` (3 stands for 3D) input variable with the name `aPos`. We also specifically set the location of the input variable via `layout (location = 0)`.
>
> To set the output of the vertex shader we have to assign the position data to the **predefined** `gl_Position` variable which is a `vec4` behind the scenes. At the end of the main function, whatever we set to `gl_Position` will be used as the output of the vertex shader. Since our input is a vector of size 3 we have to cast this to a vector of size 4. We can do this by inserting the `vec3` values inside the constructor of `vec4` and set its `w` component to 1.0f.

> [!NOTE]
> The fourth component (`vec.w`) in `vec4` is a value used for **perspective division**.

> [!CAUTION]
> The current vertex shader is probably the most simple vertex shader we can imagine because we did no processing whatsoever on the input data and simply forwarded it to the shader's output. In real applications the input data is usually not already in normalized device coordinates so we first have to transform the input data to coordinates that fall within OpenGL's visible region.

## Compiling shaders

We take the source code for the vertex shader and store it in a const C string at the top of the code file for now:

```c++
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
```

In order for OpenGL to use the shader it has to dynamically compile it at run-time from its source code. The first thing we need to do is create a shader object, again referenced by an ID. So we store the vertex shader as an unsigned int and create the shader with glCreateShader:

```c++
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
```

We provide the type of shader we want to create as an argument to glCreateShader. Since we're creating a vertex shader we pass in `GL_VERTEX_SHADER`.

Next we attach the shader source code to the shader object and compile the shader:

```c++
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
```

Where `glShaderSource()`'s arguments are:

- The **first** argument is the shader object to compile to.
- The **second** argument specifies how many strings we're passing as source code, which is only one.
- The **third** parameter is the actual source code of the vertex shader
- We can leave the **fourth** parameter to `NULL`.

> [!IMPORTANT] Checking shaders' compile-time errors
> In order that we want to keep informed about shaders' compile output, we should add this code after the call to `glCompileShader()`:
>
> ```c++
> int  success;
> char infoLog[512];
> glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
> if (!success)
> {
>     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
>     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
> }
> ```

First we define an integer to indicate success and a storage container for the eventual error messages. Then we check if compilation was successful with `glGetShaderiv()`. If compilation failed, we should retrieve the error message with `glGetShaderInfoLog()` and print the error message.

## Fragment shader

The fragment shader is the second and final shader we're going to create for rendering a triangle. The fragment shader is all about calculating the color output of your pixels. To keep things simple the fragment shader will always output an orange-ish color.

> [!IMPORTANT] RGBA
> Colors in computer graphics are represented as an array of 4 values: the **r**ed, **g**reen, **b**lue and **a**lpha (opacity) component, commonly abbreviated to **RGBA**. When defining a color in OpenGL or GLSL we set the strength of each component to a value between 0.0 and 1.0. If, for example, we would set red to 1.0 and green to 1.0 we would get a mixture of both colors and get the color yellow. Given those 3 color components we can generate over 16 million different colors!

A basic fragment shader looks like:

```GLSL
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
```

The fragment shader only requires one output variable and that is a vector of size 4 that defines the final color output that we should calculate ourselves. We can declare output values with the `out` keyword, that we here promptly named FragColor. Next we simply assign a `vec4` to the color output as an orange color with an alpha value of 1.0 (1.0 being completely opaque).

The process for compiling a fragment shader is similar to the vertex shader, although this time we use the `GL_FRAGMENT_SHADER` constant as the shader type:

```c++
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
```

## Shader program

A **shader program object** is the final linked version of multiple shaders combined. To use the recently compiled shaders we have to link them to a shader program object and then activate this shader program when rendering objects. The activated shader program's shaders will be used when we issue render calls.

When **linking** the shaders into a program it links the outputs of each shader to the inputs of the next shader. This is also where you'll get linking errors if your outputs and inputs do not match.

Creating a shader program:

```c++
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
```

The `glCreateProgram()` function creates a program and returns the ID reference to the newly created program object. Now we need to attach the previously compiled shaders to the program object and then link them with `glLinkProgram()`:

```c++
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
```

> [!NOTE]
> Just like shader compilation we can also check if linking a shader program failed and retrieve the corresponding log. However, instead of using `glGetShaderiv()` and `glGetShaderInfoLog()` we now use:
>
> ```c++
> glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
> if (!success) {
>     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
>     // ...
> }
> ```

The result is a program object that we can activate by calling `glUseProgram()` with the newly created program object as its argument:

```c++
glUseProgram(shaderProgram);
```

Every shader and rendering call after glUseProgram will now use this program object (and thus the shaders).

> [!CAUTION]
> Don't forget to delete the shader objects once we've linked them into the program object; we no longer need them anymore:
>
> ```c++
> glDeleteShader(vertexShader);
> glDeleteShader(fragmentShader);
> ```

## Linking vertex attributes

The vertex shader allows us to specify any input we want in the form of vertex attributes and while this allows for great flexibility, it does mean we have to manually specify what part of our input data goes to which vertex attribute in the vertex shader. This means we have to specify how OpenGL should interpret the vertex data before rendering.

Our vertex buffer data is formatted as follows:

![vertex_data_buffer_format](/resources/buffer.png)

#### Properties:

- The position data is stored as 32-bit (4 byte) floating point values.
- Each position is composed of 3 of those values.
- There is no space (or other values) between each set of 3 values. The **values are tightly packed in the array**.
- The first value in the data is at the beginning of the buffer.

With this knowledge we can tell OpenGL how it should interpret the vertex data (per vertex attribute) using `glVertexAttribPointer()`:

```c++
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

The function glVertexAttribPointer has quite a few parameters so let's carefully walk through them:

1. The **first** parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (`location = 0`). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
2. The **second** argument specifies the size of the vertex attribute. The vertex attribute is a `vec3` so it is composed of 3 values.
3. The **third** argument specifies the type of the data which is `GL_FLOAT` (a `vec*` in GLSL consists of floating point values).
4. The **fourth** argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to `GL_TRUE`, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at `GL_FALSE`.
5. The **fifth** argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
6. The **sixth** parameter is of type `void*` and thus requires that weird cast. This is the offset of where the position data begins in the buffer. Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on.

> [!NOTE]
> Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from (you can have multiple VBOs) is determined by the VBO currently bound to `GL_ARRAY_BUFFER` when calling `glVertexAttribPointer()`. Since the previously defined VBO is still bound before calling `glVertexAttribPointer()` vertex attribute 0 is now associated with its vertex data.
