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
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

> [!IMPORTANT] GLSL
> As you can see, GLSL looks similar to C. Each shader begins with a declaration of its version. Since OpenGL 3.3 and higher the version numbers of GLSL match the version of OpenGL (GLSL version 420 corresponds to OpenGL version 4.2 for example). We also explicitly mention we're using core profile functionality.
> 
> Next we declare all the input vertex attributes in the vertex shader with the `in` keyword. Right now we only care about position data so we only need a single vertex attribute. GLSL has a vector datatype that contains 1 to 4 floats based on its postfix digit. Since each vertex has a 3D coordinate we create a `vec3` (3 stands for 3D) input variable with the name `aPos`. We also specifically set the location of the input variable via `layout (location = 0)`.
>
> To set the output of the vertex shader we have to assign the position data to the predefined `gl_Position` variable which is a `vec4` behind the scenes. At the end of the main function, whatever we set to `gl_Position` will be used as the output of the vertex shader. Since our input is a vector of size 3 we have to cast this to a vector of size 4. We can do this by inserting the `vec3` values inside the constructor of `vec4` and set its `w` component to 1.0f.

> [!NOTE]
> The fourth component (`vec.w`) in `vec4` is a value used for **perspective division**.

> [!CAUTION]
> The current vertex shader is probably the most simple vertex shader we can imagine because we did no processing whatsoever on the input data and simply forwarded it to the shader's output. In real applications the input data is usually not already in normalized device coordinates so we first have to transform the input data to coordinates that fall within OpenGL's visible region.