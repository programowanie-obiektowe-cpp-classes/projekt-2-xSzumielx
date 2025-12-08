#pragma once
#include "Figura3D.hpp"
#include "RotationStrategy.hpp"
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <Eigen/Dense>
#include <cmath>
#include <numbers>

#include <sstream>
#include <type_traits>

template < typename T >
T readLine()
{
    static_assert(std::is_same_v< T, int > || std::is_same_v< T, double >,
                  "readLine<T>() obsluguje tylko int oraz double");

    while (true)
    {
        std::string line;

        if (!std::getline(std::cin, line))
            throw std::runtime_error("Blad wejscia");

        if (line.empty())
            continue;

        std::stringstream ss(line);
        T                 value;
        char              extra;

        if (!(ss >> value) || (ss >> extra))
        {
            if constexpr (std::is_same_v< T, int >)
                std::cout << "Nieprawidlowe wejscie. Wprowadz liczbe calkowita: \n";
            else
                std::cout << "Nieprawidlowe wejscie. Wprowadz liczbe: \n";

            continue;
        }

        return value;
    }
}


// Menu do interakcji z u¿ytkownikiem
class Menu
{
private:
    Figura3D fig_{}; // pusta figura na start
    bool     isCreated_ = false;

    void showHelp() const
    {
        std::cout << "\nDostepne opcje:\n"
                  << "1. Tworzenie figury 3D\n"
                  << "2. Wyswietlanie aktualnej figury\n"
                  << "3. Obrot figury (X/Y/Z/A - dowolna os)\n"
                  << "4. Przesuniecie figury\n"
                  << "5. Rzut figury na plaszczyzne (XY/YZ/XZ/A - dowolna)\n"
                  << "6. Wyswietlenie tego menu\n"
                  << "0. Wyjscie\n";
    }
    

    // tworzenie figury
    void createFigure()
    {
        try
        {
            std::cout << "Podaj liczbe punktow figury: ";
            int n = readLine<int>();

            if (n <= 0)
            {
                throw std::runtime_error("Liczba punktow musi byc wieksza od 0.");
            }

            // wczytanie punktów
            std::vector< Point3D > points;
            for (int i = 0; i < n; ++i)
            {
                std::cout << "Punkt " << (i + 1) << " (x y z): \n";
                std::cout << "x = ";
                double x = readLine< double >();
                std::cout << "y = ";
                double y = readLine< double >();
                std::cout << "z = ";
                double z = readLine< double >();
                points.emplace_back(x, y, z);
            }

            fig_       = Figura3D(points);
            isCreated_ = true;
            std::cout << "Figura utworzona.\n";
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "Blad: " << e.what() << "\n";
        }
    }

    // wyœwietlanie figury
    void printFigure() const
    {
        if (!isCreated_)
        {
            std::cout << "Brak figury do wyswietlenia.\n";
            return;
        }
        fig_.print();
    }

    // obracanie figury
    void rotateFigure()
    {
        if (!isCreated_)
        {
            std::cout << "Najpierw stworz figure.\n";
            return;
        }

        try
        {
            // wczytanie osi obrtu
            std::cout << "Wybierz os obrotu (X/Y/Z/A - dowolna): ";
            std::string axisLine;

            while (axisLine.empty())
            {
                if (!std::getline(std::cin, axisLine))
                    throw std::runtime_error("Blad wejscia");
            }
            char axis = axisLine[0];

            // wczytanie k¹ta obrotu
            std::cout << "Podaj kat w stopniach: ";
            double deg = readLine< double >();
            double rad = deg * std::numbers::pi / 180.0; // zamiana stopni na radiany

            std::unique_ptr< RotationStrategy > strategy;

            if (axis == 'X' || axis == 'x')
            {
                strategy = std::make_unique< RotateAroundX >();
            }
            else if (axis == 'Y' || axis == 'y')
            {
                strategy = std::make_unique< RotateAroundY >();
            }
            else if (axis == 'Z' || axis == 'z')
            {
                strategy = std::make_unique< RotateAroundZ >();
            }
            else if (axis == 'A' || axis == 'a')
            {
                std::cout << "Podaj punkt nalezacy do osi obrotu (px py pz): \n";
                std::cout << "px = ";
                double px = readLine< double >();
                std::cout << "py = ";
                double py = readLine< double >();
                std::cout << "pz = ";
                double pz = readLine< double >();
                Eigen::Vector3d axisPoint(px, py, pz);

                // wczytanie wektora osi obrotu
                std::cout << "Podaj wektor osi obrotu (ax ay az): \n";
                std::cout << "ax = ";
                double ax = readLine< double >();
                std::cout << "ay = ";
                double ay = readLine< double >();
                std::cout << "az = ";
                double az = readLine< double >();
                Eigen::Vector3d axisVec(ax, ay, az);
                if (axisVec.norm() == 0.0)
                    throw std::runtime_error("Wektor osi nie moze byc zerowy");

                strategy = std::make_unique< RotateAroundAxis >(axisVec, axisPoint);
            }
            else
            {
                throw std::runtime_error("Nieznana os obrotu");
            }

            fig_.rotate(*strategy, rad);
            std::cout << "Figura obrocona.\n";
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "Blad: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Nieznany blad przy obrocie.\n";
        }
    }

