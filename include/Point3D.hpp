#pragma once
#include <Eigen/Dense>
#include <array>
#include <iostream>
#include <stdexcept>

// Klasa do wykonywania operacji na punkcie 3D
class Point3D
{
private:
    Eigen::Vector3d vec_; // tworzenie wektora 3D

public:
    Point3D() = default;
    Point3D(double x, double y, double z) : vec_(x, y, z) {}

    double x() const { return vec_.x(); }
    double y() const { return vec_.y(); }
    double z() const { return vec_.z(); }

    // przesuniêcie punktu
    void translate(double dx, double dy, double dz) { vec_ += Eigen::Vector3d(dx, dy, dz); }

    // obrót punktu wokó³ osi X
    void rotateX(double angle)
    {
        Eigen::Matrix3d Rx;
        double          c = std::cos(angle), s = std::sin(angle);
        Rx << 1, 0, 0, 0, c, -s, 0, s, c;
        vec_ = Rx * vec_;
    }

    // obrót punktu wokó³ osi Y
    void rotateY(double angle)
    {
        Eigen::Matrix3d Ry;
        double          c = std::cos(angle), s = std::sin(angle);
        Ry << c, 0, s, 0, 1, 0, -s, 0, c;
        vec_ = Ry * vec_;
    }

    // obrót punktu wokó³ osi Z
    void rotateZ(double angle)
    {
        Eigen::Matrix3d Rz;
        double          c = std::cos(angle), s = std::sin(angle);
        Rz << c, -s, 0, s, c, 0, 0, 0, 1;
        vec_ = Rz * vec_;
    }

    // definicja p³aszczyzn
    enum class Plane
    {
        XY,
        YZ,
        XZ
    };

    // rzutowanie punktu na p³aszczyznê
    std::array< double, 2 > project(Plane plane) const
    {
        switch (plane)
        {
        case Plane::XY:
            return {vec_.x(), vec_.y()};
        case Plane::YZ:
            return {vec_.y(), vec_.z()};
        case Plane::XZ:
            return {vec_.x(), vec_.z()};
        }
        throw std::runtime_error("Nieznana p³aszczyzna");
    }

    // wyœwietlanie wspó³rzêdnych punktu
    void printPoint() const { std::cout << "(" << vec_.x() << ", " << vec_.y() << ", " << vec_.z() << ")"; }
};
