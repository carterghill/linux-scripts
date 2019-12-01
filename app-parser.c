#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct App {

	char * name;
	char * comment;
	char * exec;
	char * icon;
	
} App;

/* Loop through the line's characters and figure out it's type and value */
void ParseLine(App * app, char * line) {
	
	int i = 0;
	int len = strlen(line);
	char type[16];
	char value[64];
	printf("Line: %s", line);
	
	for (i = 0; i < len; i++) {
		
		if (line[i] == '=') {
			strcpy(value, &line[i+1]);
			type[i] = '\0';
			break;	
		}

		type[i] = line[i];

	}

	if (strcmp(type, "Name")==0) {
		app->name = malloc(strlen(value)+1);
		strcpy(app->name, value);
		return;
	}
	
	if (strcmp(type, "Comment")==0) {
                app->comment = malloc(strlen(value)+1);
                strcpy(app->comment, value);
		return;
        }

	if (strcmp(type, "Exec")==0) {
                app->exec = malloc(strlen(value)+1);
                strcpy(app->exec, value);
		return;
        }

	if (strcmp(type, "Icon")==0) {
                app->icon = malloc(strlen(value)+1);
                strcpy(app->icon, value);
		return;
        }

}

App * ParseApp (char * path) {

	printf("Path: %s\n", path);

	FILE * file = fopen(path, "r");
	char line[256];

	App * app = malloc(sizeof(struct App));

	while (fgets(line, sizeof(line), file)) {
		ParseLine(app, line);
	}

	return app;

}

int main () {

	App * app = ParseApp("/usr/share/applications/atom.desktop");
	printf("Name: %s", app->name);
	printf("Comment: %s", app->comment);
	printf("Exec: %s", app->exec);
	return 1;

}
