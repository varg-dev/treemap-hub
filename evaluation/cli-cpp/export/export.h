
#pragma once


#include <iosfwd>

#include <treemap/buffers/LinearizedBuffer.h>
#include <treemap/layout/Rect.h>


enum class SVGColorType : unsigned char
{
    ColorScale,
    GrayScaleDiscriminatibility,
    GrayScaleContinuous
};

enum class PathType
{
    None,
    PerDepth,
    LeavesOnly,
    DepthFirst
};

bool writeToFile(const LinearizedBuffer<Rect> & layout, std::ostream & out);

template <PathType pathType = PathType::LeavesOnly>
bool writeToSVG(const LinearizedBuffer<Rect> & layout, const LinearizedTree * tree, float layoutScale, bool outlined, SVGColorType colored, std::ostream & out);


#include "export.inl"
