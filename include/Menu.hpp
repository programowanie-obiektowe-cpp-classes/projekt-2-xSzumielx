#pragma once
#include "Figura3D.hpp"
#include "RotationStrategy.hpp"
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <numbers>


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
                  << "3. Obrot figury\n"
                  << "4. Przesuniecie figury\n"
                  << "5. Rzut figury na wybrana plaszczyzne\n"
                  << "6. Wyswietlenie tego menu\n"
                  << "0. Wyjscie\n";
    }

    // tworzenie figury
    void createFigure()
    {
        try
        {
            std::cout << "Podaj liczbe punktow figury: ";
            int n;
            std::cin.exceptions(std::ios::failbit | std::ios::badbit);
            std::cin >> n;

            if (n <= 0)
            {
                std::string exception = "Liczba punktow musi byc wieksza od 0.";
                throw exception;
            }
            std::vector< Point3D > points;
            for (unsigned int i = 0; i < n; ++i)
            {
                double x, y, z;
                std::cout << "Punkt " << i + 1 << " (x y z): ";
                std::cin >> x >> y >> z;
                points.emplace_back(x, y, z);
            }

            fig_             = Figura3D(points);
            isCreated_ = true;
            std::cout << "Figura utworzona.\n";
        }
        catch (std::string exception)
        {
            std::cout << exception;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
        catch (...)
        {
            std::cout << "Nieprawidlowe wejscie. Wprowadz liczbe naturalna.\n";
            // czyszczenie strumienia i buforu
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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

        std::cout << "Wybierz os obrotu (X/Y/Z): ";
        char axis;
        std::cin >> axis;
        std::cout << "Podaj kat w stopniach: ";
        double deg;
        std::cin >> deg;
        double rad = deg * std::numbers::pi / 180.0;

        std::unique_ptr< RotationStrategy > strategy;
        switch (toupper(axis))
        {
        case 'X':
            strategy = std::make_unique< RotateAroundX >();
            break;
        case 'Y':
            strategy = std::make_unique< RotateAroundY >();
            break;
        case 'Z':
            strategy = std::make_unique< RotateAroundZ >();
            break;
        default:
            std::cout << "Nieznana os.\n";
            return;
        }

        fig_.rotate(*strategy, rad);
        std::cout << "Figura obrocona.\n";
    }

    // przesuwanie figury
    void translateFigure()
    {
        if (!isCreated_)
        {
            std::cout << "Najpierw stworz figure.\n";
            return;
        }
        double dx, dy, dz;
        std::cout << "Podaj przesuniecie (dx dy dz): ";
        std::cin >> dx >> dy >> dz;
        fig_.translate(dx, dy, dz);
        std::cout << "Figura przesunieta.\n";
    }

    // rzutowanie figury na p³aszczyznê
    void projectFigure()
    {
        if (!isCreated_)
        {
            std::cout << "Najpierw stworz figure.\n";
            return;
        }
        std::cout << "Wybierz plaszczyzne rzutowania (XY/YZ/XZ): ";
        std::string plane;
        std::cin >> plane;

        Point3D::Plane p;
        if (plane == "XY" || plane == "xy")
            p = Point3D::Plane::XY;
        else if (plane == "YZ" || plane == "yz")
            p = Point3D::Plane::YZ;
        else if (plane == "XZ" || plane == "xz")
            p = Point3D::Plane::XZ;
        else
        {
            std::cout << "Nieznana plaszczyzna.\n";
            return;
        }

        auto proj = fig_.project(p);
        std::cout << "Rzut figury: ";
        for (auto& pt : proj)
            std::cout << "(" << pt[0] << ", " << pt[1] << ") ";
        std::cout << "\n";
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
                int choice;
                std::cin >> choice;

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
            catch (...)
            {
                std::cout << "Nieznany blad.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            }
        }
    }
};
