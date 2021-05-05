
#include <treemap/layout/Rect.h>

#include <algorithm>

#include <glm/common.hpp>


Rect::Rect()
: pos(0.0f, 0.0f)
, extent(0.0f, 0.0f)
{
}

Rect::Rect(Rotation rotation, CurveOrientation orientation)
: pos(0.0f, 0.0f)
, extent(0.0f, 0.0f)
, m_rotation(rotation)
, m_curve(orientation)
{
}

Rect::Rect(float left, float bottom, float width, float height)
: pos(left, bottom)
, extent(width, height)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(float left, float bottom, float width, float height, Rotation rotation, CurveOrientation orientation)
: pos(left, bottom)
, extent(width, height)
, m_rotation(rotation)
, m_curve(orientation)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(const glm::vec2 & pos, const glm::vec2 & extent)
: pos(pos)
, extent(extent)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(const glm::vec2 & pos, const glm::vec2 & extent, Rotation rotation, CurveOrientation orientation)
: pos(pos)
, extent(extent)
, m_rotation(rotation)
, m_curve(orientation)
{
}

Rect::Rect(glm::vec2 && pos, glm::vec2 && extent)
: pos(std::move(pos))
, extent(std::move(extent))
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(glm::vec2 && pos, glm::vec2 && extent, Rotation rotation, CurveOrientation orientation)
: pos(std::move(pos))
, extent(std::move(extent))
, m_rotation(rotation)
, m_curve(orientation)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(const glm::vec4 & rect)
: pos(rect.x, rect.y)
, extent(rect.z, rect.w)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(const glm::vec4 & rect, Rotation rotation, CurveOrientation orientation)
: pos(rect.x, rect.y)
, extent(rect.z, rect.w)
, m_rotation(rotation)
, m_curve(orientation)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(glm::vec4 && rect)
: pos(std::move(rect.x), std::move(rect.y))
, extent(std::move(rect.z), std::move(rect.w))
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(glm::vec4 && rect, Rotation rotation, CurveOrientation orientation)
: pos(std::move(rect.x), std::move(rect.y))
, extent(std::move(rect.z), std::move(rect.w))
, m_rotation(rotation)
, m_curve(orientation)
{
}

Rect::Rect(const Rect & other, Rotation rotation, CurveOrientation orientation)
: pos(other.pos)
, extent(other.extent)
, m_rotation(rotation)
, m_curve(orientation)
{
}

Rect::Rect(const Rect & other)
: pos(other.pos)
, extent(other.extent)
, m_rotation(other.m_rotation)
, m_curve(other.m_curve)
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect::Rect(Rect && other)
: pos(std::move(other.pos))
, extent(std::move(other.extent))
, m_rotation(std::move(other.m_rotation))
, m_curve(std::move(other.m_curve))
{
    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));
}

Rect & Rect::operator=(const Rect & other)
{
    pos = other.pos;
    extent = other.extent;
    m_rotation = other.m_rotation;
    m_curve = other.m_curve;

    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));

    return *this;
}

Rect & Rect::operator=(Rect && other)
{
    pos = std::move(other.pos);
    extent = std::move(other.extent);
    m_rotation = std::move(other.m_rotation);
    m_curve = std::move(other.m_curve);

    assert(!glm::isnan(pos.x));
    assert(!glm::isnan(pos.y));
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));

    return *this;
}

float Rect::left() const
{
    return pos.x;
}

float Rect::top() const
{
    return pos.y + extent.y;
}

float Rect::right() const
{
    return pos.x + extent.x;
}

float Rect::bottom() const
{
    return pos.y;
}

float Rect::width() const
{
    return extent.x;
}

float Rect::height() const
{
    return extent.y;
}

glm::vec2 Rect::center() const
{
    return glm::vec2(pos) + glm::vec2(extent) / glm::vec2(2.0f);
}

glm::vec2 Rect::leftCenter() const
{
    return glm::vec2(pos.x, pos.y + extent.y / 2.0f);
}

glm::vec2 Rect::topLeft() const
{
    return glm::vec2(pos.x, pos.y + extent.y);
}

glm::vec2 Rect::topCenter() const
{
    return glm::vec2(pos.x + extent.x / 2.0f, pos.y + extent.y);
}

