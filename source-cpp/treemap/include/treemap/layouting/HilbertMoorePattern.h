
#pragma once

#include <vector>
#include <tuple>

#include <treemap/layout/Rect.h>

#include <treemap/treemap_api.h>


// ############################ 1 elem  ########################

struct TREEMAP_API DoNothing
{
    static const int PARTS = 1;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        return {rect};
    }
};

// ############################ 2 elems ########################

struct TREEMAP_API SnakeM2
{
    static const int PARTS = 2;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        auto [left, right] = rect.horizontalSplit(weights[0] / (weights[0]+weights[1]));

        left.rotate(Rect::R090);
        // left.setCurveDirection(Rect::CW);
        right.rotate(Rect::R270);
        // right.setCurveDirection(Rect::CW);

        return {left, right};
    }
};

struct TREEMAP_API SnakeH2
{
    static const int PARTS = 2;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        auto [left, right] = rect.horizontalSplit(weights[0] / (weights[0]+weights[1]));

        // left.rotate(Rect::R000);
        // left.setCurveDirection(Rect::CW);
        // right.rotate(Rect::R000);
        // right.setCurveDirection(Rect::CW);

        return {left, right};
    }
};

// ############################ 3 elems ########################

// hilbert CD, CD, CD
// no moore
struct TREEMAP_API Snake3
{
    static const int PARTS = 3;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        auto [left, temp] = rect.horizontalSplit(weights[0] / (weights[0] + weights[1] + weights[2]));
        auto [mid, right] = temp.horizontalSplit(weights[1] / (weights[1] + weights[2]));

        // left.rotate(Rect::R000);
        // left.setCurveDirection(Rect::CW);
        // mid.rotate(Rect::R000);
        // mid.setCurveDirection(Rect::CW);
        // right.rotate(Rect::R000);
        // right.setCurveDirection(Rect::CW);

        return {left, mid, right};
    }
};

// no hilbert
// moore DB, AC, AC
struct TREEMAP_API Most13
{
    static const int PARTS = 3;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        auto [left, temp] = rect.horizontalSplit(weights[0] / (weights[0] + weights[1] + weights[2]));
        auto [bottomRight, topRight] = temp.verticalSplit(weights[2] / (weights[1] + weights[2]));

        left.rotate(Rect::R090);
        // left.setCurveDirection(Rect::CW);
        topRight.rotate(Rect::R270);
        // topRight.setCurveDirection(Rect::CW);
        bottomRight.rotate(Rect::R270);
        // bottomRight.setCurveDirection(Rect::CW);

        return {left, topRight, bottomRight};
    }
};

// hilbert CA, CD, BD
// no moore
struct TREEMAP_API Most23
{
    static const int PARTS = 3;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        auto [temp, top] = rect.verticalSplit((weights[0] + weights[2]) / (weights[0] + weights[1] + weights[2]));
        auto [bottomLeft, bottomRight] = temp.horizontalSplit(weights[0] / (weights[0] + weights[2]));

        bottomLeft.rotate(Rect::R270);
        bottomLeft.setCurveDirection(Rect::CCW);
        // top.rotate(Rect::R000);
        // top.setCurveDirection(Rect::CW);
        bottomRight.rotate(Rect::R090);
        bottomRight.setCurveDirection(Rect::CCW);

        return {bottomLeft, top, bottomRight};
    }
};

// no hilbert
// moore DB, DB, AC
struct TREEMAP_API Most33
{
    static const int PARTS = 3;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);
        auto [temp, right] = rect.horizontalSplit((weights[0] + weights[1]) / (weights[0] + weights[1] + weights[2]));
        auto [bottomLeft, topLeft] = temp.verticalSplit(weights[0] / (weights[0] + weights[1]));

        bottomLeft.rotate(Rect::R090);
        // bottomLeft.setCurveDirection(Rect::CW);
        topLeft.rotate(Rect::R090);
        // topLeft.setCurveDirection(Rect::CW);
        right.rotate(Rect::R270);
        // right.setCurveDirection(Rect::CW);

        return {bottomLeft, topLeft, right};
    }
};

// ############################ 4 elems ########################

