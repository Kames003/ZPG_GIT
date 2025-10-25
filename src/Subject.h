#pragma once
#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <algorithm>

class Observer;

class Subject
{
protected:

    // Subject pozná len Observer*, zbernica
    std::vector<Observer*> observerCollection;

public:
    virtual ~Subject() = default;

    void attach(Observer* observer);
    void detach(Observer* observer);
    void notifyAll();
};

#endif