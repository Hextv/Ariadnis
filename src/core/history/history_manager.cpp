#pragma once

#include <memory>
#include <vector>
#include "command.h"

class HistoryManager {
private:
    std::vector<std::unique_ptr<Command>> m_undoStack;
    std::vector<std::unique_ptr<Command>> m_redoStack;
    size_t m_maxHistorySize = 30; // Prevent runaway memory usage

public:
    HistoryManager() = default;

    // Call this when a tool finishes an action (e.g. Mouse Release)
    void pushCommand(std::unique_ptr<Command> cmd) {
        m_undoStack.push_back(std::move(cmd));

        // Clearing redo stack on new user actions is standard editor behavior
        m_redoStack.clear();

        // Keep stack within reasonable memory limits
        if (m_undoStack.size() > m_maxHistorySize) {
            m_undoStack.erase(m_undoStack.begin());
        }
    }

    void undo() {
        if (m_undoStack.empty()) return;

        auto cmd = std::move(m_undoStack.back());
        m_undoStack.pop_back();

        cmd->undo();

        m_redoStack.push_back(std::move(cmd));
    }

    void redo() {
        if (m_redoStack.empty()) return;

        auto cmd = std::move(m_redoStack.back());
        m_redoStack.pop_back();

        cmd->redo();

        m_undoStack.push_back(std::move(cmd));
    }

    bool canUndo() const { return !m_undoStack.empty(); }
    bool canRedo() const { return !m_redoStack.empty(); }

    void clear() {
        m_undoStack.clear();
        m_redoStack.clear();
    }
};