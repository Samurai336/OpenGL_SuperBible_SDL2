#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <math.h>



#ifdef APPLE
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "../Define.h"

#include "FrameRateController.h"



class GLRenderer
{
	public:
		GLRenderer(void);
		~GLRenderer(void);

		bool InitRenderer(SDL_Window*);

		FrameRateController FrameRateControl;
		GLuint compile_shaders(void);


		void startup();
		void shutdown();


		void Clean();

		void Render(float currentTime);



	private:

		SDL_GLContext gContext;
		SDL_Window*	  CurrentWindow;

		GLuint  rendering_program;
		GLuint  vertex_array_object;
};
