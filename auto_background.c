#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <unistd.h> 

int main(int argc, char *argv[]) {
	
	FILE *ls;
	char buf[256];
	char files[64][64];
	int i = 0;
	int num;

	/* User must specify path and time interval */
	if (argc < 3) {
		printf("Usage: auto_background [path] [time]\n");
		return 1;
	}

	/* List the files in the folder */
	strcpy(buf, "ls ");
	strcat(buf, argv[1]);
	ls = popen(buf, "r");

	/* Loop through the list and store each image path as a string */
	while (fgets(buf, sizeof(buf), ls) != 0) {
		strcpy(files[i], buf);
		i++;
	}
	pclose(ls);
	
	/* Loop forever and switch to a new background then sleep */
	num = i;
	for (i = rand()%num; ; i++) {
		strcpy(buf, "feh --bg-scale ");
		strcat(buf, argv[1]);
		strcat(buf, "/");
		strcat(buf, files[i]);
		system(buf);
		sleep(atoi(argv[2]));
		if (i >= num) {
			i = -1;
		}
	}

	return 1;

}
