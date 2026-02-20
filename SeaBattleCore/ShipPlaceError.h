#pragma once
#include <stdexcept>

class ShipPlaceError : public std::logic_error {
public:
    ShipPlaceError(const std::string& msg) : std::logic_error(msg) {}
};