#include "shaderprogram.h"

#include <fstream>
#include <string>
#include <stdexcept>

namespace renderframework
    {
    ShaderProgram::ShaderProgram()
    {

    }

    ShaderProgram::~ShaderProgram()
    {
        if( m_program )
        {
            for( auto& s : m_shaders )
            {
                glDeleteShader(s.shader);
            }
            glDeleteProgram(m_program);
        }
    }

    void ShaderProgram::addShader( GLenum shaderType, std::string shaderFileName )
    {
        std::ifstream file(shaderFileName);
        if(!file.is_open()) throw std::runtime_error("Failed to open shader: " + shaderFileName);
        std::string shaderSource( (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        //std::cerr << "Loaded shader source: " << path << "\n" << shaderSource << std::endl;

        auto shader = glCreateShader(shaderType);
        auto* srcPtr = shaderSource.c_str();
        glShaderSource(shader, 1, &srcPtr, nullptr);
        glCompileShader(shader);

        m_shaders.push_back( {shaderType, shaderSource, shader} );
    }

    void ShaderProgram::init() const { id(); }

    GLuint ShaderProgram::id() const
    {
      if( m_program ) return m_program;

      m_program = glCreateProgram();
      for( auto& shad : m_shaders ) glAttachShader(m_program, shad.shader);
      glLinkProgram(m_program);

      GLint linkSuccess = 0;
      glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess);
      if( linkSuccess == GL_FALSE )
      {
          GLint logLength = 0;
          glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
          std::vector<GLchar> log(static_cast<std::vector<GLchar>::size_type>(logLength));
          glGetProgramInfoLog(m_program, logLength, &logLength, &log[0]);
          glDeleteProgram(m_program);
          if( !log.empty() )
          {
            throw std::runtime_error("Failed to link shader: " + std::string(reinterpret_cast<const char*>(&log[0])));
          }
          else
          {
              throw std::runtime_error("Failed to link shader, unknown reason");
          }
      }

      return m_program;
    }

    void ShaderProgram::regAttribute(std::string attributeName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
    {
        if( !m_program ) throw std::runtime_error("ShaderProgram::regAttribute: Program not initialised, call id first");
        auto id = glGetAttribLocation(m_program, attributeName.c_str());
        m_attributes[attributeName] = {id, attributeName};

        glEnableVertexAttribArray(static_cast<GLuint>(id));
        glVertexAttribPointer(static_cast<GLuint>(id), size, type, normalized, stride, ptr);
    }

    void ShaderProgram::regUniform(std::string uniformName)
    {
        if( !m_program ) throw std::runtime_error("ShaderProgram::regUniform: Program not initialised, call id first");
        m_uniforms[uniformName] = {glGetUniformLocation(m_program, uniformName.c_str()), uniformName};
    }

    GLint ShaderProgram::attribute(std::string attributeName)
    {
        return m_attributes[attributeName].index;
    }

    GLint ShaderProgram::uniform(std::string uniformName)
    {
        return m_uniforms[uniformName].index;
    }
}
