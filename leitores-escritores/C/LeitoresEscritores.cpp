/*
Execução do programa
./programa tipoDePrioridade numLeitores numEscritores [novoCoeficiente1 novoCoeficiente2 ... novoCoeficienteN] (N é o numEscritores)
*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "cores.h"
#define PRIORIDADE_LEITORES 1
#define PRIORIDADE_ESCRITORES 2
#define SEM_PRIORIDADE 3

using namespace std;
pthread_mutex_t mxLeitores, mxEscritores, gravacao, portalDoAluno, leitura, listaDeEspera;
pthread_mutex_t mxPrint;
int contLeitores, contEscritores, contEscritoresEsperando, versaoHistorico;
float *historicoCoeficiente;


Color::Modifier green(Color::FG_GREEN);
Color::Modifier lmagenta(Color::FG_LMAGENTA);
Color::Modifier lblue(Color::FG_LBLUE);
Color::Modifier red(Color::FG_RED);
Color::Modifier lcyan(Color::FG_LCYAN);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier amarelo(Color::FG_AMARELO);


void consultarCoeficiente(int id) {
	int espera;
	espera = rand() % 3;
	pthread_mutex_lock(&mxPrint);
	cout << lcyan << "Leitor " << id << def << " consultando coeficiente...\n";
	pthread_mutex_unlock(&mxPrint);
	sleep(espera);
	pthread_mutex_lock(&mxPrint);
	cout << lcyan << "Leitor " << id << def << " retornou: " << historicoCoeficiente[versaoHistorico] << ".\n";
	pthread_mutex_unlock(&mxPrint);
}

void alterarCoeficiente(int id) {
	int espera;
	espera = rand() % 5;
	cout << lmagenta << "Escritor " << id << def << " alterando o coeficiente...\n";
	sleep(espera);
	versaoHistorico++;
	cout << lmagenta << "Escritor " << id << def << " alterou o coeficiente, novo valor: " << historicoCoeficiente[versaoHistorico] << ".\n";
}

//Prioridade dos Leitores
void *leitorA(void *i) {
	/*int id= *((int *) i);

	// Iniciando mutex de controle
	pthread_mutex_lock(&mxLeitores);
	if(contLeitores==0)  {
		pthread_mutex_lock(&mxPrint);
		cout << lcyan << "Leitor " << id << def << " solicita acesso ao coeficiente." << red << " (bloqueado)\n" << def;
		pthread_mutex_lock(&mxPrint);

		pthread_mutex_lock(&portalDoAluno);
		cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << ". Liberado o acesso ao coeficiente para outros leitores.\n";

	} else {
		cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << ". Coeficiente está liberado para consulta.\n";
	}
	contLeitores++;
	// Liberando mutex de controle
	pthread_mutex_unlock(&mxLeitores);

	consultarCoeficiente(id);

	// Iniciando mutex de controle
	pthread_mutex_lock(&mxLeitores);
	if(contLeitores==1) {
		cout << lcyan << "Leitor " << id << def << " liberando acesso para escritores.\n";
		pthread_mutex_unlock(&portalDoAluno);
	}
	contLeitores--;

	// Liberando mutex de controle
	pthread_mutex_unlock(&mxLeitores);*/
	int id= *((int *) i);

	pthread_mutex_lock(&mxLeitores);
	cout << lcyan << "Leitor " << id << def << " solicita acesso ao coeficiente.\n";
	if(contLeitores==0)  {
		pthread_mutex_lock(&gravacao);
		cout << lcyan << "Leitor " << id << def << red << " (bloqueado)\n" << def;
		pthread_mutex_lock(&portalDoAluno);
		cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << ". Liberado o acesso ao coeficiente para outros leitores.\n";
	}
	else {
		cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << ". Coeficiente está liberado para consulta.\n";
	}
	contLeitores++;
	pthread_mutex_unlock(&mxLeitores);

	consultarCoeficiente(id);

	pthread_mutex_lock(&mxLeitores);
	if(contLeitores==1) {
		cout << lcyan << "Leitor " << id << def << " liberando acesso para escritores.\n";
		pthread_mutex_unlock(&portalDoAluno);
	//	pthread_mutex_unlock(&gravacao);
	}
	contLeitores--;

	pthread_mutex_unlock(&mxLeitores);
}

	void *escritorA(void *i) {
		int id= *((int *) i);

	//	pthread_mutex_lock(&gravacao);

		pthread_mutex_lock(&mxPrint);
		cout << lmagenta << "Escritor " << id << def << " solicita o acesso ao coeficiente." << red << "(bloqueado)\n" << def;
		pthread_mutex_unlock(&mxPrint);

		pthread_mutex_lock(&portalDoAluno);
		cout << lmagenta << "Escritor " << id << def << green << " (liberado)" << def << ".\n";
		alterarCoeficiente(id);

		cout << lmagenta << "Escritor " << id << def << " liberando acesso ao coeficiente.\n";
		pthread_mutex_unlock(&portalDoAluno);
	//	pthread_mutex_unlock(&gravacao);
	}

