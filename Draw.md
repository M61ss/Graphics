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
>
> Since we want to draw a 2D triangle, z coordinate must be 0 for every vertex. This way the **depth** of the triangle remains the same making it look like it's 2D.

> [!IMPORTANT]
>
> **Normalized Device Coordinates (NDC)**
>
> Once your vertex coordinates have been processed in the vertex shader, they should be in normalized device coordinates which is a small space where the x, y and z values vary from -1.0 to 1.0. Any coordinates that fall outside this range will be discarded/clipped and won't be visible on your screen.
\
Since NDC are different from screen-space coordinates, NDC will then be transformed to screen-space coordinates via the viewport transform using the data you provided with `glViewport()`. The resulting screen-space coordinates are then transformed to fragments as inputs to your fragment shader.


