#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("Не удалось блокировать сигнал SIGINT");
        return 1;
    }

    while (1) {
        sleep(3);
    }

    return 0;
}
