#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "app-parser.h"

/* Loop through the line's characters and figure out it's type and value */
void ParseLine(App * app, char * line) {
	
	int i = 0;
	int len = strlen(line);
	char type[16];
	char value[64];
	
	for (i = 0; i < len; i++) {
		
		if (line[i] == '=') {
			strcpy(value, &line[i+1]);
			type[i] = '\0';
			value[strcspn(value, "\n")]=0;
			value[strcspn(value, "%")]=0;
			break;	
		}

		type[i] = line[i];

	}

	if (strcmp(type, "Name")==0 && app->name == NULL) {
		app->name = malloc(strlen(value)+1);
		strcpy(app->name, value);
		printf("Name: %s\n", app->name);
		return;
	}

	if (strcmp(type, "Comment")==0 && strcmp(app->comment, "No description")==0) {
                app->comment = malloc(strlen(value)+1);
                strcpy(app->comment, value);
		return;
        }

	if (strcmp(type, "Exec")==0 && app->exec == NULL) {
                app->exec = malloc(strlen(value)+1);
                strcpy(app->exec, value);
		return;
        }

	if (strcmp(type, "Icon")==0 && app->icon == NULL) {
                app->icon = malloc(strlen(value)+1);
                strcpy(app->icon, value);
		return;
        }

	if (strcmp(type, "Categories")==0 &&  strcmp(app->tags, " ")==0) {
                app->tags = malloc(strlen(value)+1);
                strcpy(app->tags, value);
                return;
        }

}

App * ParseApp (char * path) {

	FILE * file = fopen(path, "r");
	char line[64];
	App * app = malloc(sizeof(App));

	app->comment = malloc(strlen("No description")+1);
	strcpy(app->comment, "No description");

	app->tags = malloc(strlen(" ")+1);
        strcpy(app->tags, " ");

	while (fgets(line, sizeof(line), file)) {
		ParseLine(app, line);
	}

	return app;

}
