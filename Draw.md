# Drawing on a window

## Graphic pipeline

If you need information about how a graphic pipeline works, see [this document](GraphicPipeline.md).

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

> [!IMPORTANT]
> **Normalized Device Coordinates (NDC)**
>
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
