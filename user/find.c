#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* filename) {
	char buf[512], * p;
	int fd;
	struct dirent de;
	struct stat st;
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/';
	fd=open(path,0);
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0) {
            find(buf, filename);
        }
        else if (strcmp(filename, p) == 0)
            printf("%s\n", buf);
    }
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("error\n usage:find <path><filename>\n ");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
