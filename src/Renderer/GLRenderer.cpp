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

    //glPatchParameteri(GL_PATCH_VERTICES, 3);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



}



GLuint GLRenderer::compile_shaders()
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint Geometry_shader;
    GLuint tesselation_Control_shader;
    GLuint tesselation_Eval_shader;
    GLuint program;


 static const GLchar * vertex_shader_source[] =
    {
        "#version 430 core                          \n"
	    "layout (location = 0) in vec4 offset;       \n"
        "                                           \n"
        "out vec4 vs_color;                        \n"
        "                                           \n"
        "                                           \n"
        "                                           \n"
        "void main (void)                           \n"
        "{                                          \n"
        "                                           \n"
        "const vec4 verticies[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),     \n"
        "                                  vec4(-0.25, -0.25, 0.5, 1.0),     \n"
        "                                  vec4( 0.25,  0.25, 0.5, 1.0));    \n"
        "                                           \n"
        "const vec4 colors[] = vec4[3](vec4( 1.0,  0.0, 0.0, 1.0),     \n"
        "                              vec4( 0.0,  1.0, 0.0, 1.0),     \n"
        "                              vec4( 0.0,  0.0, 1.0, 1.0));    \n"
        "                                           \n"
        "                                           \n"
        "gl_Position = verticies[gl_VertexID] + offset;    \n"
        "                                           \n"
        "vs_color = colors[gl_VertexID];             \n"
        "}                                          \n"
    };

    static const GLchar* fragment_shader_source[] =
    {
        "#version 430 core                          \n"
        "                                           \n"
        "in vec4 vs_color;                          \n"
        "                                           \n"
        "                                           \n"
        "out vec4 color;                            \n"
        "                                           \n"
        "void main(void)                            \n"
        "{                                          \n"
        "                                           \n"
        "   color = vs_color;                      \n"
        "                                           \n"
        "}                                          \n"
    };

    static const GLchar * tesselation_Control_shader_source[] =
    {
        "#version 410 core                          \n"
        "                                           \n"
        "layout (vertices = 3) out;                 \n"
        "                                           \n"
        "                                           \n"
        "void main(void)                            \n"
        "{                                          \n"
        "    if(gl_InvocationID == 0)               \n"
        "    {                                      \n"
        "         gl_TessLevelInner[0] = 5.0;       \n"
        "         gl_TessLevelOuter[0] = 5.0;       \n"
        "         gl_TessLevelOuter[1] = 5.0;       \n"
        "         gl_TessLevelOuter[2] = 5.0;       \n"
        "    }                                       \n"
        "                                           \n"
        "    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;      \n"
        "}                                          \n"

    };

     static const GLchar * tesselation_Eval_shader_source[] =
    {
        "#version 410 core                          \n"
        "                                           \n"
        "layout (triangles, equal_spacing, cw) in;  \n"
        "                                           \n"
        "                                           \n"
        "void main(void)                            \n"
        "{                                          \n"
        "     gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + \n"
        "                   (gl_TessCoord.y * gl_in[1].gl_Position) + \n"
        "                   (gl_TessCoord.z * gl_in[2].gl_Position); \n"
        "                                                            \n"
        "}                                                           \n"
    };


  static const GLchar * Geometry_shader_source[] =
    {
        "#version 410 core                          \n"
        "                                           \n"
        "layout (triangles) in;                     \n"
        "layout (points, max_vertices = 3)  out;   \n"
        "                                           \n"
        "void main(void)                            \n"
        "{                                          \n"
        "    int i;                                 \n"
        "                                           \n"
        "                                           \n"
        "    for(i = 0; i< gl_in.length(); i++)     \n"
        "    {                                      \n"
        "        gl_Position = gl_in[i].gl_Position; \n"
        "        EmitVertex();                     \n"
        "    }                                      \n"
        "                                           \n"
        "}                                          \n"
    };

    //Compile the Vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &vShaderCompiled );
    if( vShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile Vertex shader %d!\n", vertex_shader );
        printShaderLog( vertex_shader );
    }


    //Compile Frag shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv( fragment_shader, GL_COMPILE_STATUS, &fShaderCompiled );
    if( fShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile fragment shader %d!\n", fragment_shader );
        printShaderLog( fragment_shader );
    }

    tesselation_Control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tesselation_Control_shader, 1 , tesselation_Control_shader_source, NULL);
    glCompileShader(tesselation_Control_shader);

    GLint tcShaderCompiled = GL_FALSE;
    glGetShaderiv( tesselation_Control_shader, GL_COMPILE_STATUS, &tcShaderCompiled );
    if( tcShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile Tesslaton Control shader %d!\n", tesselation_Control_shader );
        printShaderLog( tesselation_Control_shader );
    }

    tesselation_Eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tesselation_Eval_shader, 1, tesselation_Eval_shader_source, NULL);
    glCompileShader(tesselation_Eval_shader);

    GLint teShaderCompiled = GL_FALSE;
    glGetShaderiv( tesselation_Eval_shader, GL_COMPILE_STATUS, &teShaderCompiled );
    if( teShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile Tesslaton Evaluation shader %d!\n", tesselation_Eval_shader );
        printShaderLog( tesselation_Eval_shader );
    }

    Geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(Geometry_shader,1, Geometry_shader_source, NULL );
    glCompileShader(Geometry_shader);

    GLint geoShaderCompiled = GL_FALSE;
    glGetShaderiv(Geometry_shader, GL_COMPILE_STATUS, &geoShaderCompiled);
    if(geoShaderCompiled != GL_TRUE)
    {

        printf( "Unable to compile Geometry shader %d!\n", Geometry_shader );
        printShaderLog( Geometry_shader );
    }




    //Create shader program ;

    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
   // glAttachShader(program, tesselation_Control_shader);
    //glAttachShader(program, tesselation_Eval_shader);
    //glAttachShader(program, Geometry_shader);

    glLinkProgram(program);

    GLint programSuccess = GL_TRUE;
    glGetProgramiv( program, GL_LINK_STATUS, &programSuccess );
    if( programSuccess != GL_TRUE )
    {
        printf( "Error linking program %d!\n", program );
        printProgramLog( program );
    }


    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(tesselation_Control_shader);
    glDeleteShader(tesselation_Eval_shader);





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

    glPointSize(5.0f);

    //


    glUseProgram(rendering_program);

    GLfloat attrib[] = { (float) sin(currentTime) * 0.5f,
                         (float) cos(currentTime) * 0.6f,
                          0.0, 0.0};


     GLfloat triColor[] = {0.0, 0.8, 1.0, 1.0};

    glVertexAttrib4fv(0,attrib);
    glVertexAttrib4fv(1,triColor);



    //He fails to mention in the book to switch to patches.
    //glDrawArrays(GL_PATCHES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 0, 3);


/*************************************************************************/
    //For SDL DON'T Delete
	SDL_GL_SwapWindow( CurrentWindow );
}




GLRenderer::~GLRenderer(void)
{
}
