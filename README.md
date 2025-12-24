# OpenGLPhysicsEngine

This project is an OpenGL-based physics engine designed for rendering and simulating 3D environments. It includes various components for handling graphics, user input, and physics calculations.

## Project Structure

- **models/**: Directory for 3D model files.

- **shaders/**: Contains GLSL shader files for rendering.

- **src/**: Contains the main source code for the engine.
  - **main.cpp**: Entry point of the application.
  
- **common/**: Contains shared utility files and classes used across the project.

- **external/**: Includes external libraries and dependencies required for the project.
  
- **distrib/**: Contains distribution-related files and scripts.
  
- **CMakeLists.txt**: Main build configuration file for the project.

## Building the Project

To build the project, navigate to the root directory of the project and run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

After building the project, you can run the executable to start the physics engine. The application will initialize the OpenGL context and enter the rendering loop.

## Dependencies

This project relies on several external libraries, which are included in the `external/` directory. Make sure to check the respective library documentation for any additional setup required.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.