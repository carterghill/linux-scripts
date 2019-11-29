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

void draw(cairo_t *cr) {
	cairo_set_source_rgba(cr, 0.8, 0.8, 0.8, 0.8);
	cairo_rectangle(cr, 0, 0, 200, 200);
	cairo_fill(cr);
}


int main(int argc, char* argv[]) {

	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	Window root = DefaultRootWindow(d);
	int default_screen = XDefaultScreen(d);
	Window overlay;

	int i;
	XEvent e;
	char buf[8];
	KeySym key;
	int numKeys = 0;
	char input[256];
	int index = 0;

	input[0] = '\0';

	// thes two lines are really all you need
	XSetWindowAttributes attrs;
	attrs.override_redirect = true;

	XVisualInfo vinfo;
	if (!XMatchVisualInfo(d, DefaultScreen(d), 32, TrueColor, &vinfo)) {
		printf("No visual found supporting 32 bit color, terminating\n");
		return 1;
	}

	attrs.colormap = XCreateColormap(d, root, vinfo.visual, AllocNone);
	attrs.background_pixel = 0;
	attrs.border_pixel = 0;

	overlay = XCreateWindow(
	    d, root,
	    s->width/2, 0, 200, 200, 0,
	    vinfo.depth, InputOutput, 
	    vinfo.visual,
	    CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs
	);

	XMapWindow(d, overlay);

	cairo_surface_t* surf = cairo_xlib_surface_create(d, overlay,
		vinfo.visual, 200, 200);
	cairo_t* cr = cairo_create(surf);

	draw(cr);
	XFlush(d);

	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	
	//for (i = 0; i < 10; i++) {
	if(XGrabKeyboard(d, root, true, GrabModeAsync, 
				GrabModeAsync, CurrentTime) == GrabSuccess) {
		//printf("Black window grabbed before loop\n");
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

	//getchar();
	cairo_destroy(cr);
	cairo_surface_destroy(surf);

	XUnmapWindow(d, overlay);
	XCloseDisplay(d);

	return 1;

}
