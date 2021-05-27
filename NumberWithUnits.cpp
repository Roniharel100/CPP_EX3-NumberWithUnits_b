#include "NumberWithUnits.hpp"
#include <string>
#include <sstream>
#include <iostream>

using namespace ariel;
using namespace std;

map<string, std::map<string, double>> NumberWithUnits::unitsMap;

NumberWithUnits::NumberWithUnits(double value, const string &type)
{
    if (unitsMap.count(type) == 0)
    {
        throw invalid_argument{"error!"};
    }
    this->type = type;
    this->value = value;
}

void NumberWithUnits::read_units(ifstream &units_file)
{
    string line, unit1, unit2;
    double value1 = 0, value2 = 0;

    while (getline(units_file, line))
    {
        istringstream ss(line);
        ss >> value1 >> unit1 >> line >> value2 >> unit2;

        unitsMap[unit1][unit2] = value2;
        unitsMap[unit2][unit1] = value1 / value2;

        for (auto &current1 : unitsMap[unit1])
        {
            unitsMap[current1.first][unit2] = value1 / (current1.second * unitsMap[unit2][unit1]);
            unitsMap[unit2][current1.first] = current1.second * unitsMap[unit2][unit1];
        }

        for (auto &current2 : unitsMap[unit2])
        {
            unitsMap[current2.first][unit1] = value1 / (current2.second * unitsMap[unit1][unit2]);
            unitsMap[unit1][current2.first] = current2.second * unitsMap[unit1][unit2];
        }
    }
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &n)
{
    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return NumberWithUnits(result + value, type);
}

NumberWithUnits NumberWithUnits::operator+() const
{
    return NumberWithUnits(+value, type);
}

NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &n)
{
    *this = *this + n;
    return *this;
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &n)
{
    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return NumberWithUnits(value - result, type);
}

NumberWithUnits NumberWithUnits::operator-() const
{
    return NumberWithUnits(-value, type);
}

NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &n)
{
    *this = *this - n;
    return *this;
}

bool NumberWithUnits::operator>(const NumberWithUnits &n) const
{
    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return value > result;
}

bool NumberWithUnits::operator<(const NumberWithUnits &n) const
{
    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return value < result;
}

bool NumberWithUnits::operator>=(const NumberWithUnits &n) const
{
    const float TEMP = 0.0001;

    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return value > result || abs(value - result) <= TEMP;
}

bool NumberWithUnits::operator<=(const NumberWithUnits &n) const
{
    const float TEMP = 0.0001;

    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return value < result || abs(value - result) <= TEMP;
}

bool NumberWithUnits::operator==(const NumberWithUnits &n) const
{
    const float TEMP = 0.0001;

    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return abs(value - result) <= TEMP;
}

bool NumberWithUnits::operator!=(const NumberWithUnits &n) const
{
    if (unitsMap.at(type).count(n.type) == 0 || unitsMap.at(n.type).count(type) == 0)
    {
        throw invalid_argument{"different types"};
    }
    double result = n.value * unitsMap.at(n.type).at(type);
    return !(value == result);
}

NumberWithUnits NumberWithUnits::operator++(int)
{
    NumberWithUnits num = *this;
    value += 1;
    return num;
}

NumberWithUnits NumberWithUnits::operator--(int)
{
    NumberWithUnits num = *this;
    value -= 1;
    return num;
}

NumberWithUnits &NumberWithUnits::operator++()
{
    value++;
    return *this;
}

NumberWithUnits &NumberWithUnits::operator--()
{
    value--;
    return *this;
}

NumberWithUnits NumberWithUnits::operator*(double num)
{
    return NumberWithUnits(num * value, type);
}

NumberWithUnits ariel::operator*(double num, NumberWithUnits const &n)
{
    return NumberWithUnits(num * n.value, n.type);
}

ostream &ariel::operator<<(ostream &os, const NumberWithUnits &n)
{
    return os << n.value << "[" << n.type << "]";
}

istream &ariel::operator>>(istream &is, NumberWithUnits &n)
{
    char A = ' ';
    double value = 0;
    string type;

    is >> value >> A >> type;
    n.value = value;
    if (type.back() == ']')
    {
        type = type.substr(0, type.size() - 1);
    }
    else
    {
        is >> A;
    }
    n.type = type;

    if (NumberWithUnits::unitsMap.count(type) == 0)
    {
        throw invalid_argument("illigal type");
    }
    return is;
}