// hilbert CD, CD, CD, CD
// moore ?
struct TREEMAP_API Snake4
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        const auto leftSum = weights[0] + weights[1];
        const auto rightSum = weights[2] + weights[3];
        auto [tempLeft, tempRight] = rect.horizontalSplit(leftSum / (leftSum + rightSum));

        std::vector<Rect> result(4);
        std::tie(result[0], result[1]) = tempLeft.horizontalSplit(weights[0] / leftSum);
        std::tie(result[2], result[3]) = tempRight.horizontalSplit(weights[2] / rightSum);

        // result[0].rotate(Rect::R000);
        // result[0].setCurveDirection(Rect::CW);
        // result[1].rotate(Rect::R000);
        // result[1].setCurveDirection(Rect::CW);
        // result[2].rotate(Rect::R000);
        // result[2].setCurveDirection(Rect::CW);
        // result[3].rotate(Rect::R000);
        // result[3].setCurveDirection(Rect::CW);

        return result;
    }
};

// hilbert ?
// moore DB, AC, AC, AC
struct TREEMAP_API Most14
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        std::vector<Rect> result(4);
        std::array<Rect, 2> temps;

        const auto leftSum = weights[0] + weights[1];
        const auto rightSum = weights[2] + weights[3];

        std::tie(result[0], temps[0]) = rect.horizontalSplit(weights[0] / (leftSum + rightSum));
        std::tie(temps[1], result[1]) = temps[0].verticalSplit((weights[2] + weights[3]) / (weights[1] + weights[2] + weights[3]));
        std::tie(result[3], result[2]) = temps[1].verticalSplit(weights[3] / ( weights[2] + weights[3]));

        result[0].rotate(Rect::R090);
        // result[0].setCurveDirection(Rect::CW);
        result[1].rotate(Rect::R270);
        // result[1].setCurveDirection(Rect::CW);
        result[2].rotate(Rect::R270);
        // result[2].setCurveDirection(Rect::CW);
        result[3].rotate(Rect::R270);
        // result[3].setCurveDirection(Rect::CW);

        return result;
    }
};


// hilbert CA, CD, BD, BD
// moore ?
struct TREEMAP_API Most24
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        std::vector<Rect> result(4);
        std::array<Rect, 2> temps;

        const auto leftSum = weights[0] + weights[1];
        const auto rightSum = weights[2] + weights[3];

        std::tie(temps[0], result[1]) = rect.verticalSplit((weights[0] + weights[2] + weights[3]) / (leftSum + rightSum));
        std::tie(result[0], temps[1]) = temps[0].horizontalSplit(weights[0] / (weights[0] + weights[2] + weights[3]));
        std::tie(result[3], result[2]) = temps[1].verticalSplit(weights[3] / ( weights[2] + weights[3]));

        result[0].rotate(Rect::R270);
        result[0].setCurveDirection(Rect::CCW);
        // result[1].rotate(Rect::R000);
        // result[1].setCurveDirection(Rect::CW);
        result[2].rotate(Rect::R090);
        result[2].setCurveDirection(Rect::CCW);
        result[3].rotate(Rect::R090);
        result[3].setCurveDirection(Rect::CCW);

        return result;
    }
};

// hilbert CA, CA, CD, BD
// moore ?
struct TREEMAP_API Most34
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        std::vector<Rect> result(4);
        std::array<Rect, 2> temps;

        const auto leftSum = weights[0] + weights[1];
        const auto rightSum = weights[2] + weights[3];

        std::tie(temps[0], result[2]) = rect.verticalSplit((weights[0] + weights[1] + weights[3]) / (leftSum + rightSum));
        std::tie(temps[1], result[3]) = temps[0].horizontalSplit((weights[0] + weights[1]) / (weights[0] + weights[1] + weights[3]));
        std::tie(result[0], result[1]) = temps[1].verticalSplit(weights[0] / ( weights[0] + weights[1]));

        result[0].rotate(Rect::R270);
        result[0].setCurveDirection(Rect::CCW);
        result[1].rotate(Rect::R270);
        result[1].setCurveDirection(Rect::CCW);
        // result[2].rotate(Rect::R000);
        // result[2].setCurveDirection(Rect::CW);
        result[3].rotate(Rect::R090);
        result[3].setCurveDirection(Rect::CCW);

        return result;
    }
};

