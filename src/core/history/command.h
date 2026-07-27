#pragma once

class Command {
public:
    virtual ~Command() = default;

    virtual void redo() = 0; // Called during Ctrl+Y
    virtual void undo() = 0; // Called during Ctrl+Z
};