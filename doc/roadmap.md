# Ariadnis Project Roadmap

This document tracks the feature progress and implementation milestones for Ariadnis.

---

## 🟢 Phase 1: Core Terrain & Mesh Engine

- [ ] **Task 1: Parametric Grid Generation**
  - Generate a basic flat 2D grid of vertices using `width`, `depth`, and `cellSize`.
  - Calculate index buffers to create triangle pairs for every cell.
  - Setup VAO, VBO, and EBO to render the grid in wireframe/solid mode.

- [ ] **Task 2: Basic Lighting Shader**
  - Write vertex and fragment shaders for surface lighting.
  - Compute face and vertex normals for accurate low-poly shading.

- [ ] **Task 3: Basic Camera System**
  - Implement a free-fly (FlyCam) to navigate around the grid.
  - Connect mouse and keyboard inputs via GLFW.
