#include <iostream>
#include <queue>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "colormod.h"

#define BUFFER_SIZE 5
#define N_BARBEIROS 4

using namespace std;

sem_t sem_barbeiro; // semáforo para indicar quando o barbeiro está dormindo. Inicialmente 0 (acordado).
sem_t sem_cliente; // semáforo para indicar que o cliente está sendo atendido pelo barbeiro. Inicialmente 0 (livre).
sem_t sem_sala_espera; // semáforo que simula/controla o buffer (fila) de clientes na sala de espera.
sem_t mutex; // mutex para garantir que somente um cliente está sendo atendido por vez.
sem_t print_mutex; // mutex auxiliar para impressão
sem_t mutex_t0p; // vamo que vamo

Color::Modifier green(Color::FG_GREEN);
Color::Modifier lmagenta(Color::FG_LMAGENTA);
Color::Modifier lblue(Color::FG_LBLUE);
Color::Modifier lcyan(Color::FG_LCYAN);
Color::Modifier def(Color::FG_DEFAULT);

queue<int> sala_espera;
bool finalizado=false; // para indicar quando fechar a thread do barbeiro
int atual; // indica qual cliente está sendo atendido atualmente

void *barbeiro(void *arg) { // Consumidor (barbeiro)
    int n, barbeiro;
    //pra quando tiver mais de um barbeiro
    barbeiro = (intptr_t) arg;

    while(!finalizado) {
        cout << green << "Barbeiro " << barbeiro << " está dormindo zZzZz " << def << '\n';
        sem_wait(&sem_barbeiro);


        cout << "\nBarbeiro " << barbeiro << " está cortando o cabelo do cliente " << atual << ".\n";
        n = rand()%10;
        sleep(n);
        cout << "\nAgora, barbeiro " << barbeiro << " está fazendo a barba do cliente " << atual << ".\n";
        n = rand()%10;
        sleep(n);
        cout << "\nServiço finalizado com o cliente " << atual << "!\n\n";

        sem_post(&sem_cliente);
    }

    //cout << "Barbeiro finalizou o expediente!\n- Que dia de cão! - pensou, ao trancar a porta.\n";
}

void imprimir_fila(queue<int> fila) {
    cout << "---------------------SALA DE ESPERA-----------------------\n";
    if(fila.size()) {
        cout << lmagenta <<"|"<<def;
        while(!fila.empty()) {
            cout << lmagenta << fila.front() << "|" << def ;
            fila.pop();
        }
        cout << "\n";
    } else {
        cout << "VAZIA\n\n";
    }
}

void *cliente(void *arg) {
    int num;
    int cliente = (intptr_t) arg;

    num = rand()%30;
    sleep(num);
    cout << "\nCliente " << cliente << " chegando no pedaço!\n";
    sem_wait(&print_mutex);
    imprimir_fila(sala_espera);
    sem_post(&print_mutex);

    if(sala_espera.size() == BUFFER_SIZE) {
        cout << "\nSalão cheio! Cliente " << cliente << " cabeludo indo embora tristinho :(.\n";
        return NULL;
    }

    sem_wait(&sem_sala_espera);
    cout << "Cliente " << cliente << " esperando por sua vez...\n";
    sala_espera.push(cliente);

    // Esperando cadeira ser desocupada.
    sem_wait(&mutex);
    // Acabou espera na sala.
    atual = sala_espera.front();
    sala_espera.pop();
    sem_post(&sem_sala_espera);

    cout << "Cliente " << cliente << " acordando o barbeiro!\n";
    imprimir_fila(sala_espera);
    sem_post(&sem_barbeiro);

    // Esperando o barbeiro finalizar o corte
    sem_wait(&sem_cliente);
    // Liberando cadeira
    sem_post(&mutex);
}

int main() {
    pthread_t *threads_clientes, *threads_barbeiros;
    int num, n_clientes=10, n_barbeiros;
    long int i;

    cout<< lcyan <<"------------------------- NÚMERO DE BARBEIROS -------------------------" << def <<'\n';
    cin >> n_barbeiros;

    threads_clientes = (pthread_t*) malloc(sizeof(pthread_t)*n_clientes);
    threads_barbeiros = (pthread_t*) malloc(sizeof(pthread_t)*n_barbeiros);
    //threads_barbeiros = (pthread_t*) malloc(sizeof(pthread_t)*N_BARBEIROS);

    /* Inicializando os semáforos. O segundo argumento indica que será compartilhado entre
    as threads de um mesmo processo. O terceiro argumento especifica o valor inicial.*/
    sem_init(&sem_barbeiro, 1, 0);
    sem_init(&sem_cliente, 1, 0);
    sem_init(&sem_sala_espera, 1, BUFFER_SIZE);
    sem_init(&mutex, 1, 1);
    sem_init(&print_mutex, 1, 1);
    sem_init(&mutex_t0p, 1, 1);

    cout << "*****************************************************************\n";
    cout << "O expediente está começando e o barbeiro vai arrumar a barbearia!\n\n";
    // criando as threads dos barbeiros
    for (i = 1; i <= n_barbeiros; i++){
      if(pthread_create(&threads_barbeiros[i-1], NULL, barbeiro, (void*)i)){
        cout << "O barbeiro está doente e não pode trabalhar.\n";
      }
    }
    for (i = 1; i <= n_clientes; i++) {
        // pthread_create retorna 0 se bem-sucedida
        if(pthread_create(&threads_clientes[i-1], NULL, cliente, (void*)i)) {
            cout << "O cliente não está contente por algum motivo.\n";
        }
    }

    /* Esse parte é responsável por "amarrar" as threads de modo que uma espere pelo término
    da execução de todas as outras.*/
    for (i = 0; i < n_clientes; i++) {
        pthread_join(threads_barbeiros[i],NULL);
    }

    for (i = 0; i < n_clientes; i++) {
        pthread_join(threads_clientes[i],NULL);
    }

    // Mata o barbeiro quando todos os clientes forem atendidos
    finalizado = true;
    //sem_post(&sem_barbeiro);  // Acordar o barbeiro para o abate.
    //pthread_join(thread_barbeiro, NULL);
    for (i = 0; i < n_clientes; i++) {
        pthread_join(threads_barbeiros[i],NULL);
    }

    cout << "RIP barbeiro.\n";
}
