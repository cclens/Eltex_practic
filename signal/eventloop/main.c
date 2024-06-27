#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

volatile sig_atomic_t signal_received = 0;

void sig_handler(int signo) {
    if (signo == SIGUSR1) {
        signal_received = 1;
    }
}

int main() {
    struct sigaction sa;
    sigset_t mask;
    int sig;

    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Невозможно установить обработчик сигнала");
        return 1;
    }

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (sigwait(&mask, &sig) != 0) {
        perror("Ошибка при ожидании сигнала");
        return 1;
    }

    printf("Получен сигнал SIGUSR1.\n");

    return 0;
}

