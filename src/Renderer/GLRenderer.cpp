#include "GLRenderer.h"

GLRenderer::GLRenderer(void)
{
}

bool GLRenderer::InitRenderer(SDL_Window* windowToRenderTo)
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	
	gContext = SDL_GL_CreateContext( windowToRenderTo );



	if( gContext == NULL )
	{
			printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
			return false; 
	}
	else 
	{
			//Initialize GLEW
			glewExperimental = GL_TRUE; 
			GLenum glewError = glewInit();
			if( glewError != GLEW_OK )
			{
				printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
			}

			//Use Vsync
			if( SDL_GL_SetSwapInterval( 1 ) < 0 )
			{
				printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
			}

			CurrentWindow = windowToRenderTo; 
			startup(); 
	}



	return true;
}

void GLRenderer::startup()
{
	 rendering_program = compile_shaders();
     glGenVertexArrays(1, &vertex_array_object);
     glBindVertexArray(vertex_array_object);
}
		


GLuint GLRenderer::compile_shaders()
{
	GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    // Source code for vertex shader
    static const GLchar * vertex_shader_source[] =
    {
        "#version 150 core                                  \n"
        "                                                   \n"
        "void main(void)                                    \n"
        "{                                                  \n"
        "    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);        \n"
        "}                                                  \n"
    };

    // Source code for fragment shader
    static const GLchar * fragment_shader_source[] =
    {
        "#version 150 core                                  \n"
        "                                                   \n"
        "out vec4 color;                                    \n"
        "                                                   \n"
        "void main(void)                                    \n"
        "{                                                  \n"
        "    color = vec4(0.0, 0.8, 1.0, 1.0);              \n"
        "}                                                  \n"
    };

    // Create and compile vertex shader
	GLint vShaderCompiled = GL_FALSE;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

	glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &vShaderCompiled );
	if( vShaderCompiled != GL_TRUE )
	{
		printf( "Unable to compile Fragment shader %d!\n", vertex_shader );
		printShaderLog( vertex_shader );
        //success = false;
	}

    // Create and compile fragment shader
	GLint vFragCompiled = GL_FALSE;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader,GL_COMPILE_STATUS, &vFragCompiled); 
	if( vFragCompiled != GL_TRUE )
	{
		printf( "Unable to compile vertex shader %d!\n", fragment_shader );
		printShaderLog( fragment_shader );
        //success = false;
	}

    // Create program, attach shaders to it, and link it
	
	
    program = glCreateProgram();
	glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv( program, GL_LINK_STATUS, &programSuccess );
	if( programSuccess != GL_TRUE )
	{
		printf( "Error linking program %d!\n", program );
		printProgramLog( program );
	}

   

	

    // Delete the shaders as the program has them now
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

void GLRenderer::shutdown()
{
	glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1, &vertex_array_object);
}

void GLRenderer::Clean()
{
	 shutdown(); 
}

void GLRenderer::Render()
{
	
	const GLfloat color[4] = { (float)sin((float)SDL_GetTicks()/1000) * 0.5f + 0.5f,
                               (float) cos((float)SDL_GetTicks()/1000) * 0.5f + 0.5f,
                                0.0f, 1.0f };
								
	//static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, color);
    

    // Use the program object we created earlier for rendering
    glUseProgram(rendering_program);

	glPointSize(40.0f);
	
	// Draw one point
    glDrawArrays(GL_POINTS, 0, 1);

	

	SDL_GL_SwapWindow( CurrentWindow );
}


void GLRenderer::printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = new char[ maxLength ];
		
		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}

void GLRenderer::printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = new char[ maxLength ];
		
		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}
		
		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a program\n", program );
	}
}

GLRenderer::~GLRenderer(void)
{
}
