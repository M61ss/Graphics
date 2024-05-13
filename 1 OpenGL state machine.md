# How OpenGL works

About difference between **immediate** and **core version** and about OpenGL's **extensions** see more details on [this site](https://learnopengl.com/Getting-started/OpenGL).

## OpenGL is a state machine

OpenGL is by itself a large **state machine**: a collection of variables that define how OpenGL should currently operate. The state of OpenGL is commonly referred to as the **OpenGL context**. When using OpenGL, we often change its state by setting some options, manipulating some buffers and then render using the current context.

Whenever we tell OpenGL that we now want to draw lines instead of triangles for example, we change the state of OpenGL by changing some context variable that sets how OpenGL should draw. As soon as we change the context by telling OpenGL it should draw lines, the next drawing commands will now draw lines instead of triangles.

When working in OpenGL we will come across several state-changing functions that change the context and several state-using functions that perform some operations based on the current state of OpenGL. As long as you keep in mind that OpenGL is basically one large state machine, most of its functionality will make more sense.

## Objects and their reflections on the state machine

The OpenGL libraries are written in C and allows for many derivations in other languages, but in its core it remains a C-library. Since many of C's language-constructs do not translate that well to other higher-level languages, OpenGL was developed with several abstractions in mind. One of those abstractions are objects in OpenGL.

An **object** in OpenGL is a **collection of options** that represents a **subset of OpenGL's state**. For example, we could have an object that represents the settings of the drawing window; we could then set its size, how many colors it supports and so on. One could visualize an object as a C-like struct:

```c
struct object_name {
    float  option1;
    int    option2;
    char[] name;
};
```

Whenever we want to use objects, it generally looks something like this (with OpenGL's context visualized as a large struct):

```c
// The State of OpenGL machine
struct OpenGL_Context {
  	...
  	object_name* object_Window_Target;
  	...
};
```

```c
// create object
unsigned int objectId = 0;
glGenObject(1, &objectId);
// bind/assign object to context
glBindObject(GL_WINDOW_TARGET, objectId);
// set options of object currently bound to GL_WINDOW_TARGET
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH,  800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
// set context target back to default
glBindObject(GL_WINDOW_TARGET, 0);
```

This little piece of code is a workflow you'll frequently see when working with OpenGL. We first create an object and store a reference to it as an **id** (the real object's data is stored behind the scenes). Then we **bind the object** (using its id) to the target location of the context (the location of the example window object target is defined as GL_WINDOW_TARGET). Next we set the window options and finally we un-bind the object by setting the current object id of the window target to 0. The options we set are stored in the object referenced by objectId and restored as soon as we bind the object back to GL_WINDOW_TARGET.

The great thing about using these objects is that we can define more than one object in our application, set their options and whenever we start an operation that uses OpenGL's state, we bind the object with our preferred settings. There are objects for example that act as container objects for 3D model data (a house or a character) and whenever we want to draw one of them, we bind the object containing the model data that we want to draw (we first created and set options for these objects). Having several objects allows us to specify many models and whenever we want to draw a specific model, we simply bind the corresponding object before drawing without setting all their options again.
