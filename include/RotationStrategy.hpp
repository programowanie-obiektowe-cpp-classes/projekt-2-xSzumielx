#pragma once
#include "Point3D.hpp"

// Polimorficzna klasa do obracania punktu 3D wokó³ osi
class RotationStrategy
{
public:
    virtual ~RotationStrategy() = default; // wirtualny destruktor
    virtual void rotate(Point3D& p, double angle) const = 0;
};

// Obrót wokó³ osi X
class RotateAroundX : public RotationStrategy
{
public:
    void rotate(Point3D& p, double angle) const override { p.rotateX(angle); }
};

// Obrót wokó³ osi Y
class RotateAroundY : public RotationStrategy
{
public:
    void rotate(Point3D& p, double angle) const override { p.rotateY(angle); }
};

// Obrót wokó³ osi Z
class RotateAroundZ : public RotationStrategy
{
public:
    void rotate(Point3D& p, double angle) const override { p.rotateZ(angle); }
};
