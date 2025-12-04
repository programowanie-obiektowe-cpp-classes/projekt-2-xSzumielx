#pragma once
#include "Point3D.hpp"
#include "RotationStrategy.hpp"
#include <algorithm>
#include <memory>
#include <vector>

// Klasa do tworzenia figury 3D z punktów
class Figura3D
{
protected:
    std::vector< Point3D > points_;
public:
    Figura3D() = default;
    Figura3D(const std::vector< Point3D >& pts) : points_(pts) {}
    
    // przesuniêcie figury
    void translate(double dx, double dy, double dz)
    {
        for (auto& p : points_)
            p.translate(dx, dy, dz);
    }

    // obrót figury
    void rotate(const RotationStrategy& strategy, double angle)
    {
        for (auto& p : points_)
            strategy.rotate(p, angle);
    }

    // rzutowanie figury na p³aszczyznê
    std::vector< std::array< double, 2 > > project(Point3D::Plane plane) const
    {
        std::vector< std::array< double, 2 > > result;
        result.reserve(points_.size());

        std::transform(points_.begin(), points_.end(), std::back_inserter(result), [&](const Point3D& p) {
            return p.project(plane);
        });

        return result;
    }

    // wyœwietlanie wspó³rzêdnych figury
    void print() const
    {
        std::cout << "Figura3D: ";
        for (auto& p : points_)
            p.printPoint();
        std::cout << "\n";
    }
};
