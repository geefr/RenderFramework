#include "raster.h"

#include <stdexcept>
#include <fstream>

namespace renderframework { namespace raster {

    Raster::Raster( std::string fileName )
        : mFileName(fileName)
    {

    }

    Raster::~Raster()
    {

    }

    uint32_t Raster::width() const { return mWidth; }
    uint32_t Raster::height() const { return mHeight; }
    uint8_t Raster::depth() const { return mDepth; }
    uint32_t Raster::dataSize() const { return mDataSize; }
    const uint8_t* Raster::data() const { return mData.get(); }

    bool Raster::saveToRaw(std::string fileName)
    {
        if( !mData ) return false;
        std::ofstream file(fileName, std::ios_base::binary);
        file.write(reinterpret_cast<std::ofstream::char_type*>(mData.get()), mWidth * mHeight * (mDepth/8) );
        return true;
    }

    void Raster::allocateData(uint32_t size)
    {

    }
} }
