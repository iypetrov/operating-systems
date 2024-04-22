#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {
    uid_t real = getuid();
    dprintf(1, "%d\n", real);

    uid_t effective = geteuid();
    dprintf(1, "%d\n", effective);
  
    return 0;
}