glm::vec2 Rect::topRight() const
{
    return glm::vec2(pos) + glm::vec2(extent);
}

glm::vec2 Rect::rightCenter() const
{
    return glm::vec2(pos.x + extent.x, pos.y + extent.y / 2.0f);
}

glm::vec2 Rect::bottomRight() const
{
    return glm::vec2(pos.x + extent.x, pos.y);
}

glm::vec2 Rect::bottomCenter() const
{
    return glm::vec2(pos.x + extent.x / 2.0f, pos.y);
}

glm::vec2 Rect::bottomLeft() const
{
    return pos;
}

bool Rect::isValid() const
{
    return !glm::isnan(pos.x) && !glm::isnan(pos.y) && !glm::isnan(extent.x) && !glm::isnan(extent.y);
}

bool Rect::isEmpty() const
{
    return extent.x == 0.0f || extent.y == 0.0f;
}

Rect Rect::top(float percent) const
{
    return Rect(
        pos,
        glm::vec2(extent.x, extent.y * percent),
        m_rotation,
        m_curve
    );
}

Rect Rect::left(float percent) const
{
    return Rect(
        pos,
        glm::vec2(extent.x * percent, extent.y),
        m_rotation,
        m_curve
    );
}

Rect Rect::bottom(float percent) const
{
    return Rect(
        glm::vec2(pos.x, pos.y + extent.y - extent.y * percent),
        glm::vec2(extent.x, extent.y * percent),
        m_rotation,
        m_curve
    );
}

Rect Rect::right(float percent) const
{
    return Rect(
        glm::vec2(pos.x + extent.x - extent.x * percent, pos.y),
        glm::vec2(extent.x * percent, extent.y),
        m_rotation,
        m_curve
    );
}

bool Rect::isHorizontal() const
{
    return extent.x > extent.y;
}

bool Rect::isVertical() const
{
    return !isHorizontal();
}

float Rect::longerSide() const
{
    return std::max(extent.x, extent.y);
}

float Rect::shorterSide() const
{
    return std::min(extent.x, extent.y);
}

float Rect::aspectRatio() const
{
    return longerSide() / shorterSide();
}

float Rect::area() const
{
    assert(!glm::isnan(extent.x));
    assert(!glm::isnan(extent.y));

    return extent.x * extent.y;
}

Rect Rect::relativeRect(const glm::vec2 & position, const glm::vec2 & extent) const
{
    const auto p = glm::vec2(std::min(position.x, 1.0f), std::min(position.y, 1.0f));
    const auto e = glm::vec2(std::min(extent.x, std::max(1.0f - p.x, 0.0f)), std::min(extent.y, std::max(1.0f - p.y, 0.0f)));

    return Rect(
        glm::vec2(this->pos) + glm::vec2(this->extent) * std::move(p),
        glm::vec2(this->extent) * std::move(e),
        m_rotation,
        m_curve
    );
}

Rect Rect::relativeRect(float offset, float length, bool horizontal) const
{
    return Rect(
        horizontal ? pos.x + offset * extent.x : pos.x,
        horizontal ? pos.y : pos.y + offset * extent.y,
        horizontal ? extent.x * length : extent.x,
        horizontal ? extent.y : extent.y * length,
        m_rotation,
        m_curve
    );
}

Rect Rect::padded(float padding) const
{
    return Rect(
        pos.x+padding,
        pos.y+padding,
        std::max(extent.x-2*padding, 0.0f),
        std::max(extent.y-2*padding, 0.0f),
        m_rotation,
        m_curve
    );
}

Rect Rect::padded(float top, float right, float bottom, float left) const
{
    return Rect(
        pos.x+left,
        pos.y+bottom,
        std::max(extent.x - left - right, 0.0f),
        std::max(extent.y - top - bottom, 0.0f),
        m_rotation,
        m_curve
    );
}

Rect Rect::paddedWithMinArea(float padding, float minArea) const
{
    if (padding <= 0.0)
    {
        return padded(padding);
    }

    const auto marginSpace = 2.0f * width() * padding
                + 2.0f * height() * padding
                - 4.0f * padding * padding;

    const auto targetPercentage = 1.0f - marginSpace / area();

    if (targetPercentage >= minArea && shorterSide() > 2.0f * padding)
    {
        return padded(padding);
    }

    return padded(shorterSide() / 2.0f)
        .relativePadded(glm::max(0.0f, 1.0f - padding / (2.0f * shorterSide())));
}

