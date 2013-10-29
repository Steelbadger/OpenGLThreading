#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, GLuint> Shader::ShaderLibrary;

Shader::Shader(void)
{
}

Shader::Shader(std::string s)
{
	Create(s);
}

Shader::~Shader(void)
{
}

void Shader::Create(std::string fileName)
/*-------------------------------------------------------------------------*\
|	Purpose:	Load a shader component from file and compile							|
|																			|
|	Parameters:	The filename string of the shader file to load				|
\*-------------------------------------------------------------------------*/
{
	std::string fn = fileName;
	//  If no file name specified quit
	if (fn.empty()) {
		return;
	}

	//  Check we haven't already done this file
	if (!ShaderLibrary.count(fn)) {
		GLuint shader;

		//  Get the file extension so that we know what kind of shader it is
		std::string type = fn.substr(fn.find_last_of(".") + 1);

		//  depending on extension, create a shader ref of the corresponding type
		if (type == "vertexshader") {
			shader = glCreateShader(GL_VERTEX_SHADER);
		} else if (type == "fragmentshader") {
			shader = glCreateShader(GL_FRAGMENT_SHADER);
		} else if (type == "tesscontrol") {
			shader = glCreateShader(GL_TESS_CONTROL_SHADER);
		} else if (type == "tessevaluation") {
			shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		} else {
			//  If the file extension is not correct then return an error and stop
			std::cout << "Unrecognised Shader File Extension: " << type << std::endl;
			return;
		}
	
		//  Open the file
		std::string code;
		std::ifstream fileStream(fn, std::ios::in);
		if (!fileStream.is_open()) {
			std::cout << "Could not open shader file : " << fileName << std::endl;
			return;
		}
		//  Load in the code to a stream
		std::string line = "";
		while(getline(fileStream, line)) {
			code += "\n" + line;
		}
		fileStream.close();

		//  compile that code
		std::cout << "Compiling " << fn << std::endl;
		char const * codePointer = code.c_str();
		glShaderSource(shader, 1, &codePointer, NULL);
		glCompileShader(shader);


		//  Check GLSL compiler output for errors
		GLint result = GL_FALSE;
		int logLength;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> ShaderErrorMessage(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, &ShaderErrorMessage[0]);
		std::string output(ShaderErrorMessage.begin(), ShaderErrorMessage.end());
		std::cout << output << std::endl;


		//  If we failed to compile the shader quit without adding the the shadermap
		if (result == GL_FALSE) {
			return;
		}

		//  Successfully compiled, so add to library
		ShaderLibrary[fn] = shader;
	}

	//  Get the shader reference form the library for use later
	shaderRef = ShaderLibrary[fn];
}



ShaderProgram::ShaderProgram()
{
	programRef = 0;
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::AddShader(Shader s)
{
	shaders.push_back(s);
}

void ShaderProgram::Compile()
/*-------------------------------------------------------------------------*\
|	Purpose:	Link the shader components into a program					|
\*-------------------------------------------------------------------------*/
{
	//  Create the shader porgram reference
	programRef = glCreateProgram();

	//  Attach all the shaders components
	for (int i = 0; i < shaders.size(); i++) {
		glAttachShader(programRef, shaders[i].Reference());
	}

	//  Link em
	glLinkProgram(programRef);

	//  Check the log for errors
	GLint result = GL_FALSE;
	int logLength;

	glGetProgramiv(programRef, GL_LINK_STATUS, &result);
	glGetProgramiv(programRef, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> ProgramErrorMessage(logLength);
	glGetProgramInfoLog(programRef, logLength, NULL, &ProgramErrorMessage[0]);
	std::string output(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
	std::cout << output << std::endl;

	FindUniformLocations();
}

void ShaderProgram::FindUniformLocations()
/*-------------------------------------------------------------------------*\
|	Purpose:	Find all Uniforms within the linked shader program			|
|				these all have standardized names within all shader files	|
\*-------------------------------------------------------------------------*/
{
	UniformLocations newLocations;

	//  Find the matrix locations
	newLocations.ProjectionMatrix = glGetUniformLocation(programRef, "projectionMatrix");
	newLocations.ViewMatrix = glGetUniformLocation(programRef, "viewMatrix");
	newLocations.ModelMatrix = glGetUniformLocation(programRef, "modelMatrix");
	newLocations.ModelViewMatrix = glGetUniformLocation(programRef, "modelViewMatrix");
	newLocations.NormalMatrix = glGetUniformLocation(programRef, "normalMatrix");

	//  Find the other standard locations
	newLocations.NumLights = glGetUniformLocation(programRef, "numLights");
	newLocations.MapWidth = glGetUniformLocation(programRef, "mapWidth");
	newLocations.Magnitude = glGetUniformLocation(programRef, "magnitude");
	newLocations.Time = glGetUniformLocation(programRef, "time");
	newLocations.XMulFactor = glGetUniformLocation(programRef, "xmul");
	newLocations.YMulFactor = glGetUniformLocation(programRef, "ymul");

	std::string nameString;
	std::stringstream stream;
	//  Lights take a bit more effort
	for (int i = 0; i < MAXLIGHTS; i++) {
		//  Create the string to search for; eg. lights[0].colour by combining the counter and string concatenation
		stream << "lights[" << i << "].colour";

		//  Get the created string out of the stringstream
		nameString = stream.str();

		//  Clear the stringstream for next usage
		stream.str(std::string());

		newLocations.LightColours[i] = glGetUniformLocation(programRef, nameString.c_str());

		stream << "lights[" << i << "].position";
		nameString = stream.str();
		stream.str(std::string());

		newLocations.LightPositions[i] = glGetUniformLocation(programRef, nameString.c_str());

		stream << "lights[" << i << "].type";
		nameString = stream.str();
		stream.str(std::string());

		newLocations.LightTypes[i] = glGetUniformLocation(programRef, nameString.c_str());
	}

	for (int i = 0; i < MAX_TEXTURES; i++) {
		//  Create the string to search for; eg. lights[0].colour by combining the counter and string concatenation
		stream << "texture" << i+1;

		//  Get the created string out of the stringstream
		nameString = stream.str();

		//  Clear the stringstream for next usage
		stream.str(std::string());

		newLocations.Textures[i] = glGetUniformLocation(programRef, nameString.c_str());
	}
		
	uniforms = newLocations;
}