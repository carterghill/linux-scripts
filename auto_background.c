#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	FILE *ls = popen("ls ~/Pictures/Wallpapers/", "r");
	char buf[256];
	char files[64][64];
	int i = 0;
	int num;
	char command[256];

	while (fgets(buf, sizeof(buf), ls) != 0) {
		printf("buf: %s\n", buf);
		strcpy(files[i], buf);
		i++;
	}
	pclose(ls);
	num = i;

	for (i = 0; ; i++) {
		strcpy(command, "feh --bg-scale ~/Pictures/Wallpapers/");
		strcat(command, files[i]);
		printf("Changing to %i. %s\n", i, files[i]);
		system(command);
		sleep(2);
		if (i >= num-1) {
			printf("%i >= %i\n", i, num);
			i = -1;
		}
	}

	return 1;

}
