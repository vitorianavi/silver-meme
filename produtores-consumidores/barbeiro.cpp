#include <iostream>
#include <queue>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 20
#define N_BARBEIROS 4

using namespace std;

queue<int> buffer;
bool barbeiros[N_BARBEIROS];

void *atender_cliente(void *args) {
    int n;

    n = rand()%50;
    sleep(n);
}

int achar_livre() {
    int i;

    for (i = 0; i < N_BARBEIROS; i++) {
        if(barbeiros[i]==false) {
            return i;
        }
    }
    return -1;
}

int main() {
    pthread_t *threads;
    int num_barbeiros, i;
    int cont_clientes=1, livre;

    threads = (pthread_t*) malloc(sizeof(pthread_t)*num_barbeiros);
    memset(barbeiros, 0, N_BARBEIROS);

    for (i = 0; i < 30; i++) {
        n = rand()%10;
        sleep(n);
        cout << "Cliente " << cont_barbeiros << " entrando."

        livre = achar_livre();
        if(livre<0) {
            buffer.push_back(cont_clientes);
        } else {
            if(pthread_create(&threads[livre], NULL, atender_cliente, (void*)livre)) {
                cout << "Barbeiro " << cont_barbeiros+1 << "não pode atender";
            } else{
                printf("Disparando Thread %ld\n", k);
            }
        }

        cont_clientes++;
    }
}
