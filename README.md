# 3D Rendering Project (SR-01): Model Loading and Basic Rendering

![image](https://github.com/TheKiesling/CG-Project-SpaceTravel/assets/84196494/4199083d-0d3b-4d96-8d73-d94ea9e7c30a)


## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Contributing](#contributing)
- [License](#license)

## Overview
This project, SR-01, is the first part of a 3D rendering project aimed at developing a simple 3D renderer from scratch. In this part, we focus on loading 3D models in the OBJ format and rendering them using basic triangle rasterization.

## Prerequisites
Before you begin, ensure you have met the following requirements:
- **C++ Development Environment:** You should have a C++ development environment set up on your machine. You can use any C++ compiler of your choice.

## Getting Started
To get started with this project, follow these steps:
1. Clone this repository to your local machine.
2. Ensure you have the required C++ development environment set up.
3. Build and run the project using your preferred C++ development tools or command line.

## Usage
- Run the project to load a 3D model in the OBJ format and render it.
- The rendered output will be displayed in an SDL2 window.

## File Structure
The project structure is as follows:
- `main.cpp`: The main entry point of the program.
- `framebuffer.h`: Contains the Framebuffer class responsible for rendering.
- `loadOBJ.h`: Handles loading 3D models in the OBJ format.
- `shaders.h`: Contains the vertex shader used for basic transformation.
- `uniforms.h`: Defines the Uniforms structure for shader inputs.
- `vertex3.h`: Defines the Vertex3 structure for 3D vertex data.
- `face.h`: Contains the Face structure for storing face data.
- `color.h`: Defines the Color structure for color handling.

## Contributing
Contributions are what make the open-source community an incredible place to learn, inspire, and create. We welcome contributions from the community.

1. Fork the project.
2. Create your feature branch: `git checkout -b feature/your-feature-name`.
3. Commit your changes: `git commit -m 'Add some feature'`.
4. Push to the branch: `git push origin feature/your-feature-name`.
5. Open a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

