# 3D Rendering Project (SR-02): Flat Shading



## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Contributing](#contributing)
- [License](#license)

## Overview
This project, SR-02, showcasing flat shading and Z-buffer rendering techniques. It utilizes SDL for window management and rendering.

## Prerequisites
Before you begin, ensure you have met the following requirements:
- **C++ Development Environment:** You should have a C++ development environment set up on your machine. You can use any C++ compiler of your choice.
- C++ compiler
- SDL2 library
- glm library

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

## Contributing
Contributions are what make the open-source community an incredible place to learn, inspire, and create. We welcome contributions from the community.

1. Fork the project.
2. Create your feature branch: `git checkout -b feature/your-feature-name`.
3. Commit your changes: `git commit -m 'Add some feature'`.
4. Push to the branch: `git push origin feature/your-feature-name`.
5. Open a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

