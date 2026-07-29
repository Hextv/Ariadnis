# Ariadnis Project Roadmap

This document tracks the feature progress and implementation milestones for Ariadnis.

---

## 🟢 Phase 1: Core Terrain & Mesh Engine

- [ DONE ] **Task 1: Parametric Grid Generation**
  - Generate a basic flat 2D grid of vertices using `width`, `depth`, and `cellSize`.
  - Calculate index buffers to create triangle pairs for every cell.
  - Setup VAO, VBO, and EBO to render the grid in wireframe/solid mode.

- [ FUTURE ] **Task 2: Basic Lighting Shader**
  - Write vertex and fragment shaders for surface lighting.
  - Compute face and vertex normals for accurate low-poly shading.

- [ DONE ] **Task 3: Basic Camera System**
  - Implement a free-fly (FlyCam) to navigate around the grid.
  - Connect mouse and keyboard inputs via GLFW.

- [ DONE ] **Task 4: Dynamic Render Modes**
  - Cycle sequentially between Solid, Wireframe, and Hybrid modes using hotkey input ('O').

---

## 🟡 Phase 2: User Interface & Terrain Sculpting Brushes

- [ DONE ] **Task 1: Dear ImGui Navigation Bars [STRUCTURE]**
  - Implement top `TopBar` navigation component (`File`, `Edit`, `View`, `Help`).
  - Implement left vertical `Toolbar` component for selecting active tools.

- [ DONE ] **Task 2: Sculpting Tools System Architecture**
  - Define standard `Brush` base class interface (`radius`, `strength`, `apply()`).

- [ DONE ] **Task 3: Terrain Sculpting (Raise / Lower Brush)**
  - Implement 3D raycasting to the terrain grid.
  - Implement radial Gaussian/Cosine falloff logic for height modifications.
  - Add logic to elevate vertices on Left Click and lower vertices when holding `Shift`.