Rect Rect::relativePadded(float padding) const
{
    const auto halfPadding = padding / 2.0f;
    return relativeRect({halfPadding, halfPadding}, {1.0f - padding, 1.0f - padding});
}

Rect Rect::merged(const Rect & rect) const
{
    const auto left = std::min(pos.x, rect.pos.x);
    const auto bottom = std::min(pos.y, rect.pos.y);
    const auto right = std::max(pos.x+extent.x, rect.pos.x+rect.extent.x);
    const auto top = std::max(pos.y+extent.y, rect.pos.y+rect.extent.y);

    return Rect(
        left,
        bottom,
        right-left,
        top-bottom,
        m_rotation,
        m_curve
    );
}

bool Rect::intersects(const Rect & rect) const
{
    if (rect.left() > right()) return false;
    if (rect.right() < left()) return false;
    if (rect.top() < bottom()) return false;
    if (rect.bottom() > top()) return false;

    return true;
}

void Rect::comprise(const Rect & other)
{
    const auto l = std::min(left(), other.left());
    const auto t = std::max(top(), other.top());
    const auto r = std::max(right(), other.right());
    const auto b = std::min(bottom(), other.bottom());

    pos = glm::vec2(l, b);
    extent = glm::vec2(r, t) - glm::vec2(pos);
}

bool Rect::contains(const Rect & other) const
{
    return left() - std::numeric_limits<float>::epsilon() <= other.left()
            && right() + std::numeric_limits<float>::epsilon() >= other.right()
            && top() + std::numeric_limits<float>::epsilon() >= other.top()
            && bottom() - std::numeric_limits<float>::epsilon() <= other.bottom();
}

float Rect::equalizedMargin(float absoluteMargin, float minRelativeArea, float relativeMargin) const
{
    if (area() <= 0.0f)
    {
        return 0.0f;
    }

    const auto marginArea = 2.0f * extent.x * absoluteMargin
                     + 2.0f * extent.y * absoluteMargin
                     - 4.0f * absoluteMargin * absoluteMargin;

    if (marginArea <= 0.0f
        || marginArea / area() >= minRelativeArea
        || shorterSide() < 2.0f * absoluteMargin)
    {
        return equalizedRelativeMargin(relativeMargin);
    }

    return absoluteMargin;
}

float Rect::equalizedRelativeMargin(float relativeMargin) const
{
    if (area() <= 0.0f)
    {
        return 0.0f;
    }

    const auto targetArea = area() * (1.0f - relativeMargin);
    const auto ratio = extent.x / extent.y;
    const auto a = sqrt(targetArea*ratio);
    const auto b = sqrt(targetArea/ratio);

    const auto dArea = area() - targetArea;

    const auto p = -(a+b) * 0.5f;
    const auto q = dArea * 0.25f;
    const auto D = p * p * 0.25f - q;
    const auto sqrtD = sqrt(D);

    const auto d1 = -p * 0.5f + sqrtD;
    const auto d2 = -p * 0.5f - sqrtD;

    return d1 < 0 || d2 < 0 ? std::max(d1, d2) : std::min(d1, d2);
}

Rect Rect::appliedMargin(float absoluteMargin) const
{
    const auto halfMargin = absoluteMargin * 0.5f;
    const auto marginSpace = 2.0f * extent.x * halfMargin
                      + 2.0f * extent.y * halfMargin
                      - 4.0f * halfMargin * halfMargin;

    if (absoluteMargin <= 0.0f)
    {
        return *this;
    }
    else if (0.3f * area() < marginSpace && 0.3f * shorterSide() > absoluteMargin)
    {
        return padded(glm::mix(0.0f, halfMargin, 0.3f * area() / marginSpace));
    }
    else if (0.3f * shorterSide() <= absoluteMargin)
    {
        return padded(glm::mix(0.0f, halfMargin, 0.3f * shorterSide() / absoluteMargin));
    }

    return padded(halfMargin);
}