// hilbert ?
// moore DB, DB, DB, AC
struct TREEMAP_API Most44
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        std::vector<Rect> result(4);
        std::array<Rect, 2> temps;

        const auto leftSum = weights[0] + weights[1];
        const auto rightSum = weights[2] + weights[3];

        std::tie(temps[0], result[3]) = rect.horizontalSplit((weights[0]+weights[1]+weights[2]) / (leftSum + rightSum));
        std::tie(temps[1], result[2]) = temps[0].verticalSplit((weights[0] + weights[1]) / (weights[0] + weights[1] + weights[2]));
        std::tie(result[0], result[1]) = temps[1].verticalSplit(weights[0] / ( weights[0] + weights[1]));

        result[0].rotate(Rect::R090);
        // result[0].setCurveDirection(Rect::CW);
        result[1].rotate(Rect::R090);
        // result[1].setCurveDirection(Rect::CW);
        result[2].rotate(Rect::R090);
        // result[2].setCurveDirection(Rect::CW);
        result[3].rotate(Rect::R270);
        // result[3].setCurveDirection(Rect::CW);

        return result;
    }
};

// hilbert CA, CD, CD, BD
// moore ?
struct TREEMAP_API Horizontal4
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        std::vector<Rect> result(4);
        std::array<Rect, 2> temps;

        const auto topSum = weights[1] + weights[2];
        const auto bottomSum = weights[0] + weights[3];

        std::tie(temps[0], temps[1]) = rect.verticalSplit(bottomSum / (bottomSum + topSum));
        std::tie(result[0], result[3]) = temps[0].horizontalSplit(weights[0] / bottomSum);
        std::tie(result[1], result[2]) = temps[1].horizontalSplit(weights[1] / topSum);

        result[0].rotate(Rect::R270);
        result[0].setCurveDirection(Rect::CCW);
        // result[1].rotate(Rect::R000);
        // result[1].setCurveDirection(Rect::CW);
        // result[2].rotate(Rect::R000);
        // result[2].setCurveDirection(Rect::CW);
        result[3].rotate(Rect::R090);
        result[3].setCurveDirection(Rect::CCW);

        return result;
    }
};

// hilbert ?
// moore DB, AC, DB, AC
struct TREEMAP_API Vertical4
{
    static const int PARTS = 4;

    static std::vector<Rect> dissect(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        std::vector<Rect> result(4);
        std::array<Rect, 2> temps;

        const auto leftSum = weights[0] + weights[1];
        const auto rightSum = weights[2] + weights[3];

        std::tie(temps[0], temps[1]) = rect.horizontalSplit(leftSum / (leftSum + rightSum));
        std::tie(result[0], result[1]) = temps[0].verticalSplit(weights[0] / leftSum);
        std::tie(result[3], result[2]) = temps[1].verticalSplit(weights[3] / rightSum);

        result[0].rotate(Rect::R090);
        // result[0].setCurveDirection(Rect::CW);
        result[1].rotate(Rect::R090);
        // result[1].setCurveDirection(Rect::CW);
        result[2].rotate(Rect::R270);
        // result[2].setCurveDirection(Rect::CW);
        result[3].rotate(Rect::R270);
        // result[3].setCurveDirection(Rect::CW);

        return result;
    }
};

TREEMAP_API double avgAspectRatio(const std::vector<Rect>& rects) {
    assert(!rects.empty());

    double sum = 0.0;
    for(auto& rect : rects)
    {
        assert(rect.aspectRatio()>0.0);
        sum += rect.aspectRatio();
    }

    assert(sum > 0.0f);

    return sum / rects.size();
}

template<unsigned int PARTS, class PATTERN, class... PATTERNS>
class TREEMAP_TEMPLATE_API DissectorImpl
{
public:
    static_assert(PARTS==PATTERN::PARTS);

    static std::vector<Rect> dissectWithBestAR(const std::vector<float> & weights, const Rect & rect)
    {
        assert(weights.size()==PARTS);

        const auto best = DissectorImpl<PARTS,PATTERNS...>::dissectWithBestAR(weights,rect);
        const auto layout = PATTERN::dissect(weights,rect);

        return avgAspectRatio(layout) < avgAspectRatio(best)
            ? layout
            : best;
    }
};

template<unsigned int PARTS, class PATTERN>
class TREEMAP_TEMPLATE_API DissectorImpl<PARTS,PATTERN>
{
public:
    static std::vector<Rect> dissectWithBestAR(const std::vector<float> & weights, const Rect & rect)
    {
        static_assert(PARTS==PATTERN::PARTS);
        return PATTERN::dissect(weights,rect);
    }
};
