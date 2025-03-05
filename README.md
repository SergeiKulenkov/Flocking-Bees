# Flocking Bees
 
### Created using C++ 20 and ImGUI (based on my previously used project setup)

## Flocking algorithm features:
* random spawning of a few hundred boids
* boids which are represented as images so they have to manage rotation and texture's UV
* screen edges avoidance
* a few tunable parameters--weights of alignment, cohesion and separation, speed, separate perception and separation radii (or radiuses)

### ImGUI window features:
* checkbox that allows to enable/disable drawing some debug info for the boids
* update timer
* text fields showing various parameters mentioned above

### Quadtree optimization (with benchmarks) and predator avoidance will be added very soon

*Run Setup-Windows.bat to create Visual Studio project files and solution*