Rect Rect::map(const Rect & sourceReference, const Rect & targetReference) const
{
    if (sourceReference.area() <= 0.0f)
    {
        return Rect(targetReference.pos, glm::vec2(0.0f, 0.0f), m_rotation, m_curve);
    }

    return Rect(
        ((pos.x - sourceReference.pos.x) / sourceReference.extent.x) * targetReference.extent.x + targetReference.pos.x,
        ((pos.y - sourceReference.pos.y) / sourceReference.extent.y) * targetReference.extent.y + targetReference.pos.y,
        extent.x / sourceReference.extent.x * targetReference.extent.x,
        extent.y / sourceReference.extent.y * targetReference.extent.y,
        m_rotation,
        m_curve
    );
}

bool Rect::operator==(const Rect & other) const
{
    return pos == other.pos && extent == other.extent;
}

Rect::operator glm::vec4() const
{
    return glm::vec4(pos.x, pos.y, extent.x, extent.y);
}

void Rect::multiply(float factor)
{
    pos *= factor;
    extent *= factor;
}

void Rect::divide(float factor)
{
    pos /= factor;
    extent /= factor;
}

void Rect::scale(float x, float y)
{
    pos *= glm::vec2(x, y);
    extent *= glm::vec2(x, y);
}

std::pair<Rect, Rect> Rect::horizontalSplitImpl(float xCut) const
{
    return {
        Rect(pos.x, pos.y, extent.x * xCut, extent.y, m_rotation, m_curve),
        Rect(pos.x + extent.x * xCut, pos.y, extent.x * (1.0f-xCut), extent.y, m_rotation, m_curve)
    };
}

std::pair<Rect, Rect> Rect::verticalSplitImpl(float yCut) const
{
    return {
        Rect(pos.x, pos.y, extent.x, extent.y * yCut, m_rotation, m_curve),
        Rect(pos.x, pos.y + extent.y * yCut, extent.x, extent.y * (1.0f-yCut), m_rotation, m_curve)
    };
}

glm::vec2 Rect::nearestPointTo(const glm::vec2 & referencePoint, const Rect & referenceRect) const
{
    const auto pointInReferenceRect = glm::vec2(
        std::min(std::max(referencePoint.x, referenceRect.left()), referenceRect.right()),
        std::min(std::max(referencePoint.y, referenceRect.bottom()), referenceRect.top())
    );

    return glm::vec2(
        std::min(std::max(pointInReferenceRect.x, left()), right()),
        std::min(std::max(pointInReferenceRect.y, bottom()), top())
    );
}

void Rect::rotate(Rotation r)
{
    m_rotation = static_cast<Rotation>((m_rotation + r) % 4);
}

Rect::Rotation Rect::rotation() const
{
    return m_rotation;
}

Rect::CurveOrientation Rect::curveDirection() const
{
    return m_curve;
}

void Rect::setCurveDirection(CurveOrientation dir)
{
    m_curve = static_cast<CurveOrientation>((m_curve + dir) % 2);
}

std::pair<Rect, Rect> Rect::horizontalSplit(float xCut) const
{
    if (m_rotation==R000)
    {
        return horizontalSplitImpl(xCut);
    }
    else if (m_rotation==R090)
    {
        return verticalSplitImpl(xCut);
    }
    else if (m_rotation==R180)
    {
        auto [l,r] = horizontalSplitImpl(1.0f-xCut);
        return {r,l};
    }
    assert(m_rotation==R270);
    auto [b,t] = verticalSplitImpl(1.0f-xCut);
    return {t,b};
}

std::pair<Rect, Rect> Rect::verticalSplit(float yCut) const
{
    if (m_rotation==R000)
    {
        return verticalSplitImpl(yCut);
    }
    else if(m_rotation==R090)
    {
        auto [l,r] = horizontalSplitImpl(1.0f-yCut);
        return {r,l};
    }
    else if (m_rotation==R180)
    {
        auto [b,t] = verticalSplitImpl(1.0f-yCut);
        return {t,b};
    }
    assert(m_rotation==R270);
    return horizontalSplitImpl(yCut);
}

Rect Rect::intersection(const Rect & other) const
{
    const auto iLeft = std::max(left(), other.left());
    const auto iBottom = std::max(bottom(), other.bottom());
    const auto iTop = std::min(top(), other.top());
    const auto iRight = std::min(right(), other.right());
    return Rect(iLeft, iBottom, std::max(iRight - iLeft, 0.f), std::max(iTop - iBottom, 0.f));
}
