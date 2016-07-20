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
pthread_mutex_t mxLeitores, mxEscritores,portalDoAluno,leitura, listaDeEspera;
pthread_mutex_t mxPrint;
int   contLeitores, contEscritores, contEscritoresEsperando, versaoHistorico;
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
	cout << lcyan << "Leitor " << id << def << " consultando coeficiente...\n";
	sleep(espera);
	pthread_mutex_lock(&mxPrint);
	cout << lcyan << "Leitor " << id << def << " retornou: " << historicoCoeficiente[versaoHistorico] << ".\n";
	pthread_mutex_unlock(&mxPrint);
}

void alterarCoeficiente(int id) {
	int espera;
	espera = rand() % 10;
	cout << lmagenta << "Escritor " << id << def << " alterando o coeficiente...\n";
	sleep(espera);
	versaoHistorico++;
	cout << lmagenta << "Escritor " << id << def << " alterou o coeficiente, novo valor: " << historicoCoeficiente[versaoHistorico] << ".\n";
}

//Prioridade dos Leitores
void *leitorA(void *i) {
	int id= *((int *) i);

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
	pthread_mutex_unlock(&mxLeitores);
}

	void *escritorA(void *i) {
		int id= *((int *) i);

		pthread_mutex_lock(&mxPrint);
		cout << lmagenta << "Escritor " << id << def << " solicita o acesso ao coeficiente." << red << "(bloqueado)\n" << def;
		pthread_mutex_unlock(&mxPrint);

		pthread_mutex_lock(&portalDoAluno);
		cout << lmagenta << "Escritor " << id << def << green << " (liberado)" << def << ".\n";
		alterarCoeficiente(id);

		cout << lmagenta << "Escritor " << id << def << " liberando acesso ao coeficiente.\n";
		pthread_mutex_unlock(&portalDoAluno);
	}

//Prioridade dos Escritores

	void *leitorB(void *i) {
		int id= *((int *) i);
//		pthread_mutex_lock(&leitura);

		// Iniciando mutex de controle
	//	pthread_mutex_lock(&mxLeitores);
		cout << lcyan << "Leitor " << id << def << " solicita o acesso ao coeficiente.\n";
		if(contEscritores > 0 || contEscritoresEsperando > 0)  {
			cout << lcyan << "Leitor " << id << def << red << " (bloqueado)\n" << def;
			pthread_mutex_lock(&leitura);
			//cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << ".\n";
		}
		contLeitores++;
		// Liberando mutex de controle
	//	pthread_mutex_unlock(&mxLeitores);
	//	pthread_mutex_unlock(&leitura);

		consultarCoeficiente(id);

		// Iniciando mutex de controle
//		pthread_mutex_lock(&mxLeitores);
		contLeitores--;
		if(contLeitores==0) {
			cout << lcyan << "Leitor " << id << def << green << " (liberado)" << def << "\n";
			pthread_mutex_unlock(&leitura);
		}

		// Liberando mutex de controle
		//pthread_mutex_unlock(&mxLeitores);
	}

	void *escritorB(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&mxEscritores);

		contEscritoresEsperando++;
		if(contLeitores > 0 || contEscritores > 0) {
			cout << lmagenta << "Escritor " << id << def << " solicita acesso ao coeficiente." << red << "(bloqueado)\n" << def;
			pthread_mutex_lock(&leitura);
			cout << lmagenta << "Escritor " << id << def << green << " (liberado)\n" << def;
		}
		contEscritoresEsperando--;
		contEscritores++;
		pthread_mutex_unlock(&mxEscritores);

		pthread_mutex_lock(&portalDoAluno);
		alterarCoeficiente(id);
		pthread_mutex_unlock(&portalDoAluno);

		//pthread_mutex_lock(&mxEscritores);
		/*if(contEscritores==1) {
			cout << lmagenta << "Escritor " << id << def << green << " liberando sessão crítica.\n" << def;

		}*/
		contEscritores--;
		pthread_mutex_unlock(&leitura);
		//pthread_mutex_unlock(&mxEscritores);
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
	pthread_mutex_init(&leitura, NULL);
	pthread_mutex_init(&listaDeEspera, NULL);
	pthread_mutex_init(&mxPrint, NULL);

	switch(prioridade) {
		case PRIORIDADE_LEITORES:
			for(i=0;i < qtdEscritores;i++){
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
			break;

		case PRIORIDADE_ESCRITORES:
			for(i=0;i<qtdLeitores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create( &leitores[i], NULL, leitorB, (void*)num);
			}

			for(i=0;i<qtdEscritores;i++){
				num = (int*) malloc(sizeof(int));
				*num = i+1;
				pthread_create( &escritores[i], NULL, escritorB, (void*)num);
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
