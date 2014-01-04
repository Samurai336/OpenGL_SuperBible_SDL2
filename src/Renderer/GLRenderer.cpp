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

}



GLuint GLRenderer::compile_shaders()
{

}

void GLRenderer::shutdown()
{

}

void GLRenderer::Clean()
{
	 shutdown();
}

void GLRenderer::Render(float currentTime)
{

    static const GLfloat red[] = {1.0f, 0.0f,0.0f, 1.0f,};

    glClearBufferfv(GL_COLOR, 0, red);

    //For SDL
	SDL_GL_SwapWindow( CurrentWindow );
}




GLRenderer::~GLRenderer(void)
{
}
