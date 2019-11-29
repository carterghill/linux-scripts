#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

typedef enum { false, true } bool;

struct {

	int width;
	int height;
	int radius;

} settings;

void draw(cairo_t *cr) {

	int x = 0;
	int y = 0;
	int w = settings.width;
	int h = settings.height;
	int r = settings.radius;

	double angle1 = 270 * (3.14159 / 180);
	double angle2 = 0;
	double angle3 = 90 * (3.14159 / 180);
	double angle4 = 180 * (3.14159 / 180);

	/* Fill the rectangle */
	cairo_set_source_rgba(cr, 0, 0, 0, 0.8);
	cairo_move_to(cr, x+r, y);
	cairo_line_to(cr, w-r, y);
	cairo_arc(cr, w-r, y+r, r, angle1, angle2);
	cairo_line_to(cr, w, h-r);
	cairo_arc(cr, w-r, h-r, r, angle2, angle3);
	cairo_line_to(cr, r, h);
	cairo_arc(cr, r, h-r, r, angle3, angle4);
	cairo_line_to(cr, x, y+r);
	cairo_arc(cr, r, r, r, angle4, angle1);
	cairo_fill(cr);

	/* Draw Border */
	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_move_to(cr, x+r, y);
	cairo_line_to(cr, w-r, y);
	cairo_arc(cr, w-r, y+r, r, angle1, angle2);
	cairo_line_to(cr, w, h-r);
	cairo_arc(cr, w-r, h-r, r, angle2, angle3);
	cairo_line_to(cr, r, h);
	cairo_arc(cr, r, h-r, r, angle3, angle4);
	cairo_line_to(cr, x, y+r);
	cairo_arc(cr, r, r, r, angle4, angle1);	
	cairo_stroke(cr);

}

void init_settings() {

	settings.width = 800;
	settings.height = 500;
	settings.radius = 12;

}

int main(int argc, char* argv[]) {

	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	Window root = DefaultRootWindow(d);
	Window overlay;
	cairo_t* cr;
	cairo_surface_t* surf;
	XSetWindowAttributes attrs;
	XVisualInfo vinfo;

	XEvent e;
	char buf[8];
	KeySym key;
	int numKeys = 0;
	char input[256];
	int index = 0;

	init_settings();

	attrs.override_redirect = true;

	if (!XMatchVisualInfo(d, DefaultScreen(d), 32, TrueColor, &vinfo)) {
		printf("No visual found supporting 32 bit color, terminating\n");
		return 1;
	}

	attrs.colormap = XCreateColormap(d, root, vinfo.visual, AllocNone);
	attrs.background_pixel = 0;
	attrs.border_pixel = 0;

	overlay = XCreateWindow(
	    d, root,
	    s->width/2 - settings.width/2, 
	    s->height/2 - settings.height/2, 
	    settings.width, settings.height, 
	    0, vinfo.depth, InputOutput, 
	    vinfo.visual,
	    CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, 
	    &attrs
	);

	XMapWindow(d, overlay);

	surf = cairo_xlib_surface_create(d, overlay, vinfo.visual, 
			settings.width, settings.height);
	cr = cairo_create(surf);

	draw(cr);
	XFlush(d);

	/* Take all keyboard input */
	if(XGrabKeyboard(d,root,true,GrabModeAsync,GrabModeAsync,CurrentTime) 
			== GrabSuccess) {
	} else {
		printf("Grab Unsuccessful\n");
		return 1;
	}

	/* Eat the initial enter input */
	XNextEvent(d, &e);

	while (true){
		
		/* Wait for the next event */
		XNextEvent(d, &e);

		/* Skip loop on key release to avoid double input */
		if (e.xkey.type == KeyRelease)
			continue;

		/* If number of characters read > 0 */
		if((numKeys = XLookupString(&e.xkey, buf, 8, &key, 0))) {

			/* Escape key pressed, exit */
			if ((int)buf[0] == 27)
				break;

			/* Delete key was pressed */
			if ((int)buf[0] == 8 && index > 0) {
				index--;
				input[index] = '\0';
				continue;
			}

			/* Enter key was pressed */
			if ((int)buf[0] == 13) {
				printf("enter key\n");
				system(input);
				break;
			} 

			input[index] = (char) buf[0];
			index++;
			input[index] = '\0';

			printf("%s\n", input);

		}
	

	}

	printf("input: %s\n", input);

	cairo_destroy(cr);
	cairo_surface_destroy(surf);

	XUnmapWindow(d, overlay);
	XCloseDisplay(d);

	return 1;

}
