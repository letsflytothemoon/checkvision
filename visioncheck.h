#pragma once
#include <cmath>

template <class Number_t>
struct Vector2
{
  Number_t x, y;
  Vector2(const Number_t& _x, const Number_t& _y) : x(_x), y(_y) {}

  inline Number_t Length()
  {
    return sqrt(x * x + y * y);
  }

  Vector2 Norm()
  {
    Number_t length = Length();
    if (length == 0) return Vector2(0, 0);
    return Vector2(x / length, y / length);
  }

  Vector2<Number_t> operator-(const Vector2<Number_t>& other) const
  {
    return Vector2<Number_t>(x - other.x, y - other.y);
  }

  Number_t operator*(const Vector2<Number_t>& other) const
  {
    return x * other.x + y * other.y;
  }
};




template <class Number_t>
class Unit
{
  Vector2<Number_t> position;
  Vector2<Number_t> direction;
  static Number_t halfFovCos;
  static Number_t sightDistance;
public:
  static void SetFov(Number_t _fov)
  {
    halfFovCos = cos(_fov / 2);
  }

  static void SetDistance(Number_t _sightDistance)
  {
    sightDistance = _sightDistance;
  }

  Unit(const Number_t& positionX,
    const Number_t& positionY,
    const Number_t& directionX,
    const Number_t& directionY) :
    position(positionX, positionY),
    direction(directionX, directionY) {}

  Unit(const Vector2<Number_t>& _position, const Vector2<Number_t> _direction) :
    position(_position),
    direction(_direction)
  {}

  inline bool CheckVisibility(const Unit<Number_t>& other) const
  {
    Vector2<Number_t> otherRelativePosition = other.position - this->position;
    if (abs(otherRelativePosition.x) > sightDistance)
      return false;
    if (abs(otherRelativePosition.y) > sightDistance)
      return false;
    if (direction * otherRelativePosition.Norm() < halfFovCos)
      return false;
    if (otherRelativePosition.Length() > sightDistance)
      return false;
    return true;
  }
};

template <class Number_t>
Number_t Unit<Number_t>::halfFovCos;

template <class Number_t>
Number_t Unit<Number_t>::sightDistance;