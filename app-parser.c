#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct App {

	char * name;
	
}App;

App * ParseApp () {

	App * app = malloc(sizeof(struct App));
	app->name = malloc(strlen("Hello!")+1);
	app->name = "Hello!";
	return app;

}

int main () {

	App * app = ParseApp();
	printf("Name: %s\n", app->name);
	return 1;

}
