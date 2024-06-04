# Drawing on a window

## Graphics pipeline

The graphics pipeline's job can be split between two macro-steps: in the first one transforms 3D coordinates into 2D coordinates; the second one transforms 2D coordinates into actual colored pixels. Let's unwrap these two steps into the real graphics pipeline.

The graphics pipeline can be divided into several steps where each step requires the output of the previous step as its input. All of these steps are highly specialized (they have one specific function) and can easily be executed in parallel. Because of their parallel nature, graphics cards of today have thousands of small processing cores to quickly process your data within the graphics pipeline. The processing cores run small programs on the GPU for each step of the pipeline. These small programs are called **shaders**.

Some of these shaders are **configurable** by the developer which allows us to write our own shaders to replace the existing default shaders. This gives us much more fine-grained control over specific parts of the pipeline and because they run on the GPU, they can also save us valuable CPU time. Shaders are written in the OpenGL Shading Language (GLSL).
