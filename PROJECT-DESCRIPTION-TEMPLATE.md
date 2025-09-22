# Diamond mining

7/2021 - Momcilo Tosic

In this graphics display/video game - you are piloting a space shuttle on a diamond mining expedition.
You need to try and mine diamonds and catch them with your ship before they get stolen.

The project features a directional and point light (spaceship engine) which are fully customizable via the GUI panel,
a realistic diamond and gem geode models, as well as the spacecraft and fuel model visualizations.

To compile and run:

- Run the initial setups described in the README.md.

- Run from the project directory (matf-rg-...):

```
cmake .
cd app
make APP
./APP
```

## Controls

W -> Camera moves forward

A -> Camera moves left

S -> Camera moves back

D -> Camera moves right

LArrow -> Spaceship moves left

RArrow -> Spaceship moves right

UArrow -> Spaceship moves forward

DArrow -> Spaceship moves back

Q -> Spaceship moves down

E -> Spaceship moves up

RMB (while held) -> Enter look around mode

Mouse movement (while holding down RMB) -> Look around

M -> Starts the mining process. After 2s a diamond spawns randomly near the geode and starts being pulled by the
spaceship. After 4s the diamond disappears (is stolen). If it arrives to the spaceship before that, it also disappears,
but is mined.

## Features

### Fundamental:

[ OK ] Model with lighting

Models

- Asteroid
- Gem geode (+ small gems that are mined)
- Spaceship
- Fuel ball

Lights

- Sun (directional)
- Fuel ball (spot)

[ OK ] Two types of lighting with customizable colors and movement through GUI or ACTIONS

GUI & controls to alter:

- Directional light
    - Direction X, Y, Z
    - Ambient R, G, B
    - Specular R, G, B
    - Diffuse R, G, B
- Point light
    - Position by moving the spaceship
    - Ambient R, G, B
    - Specular R, G, B
    - Diffuse R, G, B
    - Constant component
    - Linear component
    - Quadratic component

[ OK ] {ACTION_X} --- AFTER_M_SECONDS---Triggers---> {EVENT_A} ---> AFTER_N_SECONDS---Triggers---> {EVENT_B}

Implemented the following:

PRESS M --- AFTER_2_SECONDS ---> Gem appears ---> AFTER_4_SECONDS ---> Gem disappears

OR

PRESS M --- AFTER_2_SECONDS ---> Gem appears ---> AFTER_<4_SECONDS + GEM_CLOSE_TO_SHIP ---> Gem disappears + Mined+=1

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Instancing  
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping

### Group B:

[ ] Bloom with the use of HDR  
[ ] Deferred Shading  
[ ] Point Shadows  
[ ] SSAO

### Engine improvement:

[ ] ...

## Models:

https://www.turbosquid.com/3d-models/rocky-asteroid-4-3d-model-1406799 - asteroid

https://www.turbosquid.com/3d-models/blue-diamond-3d-model-1362791 - diamond

https://drive.google.com/drive/folders/1fzMwpNpkYmAu--jer71HfRSx0LWe7XJs - spaceship

https://free3d.com/3d-model/porous-sphere-v1--744627.html

## Textures

https://svs.gsfc.nasa.gov/4851 - Skybox (made into a cubemap using https://jaxry.github.io/panorama-to-cubemap/)

https://www.turbosquid.com/3d-models/rocky-asteroid-4-3d-model-1406799 - diffuse asteroid texture

https://media.istockphoto.com/id/1144905390/photo/vintage-green-background-rough-painted-wall-of-emerald-color-imperfect-plane-of-virid-colored.jpg?s=612x612&w=0&k=20&c=NxvqOSSRi9fOaomt2fqZOym9OJDvjn7L9pfh6jR6slg= -
green gem texture

https://drive.google.com/drive/folders/1fzMwpNpkYmAu--jer71HfRSx0LWe7XJs - spaceship texture

## Code taken and adapted from

https://stackoverflow.com for implementation details

https://learnopengl.com/Lighting/ for lighting shaders