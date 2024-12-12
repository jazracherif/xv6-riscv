#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0; // null terminate the string
  return buf;
}

void find(char* in_path, char* name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    if ((fd = open(in_path, O_RDONLY)) < 0){
        close(fd);
        return;
    }

    struct stat st;
    if ((fstat(fd, &st)) < 0){
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            char *new_file = fmtname(in_path);
            // printf("--compare %s and %s  %d %d\n", name, new_file, strlen(name), strlen(new_file) );
            if (strcmp(name, new_file) == 0) {
                printf("%s\n", in_path);
            } else {
                // printf("No Match");
            }
            break;
        case T_DIR:
            // a directory is a file with a sequence dirent structure in it
            if(strlen(in_path) + 1 + DIRSIZ + 1 > sizeof buf){
            //   fprintf(2, "find: path too long\n");
              break;
            }
            strcpy(buf, in_path);
            p = buf + strlen(buf); // point to end of the buff
            *p++ = '/';

            // for each entry in `de` construct a new path, in_path + de.name and recurse into it
            while ((read(fd, &de, sizeof(struct dirent))) == sizeof(de)){
                // read all the entries in the dictoionary, save them in a structures
                if (de.inum == 0){
                    continue;
                }
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..")==0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                // p will now point DIRSIZ-1 ahead
                p[DIRSIZ]= 0; // the new 
                // printf("--check directory %s\n", buf);
                find(buf, name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]){

    if (argc != 3){
        printf("usage: find [dir] [name] \n");
        exit(1);
    }

    char path[256];
    char file[DIRSIZ];
    memcpy(path, argv[1], strlen(argv[1]));
    memcpy(file, argv[2], strlen(argv[2]));

    printf("search for file: %s in dir: %s \n", file, path);

    find(path, file);

}