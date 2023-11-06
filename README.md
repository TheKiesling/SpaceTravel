# Space Travel 3D Rendering Project

https://github.com/TheKiesling/CG-Project-SpaceTravel/assets/84196494/d67d5ec2-979a-41df-b1c7-f049ee38a149

## Table of Contents
- [Overview](#overview)
- [Getting Started](#getting-started)
- [Prerequisites](#prerequisites)
- [Features](#features)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

Space Travel is a 3D rendering project that simulates a space exploration experience. This project utilizes advanced rendering techniques and real-time 3D graphics to create a dynamic space environment. The rendering includes rendering of planets, a spacecraft, and celestial bodies. Users can navigate through this environment, and the project features dynamic camera movement, realistic shading, and interactive controls.

### Planets 

| ![image](https://github.com/TheKiesling/CG-Lab-Shaders/assets/84196494/eb9655ad-07ca-4b1f-91a8-d8ccb3f4154d)  | ![image](https://github.com/TheKiesling/CG-Lab-Shaders/assets/84196494/cbf3ba11-fd8b-4d0b-98ba-53fe7bb5d190)  | 
|------------  | -------------  |
| ![image](https://github.com/TheKiesling/CG-Lab-Shaders/assets/84196494/8262a118-b5c7-444f-a6ea-1c112f1a1d64)  | ![image](https://github.com/TheKiesling/CG-Lab-Shaders/assets/84196494/490be39a-b56c-4cc7-8ad6-bbd1a2428083) |

### Spacecraft

![image](https://github.com/TheKiesling/CG-Project-SpaceTravel/assets/84196494/4463f292-c380-40b3-bec6-df5911fa86fd)


## Getting Started

To get started with the Space Travel project, follow these steps:

1. Clone this repository to your local machine.
2. Set up your C++ development environment. Make sure you have the required libraries, including SDL2 and glm.
3. Build and run the project using your preferred C++ development tools or the command line.

## Prerequisites

Before running this project, ensure you have the following prerequisites in place:

- **C++ Development Environment:** You should have a C++ development environment set up on your machine.

## Features

The Space Travel project includes the following features:

- Real-time 3D rendering of celestial bodies, including planets, the sun, and spacecraft.
- Dynamic camera movements for a user-controlled space exploration experience.
- Advanced shading techniques, including Phong shading and texture mapping.
- Interactivity, allowing users to control the spacecraft's movement within the rendered environment.
- Realistic representation of the solar system with orbital paths.
- Beautiful visual effects and realistic texturing.

## Project Structure

The project is structured as follows:

- `main.cpp`: The main entry point of the program.
- `camera.h`: Contains the Camera structure for defining the viewpoint.
- `color.h`: Defines the Color structure for color handling.
- `face.h`: Contains the Face structure for storing face data.
- `fragment.h`: Defines the Fragment structure representing a fragment in rendering.
- `line.h`: Contains the line drawing algorithm and related functions.
- `loadOBJ.h`: Handles loading 3D models in the OBJ format.
- `point.h`: Contains functions for drawing points in rendering.
- `shaders.h`: Contains the vertex and fragment shaders.
- `triangle.h`: Contains functions for triangle rasterization.
- `uniforms.h`: Defines the Uniforms structure for shader inputs.
- `vertex.h`: Defines the Vertex structure for 3D vertex data.

## Usage

- Run the project to load the 3D model of the spacecraft and render the space environment.
- You can control the spacecraft's movement using keyboard inputs.
- Explore the rendered space environment and enjoy the realistic visual effects.

## Contributing

Contributions to the Space Travel project are welcome. Here's how you can contribute:

1. Fork the project.
2. Create a feature branch: `git checkout -b feature/your-feature-name`.
3. Commit your changes: `git commit -m 'Add some feature'`.
4. Push to the branch: `git push origin feature/your-feature-name`.
5. Open a pull request.

## License

This project is licensed under the MIT License. For details, see the [LICENSE](LICENSE) file.

---

Feel free to customize and extend this README to provide more detailed information about your specific project. If you need additional sections or specific details to be included, please let me know!
