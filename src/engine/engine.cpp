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

    GLuint Engine::createVertexBuffer( const std::vector<VertexDef>& data )
    {
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(VertexDef)), data.data(), GL_STATIC_DRAW);
        return vertexBuffer;
    }

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
        // Generate the vertex data and such
        vector::Cube cube({}, {2.0f, 2.0f, 2.0f});
        for( auto& v: cube.vertices() )
        {
            vertexData.push_back({v, vec2(), vec4(1.f,1.f,1.f,1.f)});
        }
        for( auto i=0u;i < cube.vertexColors().size(); ++i )
        {
            //vertexData[i].color = cube.vertexColors()[i];
            vertexData[i].color = { 0.5f, 0.5, 0.5f, 1.0f };
        }
        for( auto i=0u;i < cube.normals().size(); ++i )
        {
            vertexData[i].normal = cube.normals()[i];
        }

/*
        vertexData.push_back({vec3(-.6f,-.6f,-1.f), vec2(), vec4(1.f,0.f,0.f,1.f)});
        vertexData.push_back({vec3( .6f,-.6f,-1.f), vec2(), vec4(0.f,1.f,0.f,1.f)});
        vertexData.push_back({vec3(0.f,.6f,-1.f), vec2(), vec4(0.f,0.f,1.f,1.f)});
*/


        /*
        std::vector<Loader> loaders = {
            StaticShapeLoader(StaticShapeLoader::Geometry::Cube),
        };

        for( auto& loader : loaders )
        {
            auto vd = loaders.vertices();
            vertexData.insert(std::end(vertexData), std::begin(vd), std::end(vd));
        }*/

        // Do something really naughty here to trick the tesselation
        // As we know we'll be using tesselation patches of size 3 when drawing
        // ensure the vertices are a multiple of 3 so no vertices are lost
        // when the last patch can't be filled
        // This does mean we'll have up to 2 extra vertices on top of each
        // other but should look alright?
        if( vertexData.size() % 3 == 1 )
        {
            vertexData.emplace_back( vertexData[vertexData.size() - 3] );
            vertexData.emplace_back( vertexData[vertexData.size() - 3] );
            std::cerr << "HACK: Had to add 2 extra dummy vertices to end of vertex data in order to satisfy tesselation patch" << std::endl;
        }
        else if( vertexData.size() % 3 == 2)
        {
            vertexData.emplace_back( vertexData[vertexData.size() - 3] );
            std::cerr << "HACK: Had to add 1 extra dummy vertices to end of vertex data in order to satisfy tesselation patch" << std::endl;
        }

        vertexBuffer = createVertexBuffer(vertexData);

        ////////////////////////////////////////////////////////////
        // Let's load some shaders
        mShader.addShader(GL_VERTEX_SHADER, dataDir + "shaders/vertex/default.vert");
        mShader.addShader(GL_FRAGMENT_SHADER, dataDir + "shaders/fragment/specular.frag");
        mShader.addShader(GL_TESS_CONTROL_SHADER, dataDir + "shaders/tesselation/3vertpatch.tesscont");
        mShader.addShader(GL_TESS_EVALUATION_SHADER, dataDir + "shaders/tesselation/3vertpatch.tesseval");
        // Link the shader
        mShader.id();

        mShader.regUniform("modelViewProjectionMatrix");
        mShader.regUniform("modelMatrix");
        mShader.regUniform("viewMatrix");
        mShader.regUniform("projectionMatrix");
        mShader.regUniform("texture0");
        mShader.regUniform("enableTexture0");

        mShader.regAttribute("vertCoord", 3, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,coord)));
        mShader.regAttribute("vertTexCoord", 2, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,texCoord)));
        mShader.regAttribute("vertTexColor", 4, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,color)));
        mShader.regAttribute("vertNormal", 3, GL_FLOAT, GL_FALSE, sizeof(VertexDef), reinterpret_cast<const void*>(offsetof(VertexDef,normal)));

        // Lighting uniforms
        mShader.regUniform("light.ambient");
        mShader.regUniform("light.diffuse");
        mShader.regUniform("light.specular");
        mShader.regUniform("light.position");

        // Eye position for specular calculations
        mShader.regUniform("eyePos");


        ////////////////////////////////////////////////////////////
        // Setup some textures and stuff
        //catTexture = loadTexture(dataDir + "textures/cat.png");
        ////////////////////////////////////////////////////////////
        // Setup the lighting
        light.ambient[0] = 1.0f;
        light.ambient[1] = 1.0f;
        light.ambient[2] = 1.0f;
        light.ambient *= 0.2f;

        light.diffuse[0] = 1.0f;
        light.diffuse[1] = 1.0f;
        light.diffuse[2] = 1.0f;
        light.diffuse *= 0.8f;

        light.specular[0] = 1.0f;
        light.specular[1] = 1.0f;
        light.specular[2] = 1.0f;
        light.specular *= 1.0f;

        light.position = vec3(1.f,0.0f,1.f);

        ////////////////////////////////////////////////////////////
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

        //mat4x4_identity(m);

        m = translate(m, vec3(0.f,0.f,-0.5f));

        m = rotate(m, viewRot[0], vec3(1.f,0.f,0.f));
        m = rotate(m, viewRot[1], vec3(0.f,1.f,0.f));
        m = rotate(m, viewRot[2], vec3(0.f,0.f,1.f));

        // eye, center, up
        vec3 eyePos(0.f,0.f,1.0f);
        v = lookAt(eyePos,vec3(0.f,0.f,0.f),vec3(0.f,1.f,0.f));

        // fov, aspect, near plane distance, far plane distance
        p = perspective(90.f, width / height, 0.1f, 10.0f );

        mvp = p * v * m;

        glUseProgram(mShader.id());

        glUniformMatrix4fv(mShader.uniform("modelViewProjectionMatrix"), 1, GL_FALSE, value_ptr(mvp));
        glUniformMatrix4fv(mShader.uniform("modelMatrix"), 1, GL_FALSE, value_ptr(m));
        glUniformMatrix4fv(mShader.uniform("viewMatrix"), 1, GL_FALSE, value_ptr(v));
        glUniformMatrix4fv(mShader.uniform("projectionMatrix"), 1, GL_FALSE, value_ptr(p));

        /*
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, catTexture);
        glUniform1i(mShader.uniform("texture0"), 0);
        glUniform1i(mShader.uniform("enableTexture0"), false);
        */

        glUniform3fv(mShader.uniform("light.ambient"), 1, value_ptr(light.ambient));
        glUniform3fv(mShader.uniform("light.diffuse"), 1, value_ptr(light.diffuse));
        glUniform3fv(mShader.uniform("light.specular"), 1, value_ptr(light.specular));
        glUniform3fv(mShader.uniform("light.position"), 1, value_ptr(light.position));

        glUniform3fv(mShader.uniform("eyePos"), 1, value_ptr(eyePos));

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        // TODO: losing precision -> GLsizei, so may need multiple draws here to manage it
        glDrawArrays(GL_PATCHES, 0, vertexData.size());
    }
}
