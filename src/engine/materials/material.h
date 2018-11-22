#ifndef MATERIAL_H
#define MATERIAL_H

namespace renderframework {
    class ShaderProgram;
}

namespace renderframework { namespace materials {
    class Material
    {
    public:
        virtual ~Material();

        virtual void setUniforms( ShaderProgram& shader ) = 0;
    protected:
        Material();
    };
} }

#endif // MATERIAL_H
