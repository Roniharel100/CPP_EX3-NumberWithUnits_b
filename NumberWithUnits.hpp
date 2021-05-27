#pragma once

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <stdexcept>

namespace ariel
{
    class NumberWithUnits
    {
        double value;
        std::string type;
        static std::map<std::string, std::map<std::string, double>> unitsMap;

    public:
        NumberWithUnits(double value, const std::string &type);
        static void read_units(std::ifstream &file); 

        NumberWithUnits operator+(const NumberWithUnits &n);
        NumberWithUnits operator+() const;
        NumberWithUnits &operator+=(const NumberWithUnits &n);
        NumberWithUnits operator-(const NumberWithUnits &n);
        NumberWithUnits operator-() const;
        NumberWithUnits &operator-=(const NumberWithUnits &n);

        bool operator>(const NumberWithUnits &n) const;
        bool operator<(const NumberWithUnits &n) const;
        bool operator>=(const NumberWithUnits &n) const;
        bool operator<=(const NumberWithUnits &n) const;
        bool operator==(const NumberWithUnits &n) const;
        bool operator!=(const NumberWithUnits &n) const;

        NumberWithUnits operator++(int);
        NumberWithUnits operator--(int);
        NumberWithUnits &operator++();
        NumberWithUnits &operator--();


        NumberWithUnits operator*(double num);
        friend NumberWithUnits operator*(double num, NumberWithUnits const &n);

        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &n);
        friend std::istream &operator>>(std::istream &is, NumberWithUnits &n);
    };
}