#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char *argv[]) {

	/* Check for argument, output and exit if it's not there */
	if (argc <= 1) {
		printf("Usage: brightness [number]\n");
		return 1;
	}

	/* Check if file exists, create it if not */
	/*if( access(".brightness", F_OK ) != -1 ) {
		//file exists
	} else {
		//file doesn't exist
		printf("not there\n");
		system("echo 5 > .brightness");
	}*/

	/* Get brightness level from file */
	FILE *ls = popen("cat /sys/class/backlight/intel_backlight/brightness",
		       	"r");
	char buf[64];
	int num;

	while (fgets(buf, sizeof(buf), ls) != 0) {
		printf("buf: %s\n", buf);
		//strcpy(files[i], buf);
	}
	pclose(ls);
	num = atoi(buf);
	printf("cur nrightness: %i\n", num);

	char strNum[80] = {};
	num = num + atoi(argv[1]);
	sprintf(strNum, "%i", num);
	printf("new brightness: %s\n", strNum);

	/* Set new brightness level with xrandr */
	char cmd[256];
	strcpy(cmd, "echo ");
	strcat(cmd, strNum);
	strcat(cmd, " > /sys/class/backlight/intel_backlight/brightness");
	system(cmd);
	
	/* update current brightness */
	/*strcpy(cmd, "echo ");
	strcat(cmd, strNum);
	strcat(cmd, " > .brightness");
	system(cmd);*/

	return 1;

}