#ifndef RASTER_H
#define RASTER_H

#include <string>
#include <memory>

namespace renderframework { namespace raster {
    class Raster
    {
    public:
        virtual ~Raster();

        /// Width in pixels
        uint32_t width() const;

        /// Height in pixels
        uint32_t height() const;

        /// Depth in bits
        uint8_t depth() const;

        /// Size of data blob in bytes
        uint32_t dataSize() const;

        /// Pointer to data blob
        const uint8_t* data() const;

        /// Save the data blob to a file
        bool saveToRaw(std::string fileName);

    protected:
        /**
         * Constructor
         *
         * Child classes must:
         * - Determine the file parameters, and set class members
         * - Allocate the data buffer through allocateData
         * - Populate the data buffer with pixel data
         *
         * Data is assumed to be either 24-bit RGB or 32-bit RGBA
         *
         * @param fileName The raster to load
         */
        Raster( std::string fileName );

        void allocateData(uint32_t size);

        std::string mFileName;
        uint32_t mWidth = 0;
        uint32_t mHeight = 0;
        uint8_t mDepth = 0;
        uint32_t mDataSize = 0;
        std::unique_ptr<uint8_t[]> mData;
    };
} }

#endif // RASTERLOADER_H
