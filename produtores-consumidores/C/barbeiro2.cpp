/*
Autores: Ana Vitória Vitoriano, Maria Clara, Mário Damhur, Paulo Rodrigo Martins
Fontes:
https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
http://lia.deis.unibo.it/Courses/RTOS/assignments/2013_05_08/barber/barber.c
*/
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "colormod.h"

#define BUFFER_SIZE 5

using namespace std;
// colorir saida
Color::Modifier green(Color::FG_GREEN);
Color::Modifier lmagenta(Color::FG_LMAGENTA);
Color::Modifier lblue(Color::FG_LBLUE);
Color::Modifier red(Color::FG_RED);
Color::Modifier lcyan(Color::FG_LCYAN);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier amarelo(Color::FG_AMARELO);

sem_t sem_barbeiro; // semáforo para controlar as threads dos barbeiros (quais estão dormindo ou trabalhando)
sem_t sem_cliente; // semáforo para controlar as threads de clientes (quais estão sendo atendidos ou esperando para serem atendidos)
sem_t mutex; // mutex para controlar a escrita no buffer
sem_t print_mutex; // mutex para evitar sobreposição nas impressões

queue<int> sala_espera; // fila para receber os clientes

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

    //identificador do barbeiro
    barbeiro = (intptr_t) arg;

    while(true) {
        cout << green << "Barbeiro " << barbeiro << " está dormindo zZzZz " << def << '\n';
        // Barbeiro esperando pelo cliente (liberação do semáforo de cliente)
        sem_wait(&sem_cliente);
        // Existe cliente no buffer para ser atendido! Espera a vez para tirar o primeiro cliente da fila.
        sem_wait(&mutex);
        atual = sala_espera.front();
        cout << "\nCliente " << atual << " acordando o barbeiro " << barbeiro << "!\n";
        sala_espera.pop();
        imprimir_fila(sala_espera);
        // Liberando mutex
        sem_post(&mutex);
        // Indica que o barbeiro está acordado/livre
        sem_post(&sem_barbeiro);

        // Fazendo serviço
        sem_wait(&print_mutex);
        cout << "\nBarbeiro " << barbeiro << " está cortando o cabelo do cliente " << atual << ".\n";
        sem_post(&print_mutex);
        n = rand()%10;
        sleep(n);
        sem_wait(&print_mutex);
        cout << "\nAgora, barbeiro " << barbeiro << " está fazendo a barba do cliente " << atual << ".\n";
        sem_post(&print_mutex);
        n = rand()%10;
        sleep(n);
        sem_wait(&print_mutex);
        cout << "\nBarbeiro " << barbeiro << " finalizou o serviço com o cliente " << atual << "!\n\n" << def;
        sem_post(&print_mutex);
        // Finalizou serviço
    }
}

void *cliente(void *arg) {
    int num;
    // Identificador do cliente
    int cliente = (intptr_t) arg;

    // Tempo para o cliente chegar na barbearia
    num = rand()%30;
    sleep(num);
    cout << lblue << "\nCliente " << cliente << " chegando no pedaço! \n" << def;

    // Cliente entra no salão e espera sua vez para alterar a fila de espera.
    sem_wait(&mutex);
    imprimir_fila(sala_espera);
    if(sala_espera.size() < BUFFER_SIZE) {
        // Se ainda houver cadeira disponível na sala de espera
        cout << "\nCliente " << cliente << " entrando na sala de espera.\n";
        sala_espera.push(cliente);

        // Indica que o cliente está disponível para ser atendido
        sem_post(&sem_cliente);
        // Libera o mutex de alteração da fila de espera
        sem_post(&mutex);
        // Espera pela disponibilidade do barbeiro
        sem_wait(&sem_barbeiro);
    } else {
        // Se o salão estiver lotado
        cout << red << "\nSalão cheio! Cliente " << cliente << " cabeludo indo embora tristinho :(.\n" << def;
        // Libera o mutex de alteração da fila de espera
        sem_post(&mutex);
        return NULL;
    }
}

int main() {
    pthread_t *threads_clientes, *threads_barbeiros;
    int num, n_clientes, n_barbeiros;
    long int i;

    cout << amarelo << "***********************************************************************\n" << def;
    cout << amarelo << "*************************" << def << lblue << "BARBEARIA TOP OF LINE" << def << amarelo << "*************************\n" << def;
    cout << amarelo << "***********************************************************************\n\n" << def;

    cout << "NÚMERO DE BARBEIROS: ";
    cin >> n_barbeiros;

    cout << "\nNÚMERO DE CLIENTES: ";
    cin >> n_clientes;

    threads_clientes = (pthread_t*) malloc(sizeof(pthread_t)*n_clientes);
    threads_barbeiros = (pthread_t*) malloc(sizeof(pthread_t)*n_barbeiros);

    /* Inicializando os semáforos. O segundo argumento indica que será compartilhado entre
    as threads de um mesmo processo. O terceiro argumento especifica o valor inicial.*/
    sem_init(&sem_barbeiro, 1, n_barbeiros); // Valor inicial: número de barbeiros, indica que todos estão disponíveis (dormindo)
    sem_init(&sem_cliente, 1, 0); // Valor inicial: 0, não há clientes para serem atendidos inicialmente
    sem_init(&mutex, 1, 1);
    sem_init(&print_mutex, 1, 1);

    cout << amarelo <<"***********************************************************************\n" << def ;
    if(n_barbeiros == 1){
        cout << "\nO expediente está começando e o barbeiro vai arrumar a barbearia!\n\n";
    }else{
      cout << "\nO expediente está começando e os barbeiros vão arrumar a barbearia!\n\n";
    }


    // criando as threads dos barbeiros
    for (i = 1; i <= n_barbeiros; i++){
        // pthread_create retorna 0 se bem-sucedida
        if(pthread_create(&threads_barbeiros[i-1], NULL, barbeiro, (void*)i)){
          cout << "O barbeiro está doente e não pode trabalhar.\n";
        }
    }

    // criando as threads dos clientes
    for (i = 1; i <= n_clientes; i++) {
        if(pthread_create(&threads_clientes[i-1], NULL, cliente, (void*)i)) {
            cout << "O cliente não está contente por algum motivo.\n";
        }
    }

    /* Esse parte é responsável por "amarrar" as threads de modo que uma espere pelo término
    da execução de todas as outras e impede que o main finalize a execução do programa.*/
    for (i = 0; i < n_clientes; i++) {
        pthread_join(threads_clientes[i],NULL);
    }
    for (i = 0; i < n_barbeiros; i++) {
        pthread_join(threads_barbeiros[i],NULL);
    }
}
