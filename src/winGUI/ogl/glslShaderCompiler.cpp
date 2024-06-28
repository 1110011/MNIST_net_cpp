#include "glslShaderCompiler.h"

glslShader::glslShader(const std::string vertexPath, const std::string fragmentPath)
{
    std::string vShader_str = "";
    std::string fShader_str = "";
    std::string line = "";

    std::ifstream f(vertexPath);

    if(!f.good())
    {
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::VERTEX::LOADING_FAILED\n";
        #endif // DDEBUG
        this->id = (unsigned int)-1;
        return;
    }

    while(!f.eof())
    {
        std::getline(f,line);
        vShader_str += line;
        vShader_str += '\n';
    }
    vShader_str += '\0';

    const char* vShader = vShader_str.c_str();

    f.close();

    f.open(fragmentPath);

    if(!f.good())
    {
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::FRAGMENT::LOADING_FAILED\n";
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }

    while(!f.eof())
    {
        std::getline(f,line);
        fShader_str += line;
        fShader_str += '\n';
    }
    fShader_str += '\0';

    const char* fShader = fShader_str.c_str();

    f.close();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShader, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

glslShader::glslShader(const unsigned int vertexID, const unsigned int fragmentID)
{
    std::string vShader_str = "";
    std::string fShader_str = "";
    std::string line = "";

    // find file data resource
    HRSRC fdRes = FindResourceA(NULL, MAKEINTRESOURCE(vertexID), MAKEINTRESOURCE(256));
    // get file size
    size_t fdRSize = (size_t)SizeofResource(NULL, fdRes);
    // get resource pointer
    HGLOBAL fdRp = LoadResource(NULL, fdRes);
    unsigned int fdRpOf = 0;

    while(fdRpOf < fdRSize)
    {
        line = resourceGetLine(fdRpOf, fdRp, fdRSize);
        vShader_str += line;
        vShader_str += '\n';
    }
    vShader_str += '\0';

    const char* vShader = vShader_str.c_str();

    // find file data resource
    fdRes = FindResourceA(NULL, MAKEINTRESOURCE(fragmentID), MAKEINTRESOURCE(256));
    // get file size
    fdRSize = (size_t)SizeofResource(NULL, fdRes);
    // get resource pointer
    fdRp = LoadResource(NULL, fdRes);
    fdRpOf = 0;

    while(fdRpOf < fdRSize)
    {
        line = resourceGetLine(fdRpOf, fdRp, fdRSize);
        fShader_str += line;
        fShader_str += '\n';
    }
    fShader_str += '\0';

    const char* fShader = fShader_str.c_str();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShader, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        #ifdef DEBUG
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        #endif // DEBUG
        this->id = (unsigned int)-1;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void glslShader::use(void)
{
    glUseProgram(this->id);
}
