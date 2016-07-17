
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
sem_t sem_cliente;
sem_t mutex;

Color::Modifier green(Color::FG_GREEN);
Color::Modifier lmagenta(Color::FG_LMAGENTA);
Color::Modifier lblue(Color::FG_LBLUE);
Color::Modifier azul(Color::FG_AZUL);
Color::Modifier lcyan(Color::FG_LCYAN);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier amarelo(Color::FG_AMARELO);
queue<int> sala_espera;
bool finalizado=false; // para indicar quando fechar a thread do barbeiro
//int atual; // indica qual cliente está sendo atendido atualmente

void imprimir_fila(queue<int> fila) {
    cout << amarelo <<"---------------------SALA DE ESPERA-----------------------\n" << def;
    if(fila.size()) {
        cout << lmagenta <<"|" << def;
        while(!fila.empty()) {
            cout << lmagenta << fila.front() << "|" << def ;
            fila.pop();
        }
        cout << "\n";
    } else {
        cout << "VAZIA\n\n";
    }
}

void *barbeiro(void *arg) { // Consumidor (barbeiro)
    int n, barbeiro, atual;
    //pra quando tiver mais de um barbeiro
    barbeiro = (intptr_t) arg;

  while(1){
        cout << green << "Barbeiro " << barbeiro << " está dormindo zZzZz " << def << '\n';
        sem_wait(&sem_cliente);
        sem_wait(&mutex);
        atual = sala_espera.front();
        cout << "\nCliente " << atual << " acordando o barbeiro " << barbeiro << "!\n";
        sala_espera.pop();
        imprimir_fila(sala_espera);
        sem_post(&sem_barbeiro);
        sem_post(&mutex);

        // Fazendo serviço
        cout << "\nBarbeiro " << barbeiro << " está cortando o cabelo do cliente " << atual << ".\n";
        n = rand()%10;
        sleep(n);
        cout << "\nAgora, barbeiro " << barbeiro << " está fazendo a barba do cliente " << atual << ".\n";
        n = rand()%10;
        sleep(n);
        cout << "\nBarbeiro " << barbeiro << " finalizou o serviço com o cliente " << atual << "!\n\n" << def;
    }

    //cout << "Barbeiro finalizou o expediente!\n- Que dia de cão! - pensou, ao trancar a porta.\n";
}

void *cliente(void *arg) {
    int num;
    int cliente = (intptr_t) arg;

    num = rand()%30;
    sleep(num);
    cout << lblue << "\nCliente " << cliente << " chegando no pedaço! \n" << def;

    sem_wait(&mutex);
    imprimir_fila(sala_espera);
    if(sala_espera.size() < BUFFER_SIZE) {
        cout << "\nCliente " << cliente << " entrando na sala de espera.\n";
        sala_espera.push(cliente);
        //imprimir_fila(sala_espera);
        sem_post(&sem_cliente);
        sem_post(&mutex);
        sem_wait(&sem_barbeiro);
    } else {
        cout << "\nSalão cheio! Cliente " << cliente << " cabeludo indo embora tristinho :(.\n";
        sem_post(&mutex);
        return NULL;
    }
}

int main() {
    pthread_t *threads_clientes, *threads_barbeiros;
    int num, n_clientes=10, n_barbeiros=2;
    long int i;

    //cout<< lcyan <<"------------------------- NÚMERO DE BARBEIROS -------------------------" << def <<'\n';
    //cin >> n_barbeiros;

    threads_clientes = (pthread_t*) malloc(sizeof(pthread_t)*n_clientes);
    threads_barbeiros = (pthread_t*) malloc(sizeof(pthread_t)*n_barbeiros);
    //threads_barbeiros = (pthread_t*) malloc(sizeof(pthread_t)*N_BARBEIROS);

    /* Inicializando os semáforos. O segundo argumento indica que será compartilhado entre
    as threads de um mesmo processo. O terceiro argumento especifica o valor inicial.*/
    sem_init(&sem_barbeiro, 1, n_barbeiros);
    sem_init(&sem_cliente, 1, 0);
    sem_init(&mutex, 1, 1);

    cout << lmagenta <<"*****************************************************************\n" << def ;
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
/*    for (i = 0; i < n_clientes; i++) {
        pthread_join(threads_barbeiros[i],NULL);
    }*/

    for (i = 0; i < n_clientes; i++) {
        pthread_join(threads_clientes[i],NULL);
    }

    finalizado = true;
    // Mata o barbeiro quando todos os clientes forem atendidos
    //sem_post(&sem_barbeiro);  // Acordar o barbeiro para o abate.
    //pthread_join(thread_barbeiro, NULL);
    for (i = 0; i < n_barbeiros; i++) {
        pthread_join(threads_barbeiros[i],NULL);
    }

    cout << "RIP barbeiro.\n";
}
