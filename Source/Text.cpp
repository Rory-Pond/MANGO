#include "Text.h"
/*
void WriteToScreen(std::string text)
{
TTF_Init();
glEnable(GL_TEXTURE_2D);
glEnable(GL_BLEND);

glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B,
const double& X, const double& Y, const double& Z, const std::string& Text)
{
//Create some variables.
SDL_Color Color = { R, G, B };
SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
unsigned Texture = 0;

//Generate an OpenGL 2D texture from the SDL_Surface*.
glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_2D, Texture);

glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA,
	GL_UNSIGNED_BYTE, Message->pixels);

//Draw this texture on a quad with the given xyz coordinates.
glBegin(GL_QUADS);
glTexCoord2d(0, 0); glVertex3d(X, Y, Z);
glTexCoord2d(1, 0); glVertex3d(X + Message->w, Y, Z);
glTexCoord2d(1, 1); glVertex3d(X + Message->w, Y + Message->h, Z);
glTexCoord2d(0, 1); glVertex3d(X, Y + Message->h, Z);
glEnd();

//Clean up.//
glDeleteTextures(1, &Texture);
SDL_FreeSurface(Message);
}

#include "SDL_ttf.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_DEPTH 8

enum textquality { solid, shaded, blended };

// Loads a font, nothing fancy.
// Put sensible error handling code in. If you pass the NULL in later on,
// SDL_ttf dies pretty horribly.
TTF_Font* loadfont(char* file, int ptsize)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, ptsize);
	if (tmpfont == NULL) {
		printf("Unable to load font: %s %s \n", file, TTF_GetError());
		// Handle the error here.
	}
	return tmpfont;
}

// SDL_ttf has a few modes you can draw with, check its documentation.
SDL_Surface *drawtext(TTF_Font* fonttodraw, char fgR, char fgG, char fgB, char fgA,
	char bgR, char bgG, char bgB, char bgA, char text[], textquality quality)
{
	SDL_Color tmpfontcolor = { fgR,fgG,fgB,fgA };
	SDL_Color tmpfontbgcolor = { bgR, bgG, bgB, bgA };
	SDL_Surface* resulting_text;

	if (quality == solid) {
		resulting_text = TTF_RenderText_Solid(fonttodraw, text, tmpfontcolor);
	}
	else if (quality == shaded) {
		resulting_text = TTF_RenderText_Shaded(fonttodraw, text, tmpfontcolor, tmpfontbgcolor);
	}
	else if (quality == blended) {
		resulting_text = TTF_RenderText_Blended(fonttodraw, text, tmpfontcolor);
	}

	return resulting_text;
}


int TEXT() {
	// Must call initialise first.
	TTF_Init();

	// This loads courier, but any font will do.
	TTF_Font* font = loadfont("C:/windows/fonts/cour.ttf", 10);
	SDL_Surface* surface = drawtext(font, 255, 255, 255, 70, 0, 0, 0, 0, "test", blended);

	// End of SDL_ttf stuff, this is just SDL to display the text we made.
	SDL_Surface* screen;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
	SDL_BlitSurface(surface, NULL, screen, NULL);
	SDL_Flip(screen);

	while (1);

	return 0;
}
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

using std::cerr;
using std::endl;

int main(int argc, char* args[])
{
// Initialize the SDL
if (SDL_Init(SDL_INIT_VIDEO) != 0)
{
cerr << "SDL_Init() Failed: " <<
SDL_GetError() << endl;
exit(1);
}

// Set the video mode
SDL_Surface* display;
display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
if (display == NULL)
{
cerr << "SDL_SetVideoMode() Failed: " <<
SDL_GetError() << endl;
exit(1);
}

// Set the title bar
SDL_WM_SetCaption("SDL Tutorial", "SDL Tutorial");

// Initialize SDL_ttf library
if (TTF_Init() != 0)
{
cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
SDL_Quit();
exit(1);
}

// Load a font
TTF_Font *font;
font = TTF_OpenFont("FreeSans.ttf", 24);
if (font == NULL)
{
cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
TTF_Quit();
SDL_Quit();
exit(1);
}

// Write text to surface
SDL_Surface *text;
SDL_Color text_color = {255, 255, 255};
text = TTF_RenderText_Solid(font,
"A journey of a thousand miles begins with a single step.",
text_color);

if (text == NULL)
{
cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << endl;
TTF_Quit();
SDL_Quit();
exit(1);
}

// Main loop
SDL_Event event;
while(1)
{
// Check for messages
if (SDL_PollEvent(&event))
{
// Check for the quit message
if (event.type == SDL_QUIT)
{
// Quit the program
break;
}
}

// Clear the screen
if (SDL_FillRect(display,
NULL,
SDL_MapRGB( display->format, 0,0,0))
!= 0)
{
cerr << "SDL_FillRect() Failed: " << SDL_GetError() << endl;
break;
}

// Apply the text to the display
if (SDL_BlitSurface(text, NULL, display, NULL) != 0)
{
cerr << "SDL_BlitSurface() Failed: " << SDL_GetError() << endl;
break;
}

//Update the display
SDL_Flip(display);

}

// Shutdown the TTF library
TTF_Quit();

// Tell the SDL to clean up and shut down
SDL_Quit();

return 0;
}

*/