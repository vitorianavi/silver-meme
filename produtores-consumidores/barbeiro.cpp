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

void *atender_cliente(void *args) { // Tempo para o barbeiro atender ao cliente
    int n;

    n = rand()%50;
    sleep(n);
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
    int num_barbeiros, i;
    int n;
    int cont_clientes=1, livre;

    threads = (pthread_t*) malloc(sizeof(pthread_t)*num_barbeiros);
    memset(barbeiros, 0, N_BARBEIROS);

    cout << "O expediente está começando e o barbeiro vai arrumar a barbearia!"<< '\n';

    for (i = 0; i < 30; i++) {

        n = rand()%10; // sincronização
        sleep(n);
        cout << "Cliente " << cont_clientes << " entrando na barbearia." << '\n';

        livre = achar_livre();
        if(livre < 0) {
            cout << "Cliente" << cont_clientes << "está esperando";
            buffer.push(cont_clientes);
        } else {
            if(pthread_create(&threads[livre], NULL, atender_cliente, (void*)livre)) {
                cout << "Barbeiro " << livre << "não pode atender";
            } else{
                cout << "O Barbeiro " << livre << " está atendendo o clinte " << cont_clientes << '\n';
                barbeiros[livre] = true;
                atender_cliente();
                cout << "Corte t0p" <<'\n';
                barbeiros[livre] = false;
            }
        }

        cont_clientes++;
    }
}
