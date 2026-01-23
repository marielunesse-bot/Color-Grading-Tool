#pragma once
#include <string>
#include <GL/gl.h>

class Shader
{
public:
    Shader();
    ~Shader();

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);
    void Use() const;

    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;

private:
    GLuint mProgramID;

    std::string ReadFile(const std::string& path);
    bool CompileShader(const std::string& source, GLenum type, GLuint& shaderID);
};