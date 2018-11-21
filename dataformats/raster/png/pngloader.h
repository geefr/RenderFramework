#ifndef PNGLOADER_H
#define PNGLOADER_H

#include <string>
#include <memory>

class PNGLoader
{
public:
    PNGLoader(std::string fileName);
    ~PNGLoader();

    uint32_t width() const;
    uint32_t height() const;
    uint8_t depth() const;
    uint32_t dataSize() const;
    const uint8_t* data() const;

    void saveToRaw(std::string fileName);

private:
    std::string m_fileName;

    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint8_t m_colorType = 0x00;
    uint8_t m_depth = 0;

    std::unique_ptr<uint8_t[]> m_data;
};

inline uint32_t PNGLoader::width() const { return m_width; }
inline uint32_t PNGLoader::height() const { return m_height; }
inline uint8_t PNGLoader::depth() const { return m_depth; }
inline uint32_t PNGLoader::dataSize() const { return m_width * m_height * m_depth; }
inline const uint8_t* PNGLoader::data() const { return m_data.get(); }

#endif // PNGLOADER_H
