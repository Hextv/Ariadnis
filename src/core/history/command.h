#pragma once

#include <vector>
#include "../terrain/terrain.h"

class Command {
public:
    virtual ~Command() = default;

    virtual void redo() = 0; // Called during Ctrl+Y
    virtual void undo() = 0; // Called during Ctrl+Z
};

class TerrainModifyCommand : public Command {
private:
    Terrain& m_terrain;
    std::vector<float> m_beforeVertices;
    std::vector<float> m_afterVertices;

public:
    TerrainModifyCommand(Terrain& terrain, std::vector<float> before, std::vector<float> after)
        : m_terrain(terrain), m_beforeVertices(std::move(before)), m_afterVertices(std::move(after)) {}

    void undo() override {
        m_terrain.getVertices() = m_beforeVertices;
        m_terrain.updateBuffers();
    }

    void redo() override {
        m_terrain.getVertices() = m_afterVertices;
        m_terrain.updateBuffers();
    }
};
