#include "export/export.h"


#include <iostream>
#include <sstream>


bool writeToFile(const LinearizedBuffer<Rect> & layout, std::ostream & out)
{
    for (size_t i = 0; i < layout.size(); ++i)
    {
        const auto & rect = layout[i];

        out << rect.left() << " " << rect.bottom() << " " << rect.width() << " " << rect.height() << '\n';
    }
    
    out.flush();

    return true;
}
