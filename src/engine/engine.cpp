#include "engine.h"

using namespace std;

#include "dataformats/vector/primitives/cube.h"

namespace renderframework
{
    void GLAPIENTRY
    GLErrorCallback( [[maybe_unused]] GLenum source,
                     GLenum type,
                     [[maybe_unused]] GLuint id,
                     GLenum severity,
                     [[maybe_unused]] GLsizei length,
                     const GLchar* message,
                     [[maybe_unused]] const void* userParam )
    {
      fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                type, severity, message );
      if( severity == GL_DEBUG_SEVERITY_HIGH )
      {
          Engine::quit("High severity error raised, terminating");
      }
    }

    Engine::Engine()
    {

        auto addMat = [&](std::string name, vec3 a, vec3 b, vec3 c, float d) {
          mMaterials[name].reset( new materials::PhongMaterialBare(a,b,c,d) );
        };

        addMat("emerald",{	0.0215,	0.1745	,0.0215}, {	0.07568,	0.61424,	0.07568}, {	0.633,	0.727811,	0.633}, 	0.6 * 256.0);
        addMat("jade",{	0.135,	0.2225,	0.1575}, {	0.54,	0.89,	0.63}, {	0.316228,	0.316228,	0.316228}, 	0.1);
        addMat("obsidian",{	0.05375,	0.05	,0.06625}, {	0.18275	,0.17	,0.22525}, {	0.332741,	0.328634,	0.346435}, 	0.3 * 256.0);
        addMat("pearl",{	0.25	,0.20725,	0.20725}, {	1	,0.829,	0.829}, {	0.296648,	0.296648	,0.296648}, 	0.088 * 256.0);
        addMat("ruby",{	0.1745,	0.01175,	0.01175}, {	0.61424,	0.04136,	0.04136}, {	0.727811	,0.626959	,0.626959}, 	0.6 * 256.0);
        addMat("turquoise",{	0.1	,0.18725,	0.1745}, {	0.396	,0.74151	,0.69102}, {	0.297254,	0.30829,	0.306678}, 	0.1 * 256.0);
        addMat("brass",{	0.329412	,0.223529	,0.027451}, {	0.780392	,0.568627,	0.113725}, {	0.992157	,0.941176	,0.807843}, 	0.21794872 * 256.0);
        addMat("bronze",{	0.2125,	0.1275,	0.054}, {	0.714,	0.4284,	0.18144}, {	0.393548,	0.271906	,0.166721}, 	0.2 * 256.0);
        addMat("chrome",{	0.25,	0.25	,0.25}, {	0.4,	0.4,	0.4},{	0.774597, 	0.774597,	0.774597}, 	0.6 * 256.0);
        addMat("copper",{	0.19125,	0.0735,	0.0225}, {	0.7038,	0.27048,	0.0828}, {	0.256777	,0.137622,0.086014}, 	0.1 * 256.0);
        addMat("gold",{	0.24725,	0.1995	,0.0745}, {	0.75164	,0.60648	,0.22648}, {	0.628281,	0.555802	,0.366065}, 	0.4 * 256.0);
        addMat("silver",{	0.19225,	0.19225	,0.19225}, {	0.50754	,0.50754	,0.50754}, {	0.508273,	0.508273	,0.508273}, 	0.4 * 256.0);
        addMat("black plastic",{	0.0,	0.0	,0.0}, {	0.01	,0.01	,0.01}, {	0.50	,0.50	,0.50}, 	.25 * 256.0);
        addMat("cyan plastic",{	0.0	,0.1	,0.06}, {	0.0	,0.50980392	,0.50980392}, {	0.50196078	,0.50196078	,0.50196078}, 	.25 * 256.0);
        addMat("green plastic",{	0.0	,0.0	,0.0}, {	0.1	,0.35,	0.1}, {	0.45	,0.55,	0.45}, 	.25 * 256.0);
        addMat("red plastic",{	0.0	,0.0,	0.0}, {	0.5,	0.0	,0.0}, {	0.7,	0.6	,0.6}, .25 * 256.0);
        addMat("white plastic",{	0.0,	0.0,	0.0}, {	0.55,	0.55	,0.55}, {	0.70	,0.70	,0.70}, 	.25 * 256.0);
        addMat("yellow plastic",{	0.0,	0.0,	0.0}, {	0.5,	0.5	,0.0}, {	0.60,	0.60,	0.50}, .25 * 256.0);
        addMat("black rubber",{	0.02	,0.02	,0.02}, {	0.01	,0.01,	0.01}, {	0.4,	0.4,	0.4}, 	.078125 * 256.0);
        addMat("cyan rubber",{	0.0	,0.05,	0.05}, {	0.4,	0.5	,0.5}, {	0.04,	0.7	,0.7}, 	.078125 * 256.0);
        addMat("green rubber",{	0.0	,0.05	,0.0}, {	0.4,	0.5	,0.4}, {	0.04,	0.7	,0.04}, .078125 * 256.0);
        addMat("red rubber",{	0.05	,0.0	,0.0}, {	0.5,	0.4,	0.4}, {	0.7	,0.04	,0.04}, .078125 * 256.0);
        addMat("white rubber",{	0.05	,0.05	,0.05}, {	0.5	,0.5	,0.5}, {	0.7,	0.7,	0.7}, .078125 * 256.0);
        addMat("yellow rubber",{	0.05,	0.05	,0.0}, {	0.5	,0.5	,0.4}, {	0.7,	0.7,	0.04}, .078125 * 256.0);

        light.mAmbient = {.2f,.2f,.2f};
        light.mDiffuse = {.6f,.6f,.6f};
        light.mSpecular = {.2f,.2f,.2f};
        light.mPosition = {1.f,0.f,1.f};
    }

    //l/r/b/t
    [[noreturn]] void Engine::quit(std::string msg)
    {
        throw std::runtime_error(msg);
    }

    void Engine::checkGlError()
    {
        auto error = glGetError();
        switch(error)
        {
            case GL_NO_ERROR: return;
            case GL_INVALID_ENUM: quit("GL_INVALID_ENUM");
            case GL_INVALID_VALUE: quit("GL_INVALID_VALUE");
            case GL_INVALID_OPERATION: quit("GL_INVALID_OPERATION");
            case GL_INVALID_FRAMEBUFFER_OPERATION: quit("GL_INVALID_FRAMEBUFFER_OPERATION");
            case GL_OUT_OF_MEMORY: quit("GL_OUT_OF_MEMORY");
            case GL_STACK_UNDERFLOW: quit("GL_STACK_UNDERFLOW");
            case GL_STACK_OVERFLOW: quit("GL_STACK_OVERFLOW");
        }
    }

