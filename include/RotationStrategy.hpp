#pragma once
#include "Point3D.hpp"
#include <Eigen/Dense>
#include <memory>

// Polimorficzna klasa do obracania punktu 3D wokó³ osi
class RotationStrategy
{
public:
    virtual ~RotationStrategy() = default;  // wirtualny destruktor
    RotationStrategy()                            = default;
    RotationStrategy(const RotationStrategy&)     = default; // konstruktor kopiuj¹cy
    RotationStrategy(RotationStrategy&&) = default; // konstruktor przenosz¹cy
    RotationStrategy& operator=(const RotationStrategy&) = default; // kopiuj¹cy operator przypisania
    RotationStrategy& operator=(RotationStrategy&&) = default; // przenosz¹cy operator przypisania

    virtual void rotate(Point3D& p, double angle) const = 0; // metoda czysto wirtualna
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

// Obrót wokó³ dowolnej osi zdefiniowanej przez punkt i wektor
class RotateAroundAxis : public RotationStrategy
{
private:
    Eigen::Vector3d axisUnit_; // jednostkowy wektor osi
    Eigen::Vector3d pointOnAxis_; // punkt na osi

public:
    // Konstruktor przyjmuj¹cy dowolny wektor osi, normalizuj¹c go
    RotateAroundAxis(const Eigen::Vector3d& axis, const Eigen::Vector3d& point)
    {
        if (axis.norm() == 0.0)
            throw std::runtime_error("Os nie moze byc zerowa");

        axisUnit_    = axis.normalized();
        pointOnAxis_ = point;
    }

    void rotate(Point3D& p, double angle) const override
    {
        p.rotateAroundAxisThroughPoint(axisUnit_, pointOnAxis_, angle);
    }

    const Eigen::Vector3d& axis() const { return axisUnit_; }
};
