# Flocking Bees
 
### Flocking algorithm features:
* random spawning of a few hundred boids
* boids which are represented as images so they have to manage rotation and texture's UV
* screen edges avoidance
* a few tunable parameters--weights of alignment, cohesion and separation, speed, separate perception and separation radii (or radiuses)

### ImGUI window features:
* checkbox that allows to enable/disable drawing some debug info for the boids
* update timer
* text fields showing various parameters mentioned above

### Created using C++ 20 and ImGUI (based on my previously used project setup)
### Quadtree optimization (with benchmarks) and predator avoidance will be added very soon

![Screenshot 2025-03-05 113647](https://github.com/user-attachments/assets/4013fe56-e7ff-4d9d-872b-a9af96a94a75)
![Screenshot 2025-03-05 113721](https://github.com/user-attachments/assets/b5d19250-de39-4647-97d1-b74ff64c9012)

*Run Setup-Windows.bat to create Visual Studio project files and solution*
