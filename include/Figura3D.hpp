#pragma once
#include "Point3D.hpp"
#include "RotationStrategy.hpp"
#include <Eigen/Dense>
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

    // rzutowanie figury na p³aszczyznê osiow¹
    std::vector< std::array< double, 2 > > project(Point3D::Plane plane) const
    {
        std::vector< std::array< double, 2 > > result;
        result.reserve(points_.size());

        std::transform(points_.begin(), points_.end(), std::back_inserter(result), [&](const Point3D& p) {
            return p.project(plane);
        });

        return result;
    }

    // rzutowanie figury na dowoln¹ p³aszczyznê zdefiniowan¹ przez punkt i normaln¹
    std::vector< Eigen::Vector3d > project(const Eigen::Vector3d& planePoint, const Eigen::Vector3d& normal) const
    {
        std::vector< Eigen::Vector3d > result;
        result.reserve(points_.size());

        for (const auto& p : points_)
            result.push_back(p.projectToPlaneGlobal(planePoint, normal));

        return result;
    }

    // wyœwietlanie wspó³rzêdnych figury
    void print() const
    {
        std::cout << "Figura3D: ";
        for (const auto& p : points_)
            p.printPoint();
        std::cout << "\n";
    }
};