    // przesuwanie figury
    void translateFigure()
    {
        if (!isCreated_)
        {
            std::cout << "Najpierw stworz figure.\n";
            return;
        }
        try
        {
            std::cout << "Podaj przesuniecie (dx dy dz): \n";
            std::cout << "dx = ";
            double dx = readLine< double >();
            std::cout << "dy = ";
            double dy = readLine< double >();
            std::cout << "dz = ";
            double dz = readLine< double >();
            fig_.translate(dx, dy, dz);
            std::cout << "Figura przesunieta.\n";
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "Blad: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Nieznany blad przy przesunieciu.\n";
        }
    }

    // rzutowanie figury na p³aszczyznê
    void projectFigure()
    {
        if (!isCreated_)
        {
            std::cout << "Najpierw stworz figure.\n";
            return;
        }

        try
        {
            std::cout << "Wybierz plaszczyzne rzutowania (XY/YZ/XZ/A - dowolna): ";
            std::string plane;

            while (plane.empty())
            {
                if (!std::getline(std::cin, plane))
                    throw std::runtime_error("Blad wejscia");
            }

            if (plane == "XY" || plane == "xy")
            {
                auto proj = fig_.project(Point3D::Plane::XY);
                std::cout << "Rzut figury na XY: ";
                for (auto& pt : proj)
                    std::cout << "(" << Point3D::cleanup(pt[0]) << ", " << Point3D::cleanup(pt[1]) << ") ";
                std::cout << "\n";
            }
            else if (plane == "YZ" || plane == "yz")
            {
                auto proj = fig_.project(Point3D::Plane::YZ);
                std::cout << "Rzut figury na YZ: ";
                for (auto& pt : proj)
                    std::cout << "(" << Point3D::cleanup(pt[0]) << ", " << Point3D::cleanup(pt[1]) << ") ";
                std::cout << "\n";
            }
            else if (plane == "XZ" || plane == "xz")
            {
                auto proj = fig_.project(Point3D::Plane::XZ);
                std::cout << "Rzut figury na XZ: ";
                for (auto& pt : proj)
                    std::cout << "(" << Point3D::cleanup(pt[0]) << ", " << Point3D::cleanup(pt[1]) << ") ";
                std::cout << "\n";
            }
            else if (plane == "A" || plane == "a")
            {
                // rzut na p³aszczyznê zdefiniowan¹ przez punkt i normaln¹
                std::cout << "Podaj punkt na plaszczyznie (x y z): \n";
                std::cout << "px = ";
                double px = readLine< double >();
                std::cout << "py = ";
                double py = readLine< double >();
                std::cout << "pz = ";
                double pz = readLine< double >();
                Eigen::Vector3d planePoint(px, py, pz);

                std::cout << "Podaj wektor normalny (nx ny nz): \n";
                std::cout << "nx = ";
                double nx = readLine< double >();
                std::cout << "ny = ";
                double ny = readLine< double >();
                std::cout << "nz = ";
                double nz = readLine< double >();

                Eigen::Vector3d normal(nx, ny, nz);
                if (normal.norm() == 0.0)
                    throw std::runtime_error("Normala nie moze byc wektorem zerowym");

                auto proj = fig_.project(planePoint, normal);
                std::cout << "Rzut figury na zadana plaszczyzne: ";
                for (auto& pt : proj)
                    std::cout << "("<< Point3D::cleanup(pt.x()) << ", " << Point3D::cleanup(pt.y()) << ", " << Point3D::cleanup(pt.z()) << ") ";
                std::cout << "\n";
            }
            else
            {
                throw std::runtime_error("Nieznana opcja plaszczyzny");
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "Blad: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Nieznany blad przy rzutowaniu.\n";
        }
    }

public:
    // G³ówna pêtla programu
    void run()
    {
        showHelp();
        while (true)
        {
            try
            {
                std::cout << "\nWybierz opcje (0-6): ";
                std::cin.exceptions(std::ios::failbit | std::ios::badbit);
                int choice = readLine<int>();

                switch (choice)
                {
                case 0:
                    return;
                case 1:
                    createFigure();
                    break;
                case 2:
                    printFigure();
                    break;
                case 3:
                    rotateFigure();
                    break;
                case 4:
                    translateFigure();
                    break;
                case 5:
                    projectFigure();
                    break;
                case 6:
                    showHelp();
                    break;
                default:
                    std::cout << "Nieznana opcja.\n";
                }
            }
            catch (const std::ios_base::failure&)
            {
                std::cout << "Nieprawidlowe wejscie.\n";
                // czyszczenie strumienia i buforu
                std::cin.clear();
                std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            }
            catch (const std::runtime_error& e)
            {
                std::cout << "Blad: " << e.what() << "\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            }
            catch (...)
            {
                std::cout << "Nieznany blad.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            }
        }
    }
};
