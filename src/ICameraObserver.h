#pragma once
#ifndef ICAMERAOBSERVER_H
#define ICAMERAOBSERVER_H

// Forward declaration
class Camera;

// Observer interface pre kameru
class ICameraObserver
{
public:
    virtual ~ICameraObserver() = default;
    
    // Metóda volaná, keď sa kamera zmení
    virtual void update(Camera* camera) = 0;
};

#endif