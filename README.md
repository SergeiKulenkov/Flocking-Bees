# Flocking Bees
 
### Flocking algorithm features:
* random spawning of a few hundred boids and a few predators
* boids and predators which are represented as images so they have to manage rotation and texture's UV
* screen edges and predators avoidance
* predators avoid each other
* distance calculations without unnecessary square roots
* a few tunable parameters--weights of alignment, cohesion and separation, speed, separate perception, separation and predator avoiadance radii (or radiuses)

### ImGUI window features:
* checkbox that allows to enable/disable drawing some debug info for the boids and predators
* update timer
* text fields showing various parameters mentioned above

### Created using C++ 20 and ImGUI (based on my previously used project setup)
### Quadtree optimization (with benchmarks) will be added very soon

First version:
![Screenshot 2025-03-05 113721](https://github.com/user-attachments/assets/b5d19250-de39-4647-97d1-b74ff64c9012)
Current version with quadtree and predators:

*Run Setup-Windows.bat to create Visual Studio project files and solution*
