#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


static void recursive_print_(int pid, int level)
{
	int x;

	//Strings for location of the children folder, 
	//            location of the cmdline file (contains name of process)
	//            name of the process
	char str[4200];
	char filename[4200];
	char name[4200];
	char pdir[4200];

	sprintf(pdir, "/proc/%d/task", pid);
	DIR *d;
	struct dirent *dir;

	d = opendir(pdir);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				sprintf(str, "/proc/%d/task/%s/children", pid, dir->d_name);
				FILE *children = fopen(str, "r");

				sprintf(filename, "/proc/%d/cmdline", pid);
				FILE *namepointer = fopen(filename, "r");

				fscanf(namepointer, "%s", name);
				fclose(namepointer);

				if (pid != atoi(dir->d_name))
					printf("   ");

				for (int i = 0; i < level; ++i)
					printf(i == level-1 ? "|--" : "   ");
				printf("%s %s\n", dir->d_name, name);

				while (fscanf(children, "%d", &x) != -1)
					recursive_print_(x, level+1);

				fclose(children);
			}
		}
		closedir(d);
	}

#if 0
	//Assigning location of the children folder for this process to str
	sprintf(str, "/proc/%d/task/%d/children", pid, pid);
	FILE *children = fopen(str, "r");

	//Assigning location of the 'name' file for this process ID
	sprintf(filename, "/proc/%d/cmdline", pid);
	FILE *namepointer = fopen(filename, "r");

	//Assigning the name of the file to the string 'name'
	fscanf(namepointer, "%s", name);
	fclose(namepointer);

	//Indentation
	for (int i = 0; i < level; ++i)
		printf(i == level-1 ? "|--" : "   ");
	printf("%d %s\n", pid, name);


	//Recursively print the children
	while (fscanf(children, "%d", &x) != -1) 
	{
		recursive_print_(x, level+1);
	}

	fclose(children);
#endif
}


void recursive_print(int pid)
{
	recursive_print_(1, 0);
}

int main()
{
	recursive_print(1);
}