/*
    GLuint Engine::loadAndCompileShader( GLenum shaderType, std::string path )
    {
        std::ifstream file(path);
        if(!file.is_open()) quit("Failed to open shader: " + path);

        std::string shaderSource( (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        //std::cerr << "Loaded shader source: " << path << "\n" << shaderSource << std::endl;

        auto shader = glCreateShader(shaderType);
        auto* srcPtr = shaderSource.c_str();
        glShaderSource(shader, 1, &srcPtr, nullptr);
        checkGlError();
        glCompileShader(shader);
        checkGlError();

        return shader;
    }

    GLuint Engine::linkShaders( const std::initializer_list<GLuint>& v )
    {
        auto prog = glCreateProgram();
        for( auto& shad : v ) glAttachShader(prog, shad);
        glLinkProgram(prog);
        checkGlError();

        GLint linkSuccess = 0;
        glGetProgramiv(prog, GL_LINK_STATUS, &linkSuccess);
        if( linkSuccess == GL_FALSE )
        {
            GLint logLength = 0;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(static_cast<std::vector<GLchar>::size_type>(logLength));
            glGetProgramInfoLog(prog, logLength, &logLength, &log[0]);
            glDeleteProgram(prog);

            quit("Failed to link shader: " + std::string(reinterpret_cast<const char*>(&log[0])));
        }

        return prog;
    }
*/
    GLuint Engine::loadTexture( std::string fileName )
    {
        GLuint tex;
        glGenTextures(1, &tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the image
        raster::PNG loader(fileName);

        GLenum pixFormat;
        switch( loader.depth() )
        {
            case 24: pixFormat = GL_RGB; break;
            case 32: pixFormat = GL_RGBA; break;
            default: quit("Unsupported image depth: " + std::to_string(loader.depth()));
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(loader.width()), static_cast<GLsizei>(loader.height()), 0, pixFormat, GL_UNSIGNED_BYTE, loader.data() );

        // Generate mipmaps (After texture upload)
        glGenerateMipmap(GL_TEXTURE_2D);
        return tex;
    }
/*
    GLuint Engine::createVertexBuffer( const std::vector<VertexDef>& data )
    {
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(VertexDef)), data.data(), GL_STATIC_DRAW);
        return vertexBuffer;
    }
*/
    void Engine::setVertexAttribPointers(GLint vertCoordAttrib, GLint texCoordAttrib, GLint vertColorAttrib)
    {
        glEnableVertexAttribArray(static_cast<GLuint>(vertCoordAttrib));
        glVertexAttribPointer(static_cast<GLuint>(vertCoordAttrib), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<void*>(offsetof(VertexDef,coord)));
        glEnableVertexAttribArray(static_cast<GLuint>(texCoordAttrib));
        glVertexAttribPointer(static_cast<GLuint>(texCoordAttrib), 2, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<void*>(offsetof(VertexDef,texCoord)));
        glEnableVertexAttribArray(static_cast<GLuint>(vertColorAttrib));
        glVertexAttribPointer(static_cast<GLuint>(vertColorAttrib), 4, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<void*>(offsetof(VertexDef,color)));
    }

    void Engine::printMat4x4(mat4x4 mat, std::string name)
    {
        std::cerr << name << ":\n";
        for(auto col = 0; col < 3; ++col)
        {
            std::cerr << col << ": ";
            for(auto row = 0; row < 3; ++row)
            {
                std::cerr << "[" << mat[col][row] << "]";
            }
            std::cerr << std::endl;
        }
    }

    void Engine::init()
    {
        // TODO: Pretty much a hack right now ;)
        auto dataEnv = std::getenv("RENDERFRAMEWORK_ROOT");
        if( !dataEnv ) quit ("Failed to read root dir, is RENDERFRAMEWORK_ROOT set?");
        std::string dataDir = std::string(dataEnv) + "/";

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLErrorCallback, nullptr);

        glEnable(GL_DEPTH_TEST);

        glClearColor( 0.6f, 0.2f, 0.2f, 1.0f);

        ////////////////////////////////////////////////////////////

        // Create and bind a VAO
        // We need one bound, but otherwise don't use these right now
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        ////////////////////////////////////////////////////////////
        // Let's load some shaders
        std::shared_ptr<ShaderProgram> phong(new ShaderProgram());
        phong->addShader(GL_VERTEX_SHADER, dataDir + "shaders/vertex/default.vert");
        phong->addShader(GL_FRAGMENT_SHADER, dataDir + "shaders/fragment/phong.frag");
        phong->addShader(GL_TESS_CONTROL_SHADER, dataDir + "shaders/tesselation/3vertpatch.tesscont");
        phong->addShader(GL_TESS_EVALUATION_SHADER, dataDir + "shaders/tesselation/3vertpatch.tesseval");
        mShaders["phong"] = phong;

        // Generate the vertex data and such
        mNode.reset(new nodes::MeshNodeDA());
        mNode->meshes().emplace_back(new vector::Cube({.0f,.0f,-5.f}, {6.0f, 4.0f, 1.0f}));
        mNode->shader() = phong;
        mNode->material() = mMaterials["silver"];

        mNode->init();
        mNode->upload();

        ////////////////////////////////////////////////////////////

        // Link the shader
        phong->id();

        phong->regUniform("modelViewProjectionMatrix");
        phong->regUniform("modelMatrix");
        phong->regUniform("viewMatrix");
        phong->regUniform("projectionMatrix");
        phong->regUniform("texture0");
        phong->regUniform("enableTexture0");

        // Lighting uniforms
        Light::registerUniforms(*(phong.get()));

        // Eye position for specular calculations
        phong->regUniform("eyePos");
    }

    void Engine::loop( float width, float height )
    {
        viewRot[0] += viewRotDelta[0];
        viewRot[1] += viewRotDelta[1];
        viewRot[2] += viewRotDelta[2];

        // Hack, should use framebuffersizecallback ;)
        glViewport(0,0,width,height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4 m(1.f);
        mat4x4 v(1.f);
        mat4x4 p(1.f);
        mat4x4 mvp(1.f);

        //m = translate(m, vec3(0.f,0.f,-0.5f));


        // = translate(m, cube.mCenter);
        m = rotate(m, viewRot[0], vec3(1.f,0.f,0.f));
        m = rotate(m, viewRot[1], vec3(0.f,1.f,0.f));
        m = rotate(m, viewRot[2], vec3(0.f,0.f,1.f));
        //m = translate(m, -cube.mCenter);


        // eye, center, up
        vec3 eyePos(0.f,0.f,1.0f);
        v = lookAt(eyePos,vec3(0.f,0.f,0.f),vec3(0.f,1.f,0.f));

        // fov, aspect, near plane distance, far plane distance
        p = perspective(90.f, width / height, 0.1f, 10.0f );

        mvp = p * v * m;

        auto shader = mShaders["phong"];

        glUseProgram(shader->id());

        glUniformMatrix4fv(shader->uniform("modelViewProjectionMatrix"), 1, GL_FALSE, value_ptr(mvp));
        glUniformMatrix4fv(shader->uniform("modelMatrix"), 1, GL_FALSE, value_ptr(m));
        glUniformMatrix4fv(shader->uniform("viewMatrix"), 1, GL_FALSE, value_ptr(v));
        glUniformMatrix4fv(shader->uniform("projectionMatrix"), 1, GL_FALSE, value_ptr(p));
        glUniform3fv(shader->uniform("eyePos"), 1, value_ptr(eyePos));

        /*
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, catTexture);
        glUniform1i(mShader.uniform("texture0"), 0);
        glUniform1i(mShader.uniform("enableTexture0"), false);
        */

        light.setUniforms(*(shader.get()));

        mNode->render();

        //mMaterial.setUniforms(mShader);

        //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        //glPatchParameteri(GL_PATCH_VERTICES, 3);
        // TODO: losing precision -> GLsizei, so may need multiple draws here to manage it
        //glDrawArrays(GL_PATCHES, 0, vertexData.size());
    }
}
