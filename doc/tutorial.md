# Tutorial on Creating Geometric Content

## First Step

To complete these exercises, launch ipython with the following option:

```bash
ipython --gui=qt
```

> **Warning**
> If you don't use this option, you'll likely have problems displaying your results with the viewer.

Import all plantgl modules:

```python
from openalea.plantgl.all import *
```

---

## Exercises

### Hello World

1. Use the help on the `Box` primitive. What are the possible constructor arguments?
2. Consult the [plantgl documentation on the web](http://www-sop.inria.fr/virtualplants/doc/plantgl-2.6/).
3. Create a `Sphere` with radius 5.
4. Display it in the `Viewer`.

Your result should look like:

![Sphere with radius 5](_images/sphere_5.png){ width="40%", align="center" }

> **Note**
> In the viewer, you can change the camera angle by dragging, change the light direction by holding `ctrl` and dragging, and zoom in/out with the scroll wheel.

[Solution for Hello World](#solution-for-hello-world)

---

### Material

Create a shape with the previous sphere and associate a red material to it.

![Red sphere](_images/red_sphere.png){ width="40%", align="center" }

*Hint*: Use `Shape` and `Material`.

[Solution for Material](#solution-for-material)

---

### Transformation

Create a `Sphere` centered at point (-2,0,0).

![Translated sphere](_images/translated_sphere.png){ width="40%", align="center" }

*Hint*: Use the help on `Translated`.

[Solution for Transformation](#solution-for-transformation)

---

### Scene

Display a scene with two spheres, one red and one yellow, positioned respectively at (-5,0,0) and (-2,0,1).

![Scene with two spheres](_images/scene.png){ width="40%", align="center" }

*Hint*: Use the help on `Scene`.

[Solution for Scene](#solution-for-scene)

---

### Instantiation

In the previous scene, use the same sphere primitive for the translations. Change the radius of the sphere and observe the effect on the scene.

![Instantiated scene](_images/scene2.png){ width="40%", align="center" }

[Solution for Instantiation](#solution-for-instantiation)

---

### Mesh

1. Create a square with `QuadSet`.
2. Add different colors to the vertices of the square.
3. Create a cube with `QuadSet` and different colors for each face.
4. Create a cube with `QuadSet` and a different color for each point of each face.

![Square](_images/square.png){ width="40%" }
![Colored square](_images/color_square.png){ width="40%" }
![Cube 1](_images/cube1.png){ width="40%" }
![Cube 2](_images/cube2.png){ width="40%" }

[Solution for Mesh](#solution-for-mesh)

---

### Texture and Billboard

1. Add texture coordinates to the vertices of the square to display an image:

    ![Wood texture](_images/wood_texture.png){ width="20%", align="center" }

   *Hint*: Create a `Shape` with your square as geometry and `ImageTexture` as material.
2. Use transparency to display a leaf on a square:

    ![Leaf texture](_images/leaf_tex.png){ width="20%", align="center" }

3. Display the wood texture on the square but only part of the image (half width/height starting from 1/4,1/4).
4. Always display the same image on the 6 faces of a cube.
5. Create a cross with 5 squares and display a part of the texture on each square.

Results:

![Tex1](_images/tex1.png){ width="40%" }
![Tex2](_images/tex2.png){ width="40%" }
![Tex3](_images/tex3.png){ width="40%" }
![Tex4](_images/tex4.png){ width="40%" }
![Tex5](_images/tex5.png){ width="40%" }

[Solution for Texture and Billboard](#solution-for-texture-and-billboard)

---

### Discretization

1. Create a `QuadSet` corresponding to a cylinder.
2. Create a `TriangleSet` corresponding to a cylinder.
3. (Difficult) Create a `Sphere` as a truncated icosahedron.

Results:

![Cylinder quads](_images/cyl_quads.png){ width="40%" }
![Cylinder tris](_images/cyl_tris.png){ width="40%" }

[Solution for Discretization](#solution-for-discretization)

---

### Hulls

1. Create a python function to display a growing `AsymmetricHull`.

    ![Asym hull 1](_images/asym_hull1.png){ width="40%" }
    ![Asym hull 2](_images/asym_hull2.png){ width="40%" }
    ![Asym hull 3](_images/asym_hull3.png){ width="40%" }
    ![Asym hull 4](_images/asym_hull4.png){ width="40%" }

2. Create profiles with positive x coordinates using `Polyline2D`, `BezierCurve2D`, `NurbsCurve2D`.

    ![Polyline](_images/polyline.png){ width="40%" }
    ![Bezier](_images/beziercurve.png){ width="40%" }
    ![Nurbs](_images/nurbscurve.png){ width="40%" }

3. Create a `Swung` hull with the previous profiles at different angles between [0;2*pi].

    ![Profile hulls](_images/profile_hulls.png){ width="40%", align="center" }

4. Create a circle with `Polyline2D.Circle` and a closed profile representing the silhouette of a tree.

    ![Hulls circle](_images/hulls_circle.png){ width="40%", align="center" }

5. With these two profiles, create an `ExtrudedHull` to obtain:

    ![Extruded hulls](_images/extruded_hulls.png){ width="40%", align="center" }

[Solution for Hulls](#solution-for-hulls)

---

### Generalized Cylinder

With a circle and a 3D line, create a branch with an `Extrusion`. Expected result:

![Branches](_images/branches.png){ width="40%", align="center" }

[Solution for Generalized Cylinder](#solution-for-generalized-cylinder)

---

### Positioning Objects

1. Create a 2-meter trunk with a cylinder and pairs of lateral leaves every 50 cm.
   *Hint*: Use `Translated`, `AxisRotated`, `EulerRotated`, etc.
2. Same thing with decreasing leaf sizes.
   *Hint*: Use `Scaled`.
3. Same thing with a trunk bent 30° in the middle.

![Tree 1](_images/tree1.png){ width="30%" }
![Tree 2](_images/tree2.png){ width="30%" }
![Tree 3](_images/tree3.png){ width="30%" }

*Hint*: Pay attention to the order of transformations!

[Solution for Positioning Objects](#solution-for-positioning-objects)

---

## Solutions

### Solution for Hello World

Python script to get help on `Box`:

```python
# solutions_python/box.py
```

The arguments for initializing a `Box` are the x, y and z scales. To create a sphere with radius 5:

```python
# solutions_python/sphere_5.py
```

![Box](_images/box.png){ width="40%", align="center" }
![Sphere 5](_images/sphere_5.png){ width="40%", align="center" }

---

### Solution for Material

To associate a red material to a sphere:

```python
# solutions_python/material.py
```

![Red sphere](_images/red_sphere.png){ width="40%", align="center" }

---

### Solution for Transformation

To translate a sphere:

```python
# solutions_python/transformation.py
```

![Translated sphere](_images/translated_sphere.png){ width="40%", align="center" }

---

### Solution for Scene

To create a scene with two different spheres:

```python
# solutions_python/scene.py
```

![Scene](_images/scene.png){ width="40%", align="center" }

---

### Solution for Instantiation

To use the same sphere primitive in the scene:

```python
# solutions_python/scene2.py
```

![Instantiated scene](_images/scene2.png){ width="40%", align="center" }

---

### Solution for Mesh

Colored square:

```python
# solutions_python/carre.py
```

![QuadSet](_images/quadset.png){ width="40%", align="center" }

Cube with color per face:

```python
# solutions_python/cube.py
```

![Cube 1](_images/cube1.png){ width="40%", align="center" }

Cube with color per vertex:

```python
# solutions_python/cube2.py
```

![Cube 2](_images/cube2.png){ width="40%", align="center" }

---

### Solution for Texture and Billboard

Textured square:

```python
# solutions_python/square_tex.py
```

![Tex1](_images/tex1.png){ width="40%", align="center" }

Textured square with alpha:

```python
# solutions_python/square_tex2.py
```

![Tex2](_images/tex2.png){ width="40%", align="center" }

Partially textured square:

```python
# solutions_python/square_tex3.py
```

![Tex3](_images/tex3.png){ width="40%", align="center" }

Textured cube:

```python
# solutions_python/cube_tex.py
```

![Tex4](_images/tex4.png){ width="40%", align="center" }

Textured cross:

```python
# solutions_python/cross_tex.py
```

![Tex5](_images/tex5.png){ width="40%", align="center" }

---

### Solution for Discretization

Cylinder with QuadSet:

```python
# solutions_python/cyl_quadset.py
```

![Cylinder quads](_images/cyl_quads.png){ width="40%", align="center" }

Cylinder with TriangleSet:

```python
# solutions_python/cyl_triangleset.py
```

![Cylinder tris](_images/cyl_tris.png){ width="40%", align="center" }

---

### Solution for Hulls

Growing hulls:

```python
# solutions_python/asymmetric_hulls.py
```

![Asym hulls](_images/asym_hulls.png){ width="40%", align="center" }

Swung hulls:

```python
# solutions_python/hull_profil.py
```

![Profile hulls](_images/profile_hulls2.png){ width="40%", align="center" }

Extruded hulls:

```python
# solutions_python/hull_extruded.py
```

![Extruded hulls](_images/extruded_hulls.png){ width="40%", align="center" }

---

### Solution for Generalized Cylinder

Extruded branches:

```python
# solutions_python/branches.py
```

![Branches](_images/branches.png){ width="40%", align="center" }

---

### Solution for Positioning Objects

Tree with same-sized leaves:

```python
# solutions_python/tree1.py
```

![Tree 1](_images/tree1.png){ width="40%", align="center" }

Tree with decreasing leaf sizes:

```python
# solutions_python/tree2.py
```

![Tree 2](_images/tree2.png){ width="40%", align="center" }

Tree with bent trunk:

```python
# solutions_python/tree3.py
```

![Tree 3](_images/tree3.png){ width="40%", align="center" }