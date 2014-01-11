#include "GLRenderer.h"


#include "GLDebugPrinting.h"


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


    static const GLchar * vertex_shader_source[] =
    {
        "#version 430 core                          \n"
        "                                           \n"
        "layout (location = 0) in vec4 offset;      \n"
        "layout (location = 1) in vec4 color;       \n"
        "                                           \n"
        "out VS_OUT                                 \n"
        "{                                          \n"
        "   vec4 color;                             \n"
        "}vs_out;                                   \n"
        "                                           \n"
        "void main (void)                           \n"
        "{                                          \n"
        "                                           \n"
        "const vec4 verticies[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),     \n"
        "                                  vec4(-0.25, -0.25, 0.5, 1.0),     \n"
        "                                  vec4( 0.25,  0.25, 0.5, 1.0));   \n"
        "                                           \n"
        "   gl_Position = verticies[gl_VertexID] + offset;   \n"
        "                                           \n"
        "                                           \n"
        "   vs_out.color = color;                   \n"
        "}                                          \n"
    };

    static const GLchar* fragment_shader_source[] =
    {
        "#version 430 core                          \n"
        "                                           \n"
        "                                           \n"
        "in VS_OUT                                  \n"
        "{                                          \n"
        "   vec4 color;                             \n"
        "}fs_in;                                   \n"
        "                                           \n"
        "                                           \n"
        "out vec4 color;                            \n"
        "                                           \n"
        "void main(void)                            \n"
        "{                                          \n"
        "   color = fs_in.color;                    \n"
        "}                                          \n"
    };


    //Compile the Vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);


    //Compile Frag shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);


    //Create shader program ;

    program = glCreateProgram();

    glAttachShader(program,vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;


}

void GLRenderer::shutdown()
{
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1, &vertex_array_object) ;

}

void GLRenderer::Clean()
{
	 shutdown();
}

void GLRenderer::Render(float currentTime)
{

    const GLfloat color[] = {sin(currentTime) * 0.5f + 0.5f,
                             cos(currentTime) * 0.5f + 0.5f,
                             0.0f, 1.0f };

    glClearBufferfv (GL_COLOR, 0, color);

    glPointSize(40.0f);

    glUseProgram(rendering_program);

    GLfloat attrib[] = { (float) sin(currentTime) * 0.5f,
                         (float) cos(currentTime) * 0.6f,
                          0.0, 0.0};


     GLfloat triColor[] = {1.0, 0.0, 0.0, 1.0};

    glVertexAttrib4fv(0,attrib);
    glVertexAttrib4fv(1,triColor);


    glDrawArrays(GL_TRIANGLES, 0,3);


/*************************************************************************/
    //For SDL DON'T Delete
	SDL_GL_SwapWindow( CurrentWindow );
}




GLRenderer::~GLRenderer(void)
{
}
