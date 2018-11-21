#ifndef PNGLOADER_H
#define PNGLOADER_H

#include "dataformats/raster/raster.h"

namespace renderframework { namespace raster {
    class PNG : public Raster
    {
    public:
        PNG(std::string fileName);
        virtual ~PNG();
    };
} }

#endif // PNGLOADER_H
