#pragma once
#ifndef ICAMERAOBSERVER_H
#define ICAMERAOBSERVER_H


class Camera;

// Observer interface pre kameru
class ICameraObserver
{
public:
    virtual ~ICameraObserver() = default;
    
    // Metóda volaná, keď sa kamera zmení
    // len pull pattern, dostávame len pointer na kameru
    virtual void update( Camera* camera) = 0;
};

#endif