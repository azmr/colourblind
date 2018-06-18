#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <blit-fonts/blit16.h> /* https://github.com/azmr/blit-fonts */

#include "colourblind.h"
#define ArrayCount(a) (sizeof(a)/sizeof(*a))

int CALLBACK
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
	// UNUSED:
	Instance; ShowCode; CommandLine; PrevInstance;

	if(! glfwInit()) { return -1; }
	GLFWwindow *Window = glfwCreateWindow(640, 480, "Colourblind spectra", 0, 0);
	if(! Window) { glfwTerminate(); return -2; }
	glfwMakeContextCurrent(Window);

	while(!glfwWindowShouldClose(Window))
	{
		int W, H;
		glfwGetFramebufferSize(Window, &W, &H);
		glViewport(0, 0, W, H);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,  W,
				H,  0,  
				-1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);

		glClear(GL_COLOR_BUFFER_BIT);
		float Height = H/(float)VISION_COUNT;
		for(int iVision = 0; iVision < cbImpairmentCount; ++iVision)
		{
			// Draw a spectrum
			visual_impairment Impairment = iVision;
			float XLo = 0, XHi = 0, YLo = iVision * Height, YHi = (iVision + 1) * Height;
			double R, G, B;
			float Rs[] = { 1.f,1.f, 0.f,0.f, 0.f,1.f };
			float Gs[] = { 0.f,1.f, 1.f,1.f, 0.f,0.f };
			float Bs[] = { 0.f,0.f, 0.f,1.f, 1.f,1.f };
			int N = ArrayCount(Rs);
			glBegin(GL_QUADS);
			for(int i = 0; i < N; ++i)
			{
				XLo = XHi, XHi += W/(float)N;
				R = Rs[i], G = Gs[i], B = Bs[i];
				ColourBlind(Impairment, &R, &G, &B);
				glColor3f((float)R, (float)G, (float)B);
				glVertex2f(XLo, YLo);
				glVertex2f(XLo, YHi);

				R = Rs[(i+1)%N], G = Gs[(i+1)%N], B = Bs[(i+1)%N];
				ColourBlind(Impairment, &R, &G, &B);
				glColor3f((float)R, (float)G, (float)B);
				glVertex2f(XHi, YHi);
				glVertex2f(XHi, YLo);
			}
			glEnd();


			// Write labels
#define TextLen (sizeof("Deuteranopia")+1)
#define TextScale 3
#define TextWidth (int)(TextLen * TextScale * blit16_ADVANCE)
#define TextHeight (TextScale * blit16_ROW_ADVANCE)
#define BGHeight (4*TextHeight/2)
			blit_pixel TextTexture[TextWidth * BGHeight] = {0};
			memset(TextTexture, 0xFF, sizeof(TextTexture));
			blit_pixel Black = 0x000000FF;
			// This is drawn flipped so that OpenGL will flip it back to the right orientation
			blit16_TextExplicit(TextTexture, Black, TextScale, -TextWidth, BGHeight, 0,
				blit16_ADVANCE * TextScale, BGHeight/2 + TextHeight/2 - TextScale,
				VisionImpairmentStrings[iVision]);
			glRasterPos2f(W/2.f-TextWidth/2.f, YLo + Height/2.f + BGHeight/2.f);
			glDrawPixels(TextWidth, BGHeight, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, TextTexture);
		}

		// Add dividing lines
		glColor3f(1.f,1.f,1.f);
		float Stroke = 3.f;
		for(int i = 1; i < VISION_COUNT; ++i)
		{
			glBegin(GL_QUADS);
			glVertex2f(0.f, i*Height-Stroke);
			glVertex2f(0.f, i*Height+Stroke);
			glVertex2f((float)W, i*Height+Stroke);
			glVertex2f((float)W, i*Height-Stroke);
			glEnd();
		}


		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glfwDestroyWindow(Window);
	glfwTerminate();

	return 0;
}