//Prioridade dos Escritores

	void *leitorB(void *i) {
		int id = *((int *) i);
		pthread_mutex_lock(&leitura);
		pthread_mutex_lock(&mxLeitores);
		cout << lcyan << "Leitor " << id << def << " solicita o acesso ao coeficiente.\n";
		if(contLeitores==0)  {
			cout << lcyan << "Leitor " << id << def << red << " (bloqueado)\n" << def;
			pthread_mutex_lock(&portalDoAluno);
			cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << ".\n";
		}
		contLeitores++;
		pthread_mutex_unlock(&mxLeitores);
		pthread_mutex_unlock(&leitura);

		consultarCoeficiente(id);

		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==1) {
			cout << lcyan << "Leitor " << id << def << green << " liberando acesso ao coeficiente." << def << "\n";
			pthread_mutex_unlock(&portalDoAluno);
		}
		contLeitores--;

		pthread_mutex_unlock(&mxLeitores);
	}

	void *escritorB(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&mxEscritores);
		if(contEscritores==0) {
			cout << lmagenta << "Escritor " << id << def << red << " bloqueando acesso para leitores.\n" << def;
			pthread_mutex_lock(&leitura);
		}
		contEscritores++;
		pthread_mutex_unlock(&mxEscritores);

		cout << lmagenta << "Escritor " << id << def << " solicita acesso ao coeficiente." << red << "(bloqueado)\n" << def;
		pthread_mutex_lock(&portalDoAluno);
		alterarCoeficiente(id);
		cout << lmagenta << "Escritor " << id << def << green << " (liberado)\n" << def;
		pthread_mutex_unlock(&portalDoAluno);

		pthread_mutex_lock(&mxEscritores);
		if(contEscritores==1) {
			cout << lmagenta << "Escritor " << id << def << green << " liberando acesso para leitores.\n" << def;
			pthread_mutex_unlock(&leitura);
		}
		contEscritores--;
		pthread_mutex_unlock(&mxEscritores);
	}


//Sem Prioridade
	void *leitorC(void *i) {
		int id = *((int *) i);
		pthread_mutex_lock(&listaDeEspera);
		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==0) {
			pthread_mutex_lock(&portalDoAluno);
		}
		contLeitores--;
		pthread_mutex_unlock(&listaDeEspera);
		pthread_mutex_unlock(&mxLeitores);


		consultarCoeficiente(id);

		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==1) {
			pthread_mutex_unlock(&portalDoAluno);
		}
		contLeitores--;
		pthread_mutex_unlock(&mxLeitores);
	}

	void *escritorC(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&listaDeEspera);
		pthread_mutex_lock(&portalDoAluno);

		alterarCoeficiente(id);

		pthread_mutex_unlock(&portalDoAluno);
		pthread_mutex_unlock(&listaDeEspera);
	}


/////MAIN FUNCTION///////////////////////////////
int main(int argc, char* argv[]) {
	int i, *num;
	int  qtdLeitores, qtdEscritores, prioridade;
	pthread_t *leitores,*escritores;

	cout << amarelo << "***********************************************************************\n" << def;
	cout << amarelo << "****************************" << def << lblue << "PORTAL DO ALUNO" << def << amarelo << "****************************\n" << def;
	cout << amarelo << "***********************************************************************\n\n" << def;

	cout << "Número de professores(Escritores):";
	cin >> qtdEscritores;
	cout << "Número de consultas ao coeficiente(Leitores):";
	cin >> qtdLeitores;
	cout << "Prioridade (1- Leitores, 2-Escritores, 3- Sem prioridade): ";
	cin >> prioridade;

	leitores = (pthread_t*) malloc(sizeof(pthread_t)*qtdLeitores);
	escritores = (pthread_t*) malloc(sizeof(pthread_t)*qtdEscritores);
	contEscritores=contLeitores=contEscritoresEsperando=0;
	versaoHistorico=0;

	historicoCoeficiente= (float*) malloc(sizeof(float) * (qtdEscritores+1));

	historicoCoeficiente[0]=0;
	cout << "Insira as aterações a serem feitas:\n";
	for(i = 1;i < qtdEscritores+1;i++) {
		cout << "Alteração " << i << "\n";
		cin >> historicoCoeficiente[i] ;
	}


	//inicializacao dos semaforos...
	pthread_mutex_init(&portalDoAluno, NULL);
	pthread_mutex_init(&mxLeitores, NULL);
	pthread_mutex_init(&mxEscritores, NULL);
	pthread_mutex_init(&gravacao, NULL);
	pthread_mutex_init(&leitura, NULL);
	pthread_mutex_init(&listaDeEspera, NULL);
	pthread_mutex_init(&mxPrint, NULL);

	switch(prioridade) {
		case PRIORIDADE_LEITORES:
			for(i=0;i < qtdEscritores-4;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				if(pthread_create(&escritores[i], NULL,escritorA, (void*)num)){
					cout << "Portal em Manutenção :/ \n";
				}
			}

			for(i=0;i < qtdLeitores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create(&leitores[i], NULL, leitorA, (void*) num);
			}

			for(i=qtdEscritores-4;i < qtdEscritores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				if(pthread_create(&escritores[i], NULL,escritorA, (void*)num)){
					cout << "Portal em Manutenção :/ \n";
				}
			}
			break;

		case PRIORIDADE_ESCRITORES:
			for(i=0;i<qtdEscritores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create( &escritores[i], NULL, escritorB, (void*)num);
			}
			for(i=0;i<qtdLeitores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create( &leitores[i], NULL, leitorB, (void*)num);
			}
			break;

		default:
			for(i=0;i<qtdEscritores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create( &escritores[i], NULL, escritorC, (void*)num);
			}

			for(i=0;i<qtdLeitores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create( &leitores[i], NULL, leitorC, (void*)num);
			}
			break;
	}


	for(i=0;i<qtdEscritores;i++){
		pthread_join(escritores[i], NULL);
	}

	for(i=0;i<qtdLeitores;i++){
		pthread_join(leitores[i], NULL);
	}
}
