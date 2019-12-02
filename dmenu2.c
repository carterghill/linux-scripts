#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <ctype.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "app-parser.h"

typedef enum { false, true } bool;

App * selected;

struct {

	int width;
	int height;
	int radius;
	int font_size;
	int list_font_size;
	int num_apps;
	int index;
	int list_cursor;
	char * folder;
	App * apps[256];

} settings;

/* Draw a rectangle with rounded corners 
 * Reduced length of each line by the radius on each end 
 * Draw quarter circle arcs at each corner
 */
void rounded_rectangle(cairo_t *cr, int x, int y, int w, int h, int r) {

	/* Define angles for each corner */
	double angle1 = 270 * (3.14159 / 180);
	double angle2 = 0;
	double angle3 = 90 * (3.14159 / 180);
	double angle4 = 180 * (3.14159 / 180);

	/* Fill the rectangle */
	cairo_set_source_rgba(cr, 0.15, 0.15, 0.15, 1);
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
	cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 1);
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

bool is_in(char * str1, char * str2) {

	int i;
	int j;

	/* Loop through string string 2 */
	for (i = 0; str2[i] != '\0'; i++) {
		
		/* Loop through characters in str2 to see if match */
		for (j = 0; str1[j] != '\0'; j++) {
			
			if (str2[i+j] == '\0' || tolower(str1[j]) != tolower(str2[i+j]))
				break;

			if (str1[j+1] == '\0')
				return true;
		}

	}

	return false;

}

void draw_list(cairo_t *cr, int x, int y, int w, int h, char * input) {

	int i;
	cairo_text_extents_t extents;
	int font_size = settings.list_font_size;
	char output[64];
	int matches = 0;

	cairo_set_font_size(cr, font_size);

	/* Draw titles of matching applications */
        if (input[0] == '\0')
                return;

        for (i = 0; i < settings.num_apps; i++) {
                
		if (is_in(input, settings.apps[i]->name)) {

			strcpy(output, settings.apps[i]->name);
			cairo_text_extents(cr, settings.apps[i]->name, &extents);

			cairo_move_to(cr, w/2 - extents.width/2, y);

                        matches++;
                        if (settings.list_cursor == 0) {
                                settings.list_cursor++;
                        }

                        if (matches == settings.list_cursor) {
                                selected = settings.apps[i];
                                cairo_set_source_rgba(cr, 0.2, 0.2, 0.2, 1);
                                cairo_rectangle(cr, x+6, y-font_size, w-6, font_size+12);
                                cairo_fill(cr);
                        }


			/* Check if output is too big */
			while (extents.width > w) {
				strcpy(&output[strlen(output)-6], "....");
				output[strlen(output)-1] = '\0';
				cairo_text_extents(cr, output, &extents);
			}

                        cairo_move_to(cr, w/2 - extents.width/2, y);

			cairo_set_source_rgba(cr, 1, 1, 1, 1);
                        cairo_show_text(cr, output);
			y = y+font_size+6;
                }
        }

	if (matches == 0) {
		selected = NULL;
	}

}

void draw_text(cairo_t *cr, int x, int y, int w, int h, char * input) {

	cairo_text_extents_t extents;
	
	cairo_select_font_face(cr, "Courier",
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD
	);

	/* Draw input text */
	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_set_font_size(cr, settings.font_size);
	cairo_text_extents(cr, input, &extents);
	y = y+settings.font_size;
	cairo_move_to(cr, w/2 - extents.width/2, y);  
	cairo_show_text(cr, input);

	/* Draw titles of matching applications */
	y = y + settings.font_size;
	draw_list(cr, x, y, w/2, h, input);
}

void draw(cairo_t *cr, char * input) {

	int x = 0;
	int y = 0;
	int w = settings.width;
	int h = settings.height;
	int r = settings.radius;

	rounded_rectangle(cr, x, y, w, h, r);
	draw_text(cr, x, y, w, h, input);

}

void init_settings() {

	settings.width = 800;
	settings.height = 500;
	settings.radius = 24;
	settings.font_size = 60;
	settings.list_font_size = 28;
	settings.folder = "/usr/share/applications";
	settings.index = 0;
	settings.list_cursor = 0;

}

void load_files() {

	FILE *ls;
	char buf[64];
	char filepath[64];
	strcpy(buf, "ls ");
       	strcat(buf, settings.folder);
	ls = popen(buf, "r");
	int i = 0;
	
	/* Get all of the file names */
	while (fgets(buf, sizeof(buf), ls) != 0) {
		
		/* Skip it if it isn't a .desktop file */
		if (strstr(buf, ".desktop") == NULL)
			continue;

		strcpy(filepath, settings.folder);
		strcat(filepath, "/");
		strcat(filepath, buf);
		filepath[strcspn(filepath, "\n")]=0;
		settings.apps[i] = ParseApp(filepath);
		printf("Name: %s", settings.apps[i]->name);
		i++;

	}

	printf("%i files read\n", i);
	settings.num_apps = i;
	pclose(ls);	

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
	bool enter = false;

	//App * app = ParseApp("/usr/share/applications/Android Studio.desktop");
	//printf("Name: %s", app->name);

	init_settings();
	load_files();

	input[0] = '\0';

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

	cairo_save(cr);
	draw(cr, input);
	XFlush(d);
	cairo_save(cr);
	cairo_save(cr);

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
		if (e.xkey.type != KeyPress)
			continue;

		/* If number of characters read > 0 */
		if((numKeys = XLookupString(&e.xkey, buf, 8, &key, 0))) {

			/* Escape key pressed, exit */
			if ((int)buf[0] == 27)
				break;

			/* Delete key was pressed */
			if ((int)buf[0] == 8 && settings.index > 0) {
				settings.index--;
				input[settings.index] = '\0';
				cairo_restore(cr);
				cairo_save(cr);
				draw(cr, input);
				XFlush(d);
				continue;
			}

			/* Enter key was pressed */
			if ((int)buf[0] == 13) {
				printf("enter key\n");
				if (selected != NULL) {
					//system(selected->exec);
					enter = true;
				}
				//system(input);
				break;
			} 

			input[settings.index] = (char) buf[0];
			settings.index++;
			input[settings.index] = '\0';
			
			cairo_restore(cr);
			cairo_save(cr);
			//cairo_restore(cr);
			//cairo_surface_flush(surf);
			draw(cr, input);
			XFlush(d);
			cairo_surface_flush(surf);
			//redraw();
			//cairo_restore(cr);
			printf("%s\n", input);

		}
	

	}

	printf("input: %s\n", input);

	cairo_destroy(cr);
	cairo_surface_destroy(surf);

	XUnmapWindow(d, overlay);
	XCloseDisplay(d);

	if (enter)
		system(selected->exec);

	return 1;

}
