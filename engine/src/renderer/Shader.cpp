#include "renderer/Shader.hpp"

meteor::Shader::Shader(const std::filesystem::path vs_path, const std::filesystem::path fs_path)
{
    std::string vs_src;
    std::string fs_src;
    std::ifstream vs_file;
    std::ifstream fs_file;

    vs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        vs_file.open(vs_path);
        fs_file.open(fs_path);
        std::stringstream vs_stream, fs_stream;

        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();		

        vs_file.close();
        fs_file.close();

        vs_src = vs_stream.str();
        fs_src = fs_stream.str();		
    }
    catch(std::ifstream::failure e)
    {
        METEOR_CORE_ERROR("Failed to parse shader file: {}", e.what());
    }

    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = (const GLchar *)vs_src.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        METEOR_CORE_ASSERT(false, "Vertex shader compilation failed: {}", infoLog.data());
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar *)fs_src.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        METEOR_CORE_ASSERT(false, "Fragment shader compilation failed: {}", infoLog.data());
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    renderer_id_ = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(renderer_id_, vertexShader);
    glAttachShader(renderer_id_, fragmentShader);

    // Link our program
    glLinkProgram(renderer_id_);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(renderer_id_, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(renderer_id_, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the program anymore.
        glDeleteProgram(renderer_id_);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        METEOR_CORE_ASSERT(false, "Shader linking failed: {}", infoLog.data());
    }

    // Always detach shaders after a successful link.
    glDetachShader(renderer_id_, vertexShader);
    glDetachShader(renderer_id_, fragmentShader);
}

meteor::Shader::~Shader()
{
    glDeleteProgram(renderer_id_);
}

void meteor::Shader::Bind()
{
    glUseProgram(renderer_id_);
}

void meteor::Shader::Unbind()
{
    glUseProgram(0);
}
