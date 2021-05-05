
#pragma once


#include <utility>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <treemap/treemap_api.h>


class TREEMAP_API Rect
{
public:
    // describes conceptual rotation in CCW direction
    enum Rotation {
        R000 = 0,
        R090 = 1,
        R180 = 2,
        R270 = 3,
    };

    enum CurveOrientation {
        CW  = 0,
        CCW = 1,
    };

    struct Point {
        float x,y;
        Point(float x_, float y_) : x(x_), y(y_) {}
        Point(const glm::vec2 & vec) : x(vec.x), y(vec.y) {}
        operator glm::vec2() const {return {x,y}; }
        bool operator==(const Point & other) const { return x == other.x && y == other.y; }
        Point & operator+=(const Point & other) { x += other.x; y += other.y; return *this; }
        Point & operator-=(const Point & other) { x -= other.x; y -= other.y; return *this; }
        Point & operator*=(const Point & other) { x *= other.x; y *= other.y; return *this; }
        Point & operator/=(const Point & other) { x /= other.x; y /= other.y; return *this; }
        Point & operator+=(float v) { x += v; y += v; return *this; }
        Point & operator-=(float v) { x -= v; y -= v; return *this; }
        Point & operator*=(float v) { x *= v; y *= v; return *this; }
        Point & operator/=(float v) { x /= v; y /= v; return *this; }
    };

public:
    Rect();
    Rect(Rotation rotation, CurveOrientation orientation);
    Rect(float left, float bottom, float width, float height);
    Rect(float left, float bottom, float width, float height, Rotation rotation, CurveOrientation orientation);
    Rect(const glm::vec2 & pos, const glm::vec2 & extent);
    Rect(const glm::vec2 & pos, const glm::vec2 & extent, Rotation rotation, CurveOrientation orientation);
    Rect(glm::vec2 && pos, glm::vec2 && extent);
    Rect(glm::vec2 && pos, glm::vec2 && extent, Rotation rotation, CurveOrientation orientation);
    Rect(const glm::vec4 & rect);
    Rect(const glm::vec4 & rect, Rotation rotation, CurveOrientation orientation);
    Rect(glm::vec4 && rect);
    Rect(glm::vec4 && rect, Rotation rotation, CurveOrientation orientation);
    Rect(const Rect & other, Rotation rotation, CurveOrientation orientation);

    Rect(const Rect & other);
    Rect(Rect && other);

    Rect & operator=(const Rect & other);
    Rect & operator=(Rect && other);

    void multiply(float factor);
    void divide(float factor);
    void scale(float x, float y);

    float left() const;
    float top() const;
    float right() const;
    float bottom() const;

    float width() const;
    float height() const;

    glm::vec2 center() const;
    glm::vec2 leftCenter() const;
    glm::vec2 topLeft() const;
    glm::vec2 topCenter() const;
    glm::vec2 topRight() const;
    glm::vec2 rightCenter() const;
    glm::vec2 bottomRight() const;
    glm::vec2 bottomCenter() const;
    glm::vec2 bottomLeft() const;

    Rect top(float percent) const;
    Rect left(float percent) const;
    Rect bottom(float percent) const;
    Rect right(float percent) const;

    bool isValid() const;
    bool isHorizontal() const;
    bool isVertical() const;
    bool isEmpty() const;

    float aspectRatio() const;
    float area() const;

    bool contains(const Rect & other) const;

    float longerSide() const;
    float shorterSide() const;

    Rect relativeRect(const glm::vec2 & position, const glm::vec2 & extent) const;
    Rect relativeRect(float offset, float length, bool horizontal) const;

    Rect relativePadded(float padding) const;
    Rect padded(float padding) const;
    Rect padded(float top, float right, float bottom, float left) const;
    Rect paddedWithMinArea(float padding, float minArea) const;

    void comprise(const Rect & other);

    float equalizedMargin(float absoluteMargin, float minRelativeArea, float relativeMargin) const;
    float equalizedRelativeMargin(float relativeMargin) const;

    Rect appliedMargin(float absoluteMargin) const;

    Rect map(const Rect & sourceReference, const Rect & targetReference) const;

    Rect merged(const Rect & rect) const;

    bool intersects(const Rect & rect) const;

    bool operator==(const Rect & other) const;

    operator glm::vec4() const;

    // assuming no previous rotation, rotate some more
    void rotate(Rotation r);
    Rotation rotation() const;
    CurveOrientation curveDirection() const;
    void setCurveDirection(CurveOrientation dir);
    std::pair<Rect, Rect> horizontalSplit(float xCut) const;
    std::pair<Rect, Rect> verticalSplit(float yCut) const;
    Rect intersection(const Rect & other) const;

    glm::vec2 nearestPointTo(const glm::vec2 & referencePoint, const Rect & referenceRect) const;

public:
    Point pos;
    Point extent;


protected:
    Rotation m_rotation = R000;
    CurveOrientation m_curve = CW;


protected:
    std::pair<Rect, Rect> horizontalSplitImpl(float xCut) const;
    std::pair<Rect, Rect> verticalSplitImpl(float yCut) const;
};
