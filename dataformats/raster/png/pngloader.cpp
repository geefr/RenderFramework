#include "pngloader.h"

#include <stdexcept>
#include <fstream>

#define PNG_DEBUG 3
#include <png.h>


PNGLoader::PNGLoader(std::string fileName)
    : m_fileName(fileName)
{
    auto file = std::fopen(fileName.c_str(), "rb");
    if( !file ) throw std::runtime_error("Failed to open file: " + fileName);
    uint8_t pngHeader[8];
    if( std::fread(pngHeader, 1, 8, file) != 8) throw std::runtime_error("Failed to read png header");
    if( png_sig_cmp(pngHeader, 0, 8) ) throw std::runtime_error("File is not a PNG: " + fileName);
    auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if( !png_ptr ) throw std::runtime_error("Failed to init png read struct");
    auto info_ptr = png_create_info_struct(png_ptr);
    if( !info_ptr ) throw std::runtime_error("Failed to init png info struct");
    // if libpng aborts it will longjmp back to here, funky eh?
    if( setjmp(png_jmpbuf(png_ptr))) throw std::runtime_error("Error during png_init_io");
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    // Read file metadata
    m_width = png_get_image_width(png_ptr, info_ptr);
    m_height = png_get_image_height(png_ptr, info_ptr);
    m_colorType = png_get_color_type(png_ptr, info_ptr);
    m_depth = png_get_bit_depth(png_ptr, info_ptr) * png_get_channels(png_ptr, info_ptr);

    png_set_interlace_handling(png_ptr); // Return value relevant to writing
    png_read_update_info(png_ptr, info_ptr);

    // Read the image data itself and translate into the float array
    if( setjmp(png_jmpbuf(png_ptr))) throw std::runtime_error("Error during png_read_image");
    // So we need to allocate a buffer, helps if we know how big it is
    auto rowBytes = png_get_rowbytes(png_ptr, info_ptr);
    // Align rows to 4-bytes (For glTexImage2D and general sanity
    rowBytes += 3 - ((rowBytes-1) % 4);

    std::unique_ptr<png_bytep[]>rowPointers(new png_bytep[m_height]);
    m_data.reset( new uint8_t[rowBytes * m_height] );
    for( auto i=0u; i < m_height; ++i ) rowPointers[m_height - 1 - i] = m_data.get() + (i * rowBytes);
    png_read_image(png_ptr, rowPointers.get());

    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    std::fclose(file);
}

PNGLoader::~PNGLoader()
{

}

void PNGLoader::saveToRaw(std::string fileName)
{
    std::ofstream file(fileName, std::ios_base::binary);
    file.write((const char*)m_data.get(), m_width * m_height * m_depth );
}
