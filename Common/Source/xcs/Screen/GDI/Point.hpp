/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef XCSOAR_SCREEN_GDI_POINT_HPP
#define XCSOAR_SCREEN_GDI_POINT_HPP

#include <windows.h>

struct PixelSize;

typedef LONG PixelScalar;
typedef ULONG UPixelScalar;

struct RasterPoint : public tagPOINT {

  typedef LONG scalar_type;

  RasterPoint() = default;

  constexpr RasterPoint(PixelScalar _x, PixelScalar _y)
    :tagPOINT({_x, _y}) {}

  explicit constexpr RasterPoint(const POINT &other):tagPOINT(other) {}

  constexpr RasterPoint operator+(RasterPoint other) const {
    return { x + other.x, y + other.y };
  }

  constexpr RasterPoint operator-(RasterPoint other) const {
    return { x - other.x, y - other.y };
  }
  
  RasterPoint &operator+=(const RasterPoint& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  RasterPoint &operator-=(const RasterPoint& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr RasterPoint operator+(PixelSize size) const;
  constexpr RasterPoint operator-(PixelSize size) const;
};

inline bool operator==(const tagPOINT &first, const tagPOINT &second) {
  return first.x == second.x && first.y == second.y;
}

inline bool operator!=(const tagPOINT &first, const tagPOINT &second) {
  return !(first == second);
}

static_assert(sizeof(RasterPoint) == sizeof(POINT), "not same size");

struct PixelSize : public tagSIZE {
  PixelSize() = default;

#ifndef __WINE__
  constexpr PixelSize(PixelScalar _width, PixelScalar _height)
    :tagSIZE({_width, _height}) {}
#endif

  constexpr PixelSize(int _width, int _height)
    :tagSIZE({_width, _height}) {}

  constexpr PixelSize(unsigned _width, unsigned _height)
    :tagSIZE({PixelScalar(_width), PixelScalar(_height)}) {}

  explicit constexpr PixelSize(const SIZE &other):tagSIZE(other) {}

  bool operator==(const PixelSize &other) const {
    return cx == other.cx && cy == other.cy;
  }

  bool operator!=(const PixelSize &other) const {
    return !(*this == other);
  }

  constexpr PixelSize operator-(PixelSize size) const {
    return { cx - size.cx, cy - size.cy };
  }

  constexpr PixelSize operator/(PixelScalar f) const {
    return { cx / f, cy / f };
  }
};

static_assert(sizeof(PixelSize) == sizeof(SIZE), "not same size");

/**
 * @brief PixelRect structure and operations
 *
 * Provides support for creating and manipulating PixelRect structures
 *
 * @note This structure follows the GDI convention of the {right, bottom} coordinates being
 * immediately outside the rectangle being specified.
 */
struct PixelRect : public tagRECT {
  PixelRect() = default;

  constexpr PixelRect(PixelScalar _left, PixelScalar _top,
                      PixelScalar _right, PixelScalar _bottom)
    :tagRECT({_left, _top, _right, _bottom}) {}

  constexpr PixelRect(RasterPoint origin, PixelSize size)
    :tagRECT({origin.x, origin.y, origin.x + size.cx, origin.y + size.cy}) {}

  explicit constexpr PixelRect(PixelSize size)
    :tagRECT({0, 0, size.cx, size.cy}) {}

  explicit constexpr PixelRect(const RECT &other):tagRECT(other) {}

  void SetEmpty() {
    ::SetRectEmpty(this);
  }

  void Offset(int dx, int dy) {
    ::OffsetRect(this, dx, dy);
  }

  void Grow(int dx, int dy) {
    InflateRect(this, dx, dy);
  }

  void Grow(int d) {
    Grow(d, d);
  }

  constexpr RasterPoint GetOrigin() const {
    return { left, top };
  }

  constexpr PixelSize GetSize() const {
    return { right - left, bottom - top };
  }

  constexpr RasterPoint GetCenter() const {
    return { (left + right) / 2, (top + bottom) / 2 };
  }

  constexpr RasterPoint GetTopLeft() const {
    return { left, top };
  }

  constexpr RasterPoint GetTopRight() const {
    return { right, top };
  }

  constexpr RasterPoint GetBottomLeft() const {
    return { left, bottom };
  }

  constexpr RasterPoint GetBottomRight() const {
    return { right, bottom };
  }
  
  bool IsInside(RasterPoint pt) const {
    return ::PtInRect(this, pt);
  }
};

static_assert(sizeof(PixelRect) == sizeof(RECT), "not same size");

inline constexpr RasterPoint
RasterPoint::operator+(PixelSize size) const {
  return { x + size.cx, y + size.cy };
}

inline constexpr RasterPoint
RasterPoint::operator-(PixelSize size) const {
  return { x - size.cx, y - size.cy };
}

#endif
