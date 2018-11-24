#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

namespace renderframework {
    class ShaderProgram;
}

namespace renderframework { namespace materials {
    class Material
    {
    public:
        virtual ~Material();

        virtual void registerUniforms( ShaderProgram& shader ) = 0;
        virtual void setUniforms( ShaderProgram& shader ) = 0;
        virtual void registerUniforms( std::shared_ptr<ShaderProgram> shader ) = 0;
        virtual void setUniforms( std::shared_ptr<ShaderProgram> shader ) = 0;
    protected:
        Material();
    };
} }

#endif // MATERIAL_H
