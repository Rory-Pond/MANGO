#include <stdio.h>
#pragma warning(disable: 4996)
#include "Output.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

/*
Write the current view to a file
The multiple fputc()s can be replaced with
fwrite(image,width*height*3,1,fptr);
If the memory pixel order is the same as the destination file format.
*/
int WindowDump(void)
{
	int i, j;
	FILE *fptr;
	static int counter = 0; /* This supports animation sequences */
	char fname[32];
	unsigned char *image;
	const int width = 1600, height = 900;
	/* Allocate our buffer for the image */
	if ((image = (unsigned char*)malloc(3 * width*height*sizeof(char))) == NULL) {
		fprintf(stderr, "Failed to allocate memory for image\n");
		return(false);
	}

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	bool stereo = false;

	/* Open the file */
	if (stereo)
		sprintf(fname, "L_%04d.raw", counter);
	else
		sprintf(fname, "C_%04d.raw", counter);
	if ((fptr = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "Failed to open file for window dump\n");
		return(false);
	}

	/* Copy the image into our buffer */
	glReadBuffer(GL_BACK_LEFT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

	/* Write the raw file */
	/* fprintf(fptr,"P6\n%d %d\n255\n",width,height); for ppm */
	for (j = height - 1; j >= 0; j--) {
		for (i = 0; i<width; i++) {
			fputc(image[3 * j*width + 3 * i + 0], fptr);
			fputc(image[3 * j*width + 3 * i + 1], fptr);
			fputc(image[3 * j*width + 3 * i + 2], fptr);
		}
	}
	fclose(fptr);

	if (stereo) {
		/* Open the file */
		sprintf(fname, "R_%04d.raw", counter);
		if ((fptr = fopen(fname, "w")) == NULL) {
			fprintf(stderr, "Failed to open file for window dump\n");
			return(false);
		}

		/* Copy the image into our buffer */
		glReadBuffer(GL_BACK_RIGHT);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

		/* Write the raw file */
		/* fprintf(fptr,"P6\n%d %d\n255\n",width,height); for ppm */
		for (j = height - 1; j >= 0; j--) {
			for (i = 0; i<width; i++) {
				fputc(image[3 * j*width + 3 * i + 0], fptr);
				fputc(image[3 * j*width + 3 * i + 1], fptr);
				fputc(image[3 * j*width + 3 * i + 2], fptr);
			}
		}
		fclose(fptr);
	}

	/* Clean up */
	counter++;
	free(image);
	return(true);
}
