#pragma once
#ifndef ILIGHT_OBSERVER_H
#define ILIGHT_OBSERVER_H

class Light;

class ILightObserver
{
public:
    virtual ~ILightObserver() = default;
    virtual void onLightUpdate(Light* light) = 0;
};

#endif