#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char *argv[]) {

	int num;
	char buf[64];
	char strNum[64];
	char cmd[256];

	FILE *ls;

	/* Check for argument, output and exit if it's not there */
	if (argc <= 1) {
		printf("Usage: brightness [number]\n");
		return 1;
	}

	/* Get brightness level from file */
	ls = popen("cat /sys/class/backlight/intel_backlight/brightness", "r");
	while (fgets(buf, sizeof(buf), ls) != 0) {}
	pclose(ls);
	
	/* Turn the brightness into an integer */
	num = atoi(buf);

	/* Add the program argument as an integer */
	num = num + atoi(argv[1]);

	/* Store the number as a string */
	sprintf(strNum, "%i", num);

	/* Set up the command string echo strNum > file*/
	strcpy(cmd, "echo ");
	strcat(cmd, strNum);
	strcat(cmd, " > /sys/class/backlight/intel_backlight/brightness");
	system(cmd);

	return 1;

}
