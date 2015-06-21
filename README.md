# CG Project
OpenGL Project for the course 'Computer Graphics'
At the end there will be a merry-go-around with ultimative effects, like bumpmapping and all this fancy stuff...

current state: obj, textures, lightning, billboard(spherical and cylindrical) and bumpmapping(normal+specularmapping) are working, even a big scene like the rungholt castle with 12 Million vertices seems to be no problem(provided that the gpu can handle it)

The one and only Merry-Go-Arround:

![MerryGoAround](https://raw.githubusercontent.com/Philipp-M/CG-Project/master/images/merryBumpmapping.jpg)

Rungholt with lightning:

![Rungholt](https://raw.githubusercontent.com/Philipp-M/CG-Project/master/images/rungholt.jpg)

## Controls

- t - toggle automatic mode
- w, a, s, d - move around the scene
- mouse-movement - look around the scene
- +, -  - increase or decrease the movementspeed(w or s)
- Shift +, Shift -  - increase or decrease the merry-go-around speed

Lightning Control

- y - toggle diffuse lightning
- x - toggle specular lightning
- c - toggle ambient lightning
- v - set random hue for all lights
- b - increase value of all lights
- n - decrease value of all lights
- m - increase saturation of all lights
- , - decrease saturation of all lights
