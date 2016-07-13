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

sem_t semaforo;

queue<int> buffer;
bool barbeiros[N_BARBEIROS];
int barbeiros_ocupados=0;
int prox_cliente=0;

void *atender_cliente(void *arg) { // Tempo para o barbeiro atender ao cliente
    int n;
    int barbeiro = (intptr_t) arg;

    sem_wait(&semaforo);
    barbeiros[barbeiro] = false;
    barbeiros_ocupados += 1;
    sem_post(&semaforo);

    n = rand()%50;
    sleep(n);

    sem_wait(&semaforo);
    barbeiros[barbeiro] = true;
    barbeiros_ocupados -= 1;
    if(buffer.size() != 0) {
        prox_cliente = buffer.front();
        buffer.pop();
    }
    sem_post(&semaforo);
}

int achar_livre() { // Acha um barbeiro que possa atender
    int i;

    for (i = 0; i < N_BARBEIROS; i++) {
        if(barbeiros[i]==false) { //verifica se o barbeiro está disponivel
            return i;
        }
    }
    return -1;
}

int main() {
    pthread_t *threads;
    int num_barbeiros, i, n;
    int cont_clientes=1, livre;
    bool atender;

    threads = (pthread_t*) malloc(sizeof(pthread_t)*num_barbeiros);
    memset(barbeiros, 0, N_BARBEIROS);

    /* Inicializando o semáforo. O segundo argumento indica que será compartilhado entre as threads de um mesmo processo.
    O terceiro argumento especifica o valor inicial indicando que está livre.*/
    sem_init(&semaforo, 0, 1);

    cout << "O expediente está começando e o barbeiro vai arrumar a barbearia!"<< '\n';

    for (i = 0; i < 30; i++) {

        n = rand()%10; // sincronização
        sleep(n);
        cout << "Cliente " << i+1 << " entrando na barbearia." << '\n';

        if(buffer.size() != 0) {
            atender = false;
        } else {
            livre = achar_livre();
            if(livre < 0) {
                atender = false;
            } else {
                atender = true;
            }
        }

        if(atender) {
            if(pthread_create(&threads[livre], NULL, atender_cliente, (void*)livre)) {
                cout << "O barbeiro " << livre << " não pode atender.\n";
            } else {
                cout << "O barbeiro " << livre << " está atendendo o cliente " << i+1 << '.\n';
            }
        } else {
            cout << "Cliente " << i+1 << " está esperando.";
            buffer.push(cont_clientes);
        }

        cont_clientes++;
    }
}
