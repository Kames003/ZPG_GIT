#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;

class Observer
{
public:
    virtual ~Observer() = default;

    // ← GENERICKÝ názov, nie notifyShader, updateCamera!
    // observer nevie čo sa zmenilo, len, že sa niečo zmenilo
    virtual void notify(Subject* subject) = 0;
};

#endif