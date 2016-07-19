/*
Execução do programa
./programa tipoDePrioridade numLeitores numEscritores [novoCoeficiente1 novoCoeficiente2 ... novoCoeficienteN] (N é o numEscritores)
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define PRIORIDADE_LEITORES 1
#define PRIORIDADE_ESCRITORES 2
#define SEM_PRIORIDADE 3
    
pthread_mutex_t mxLeitores, mxEscritores,portalDoAluno,leitura, listaDeEspera;      
int   contLeitores, contEscritores, versaoHistorico;
float *historicoCoeficiente;


void consultarCoeficiente() {
	int espera;
	espera = rand() % 3;
	printf("O seu coeficiente e %.2f", historicoCoeficiente[versaoHistorico]);
}

void alterarCoeficiente() {
	int espera;
	espera = rand() % 10;
	versaoHistorico++;
	printf("Coeficiente atualizado para %.2f",historicoCoeficiente[versaoHistorico]);
}

//Prioridade dos Leitores

	void leitorA(void *i) {
		int id= *((int *) i);
		
		printf("Leitor %d esta esperando para alterar o numero ");
		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==0)  {
			pthread_mutex_lock(&portalDoAluno);   
		}
		contLeitores++;
		pthread_mutex_unlock(&mxLeitores);        
		
		consultarCoeficiente();
		
		pthread_mutex_lock(&mxLeitores);                                         
		if(contLeitores==1) {
			pthread_mutex_unlock(&portalDoAluno);
		}
		contLeitores--;
		
		pthread_mutex_unlock(&mxLeitores);          
	}
	
	void escritorA(void *i) { 
		int id= *((int *) i);	
		pthread_mutex_lock(&portalDoAluno);    
		alterarCoeficiente();          
		pthread_mutex_unlock(&portalDoAluno);     
	}

//Prioridade dos Escritores

	void leitorB(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&leitura);
		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==0)  {
			pthread_mutex_lock(&portalDoAluno);   
		}
		contLeitores++;
		pthread_mutex_unlock(&mxLeitores);        
		pthread_mutex_unlock(&leitura);
		
		consultarCoeficiente();
					
		pthread_mutex_lock(&mxLeitores);                                         
		if(contLeitores==1) {
			pthread_mutex_unlock(&portalDoAluno);
		}
		contLeitores--;
		
		pthread_mutex_unlock(&mxLeitores); 
	}
	
	void escritorB(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&mxEscritores);
		if(contEscritores==0) {
			pthread_mutex_lock(&leitura);
		}
		contEscritores++;
		pthread_mutex_unlock(&mxEscritores);
		
		pthread_mutex_lock(&portalDoAluno);
		alterarCoeficiente();
		pthread_mutex_unlock(&portalDoAluno);
		
		pthread_mutex_lock(&mxEscritores);
		if(contEscritores==1) {
			pthread_mutex_unlock(&leitura);
		}
		contEscritores--;
	}
	

//Sem Prioridade
	void leitorC(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&listaDeEspera);
		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==0) {
			pthread_mutex_lock(&portalDoAluno);
		}
		contLeitores--;
		pthread_mutex_unlock(&listaDeEspera);
		pthread_mutex_unlock(&mxLeitores);
		
		
		consultarCoeficiente();
		
		pthread_mutex_lock(&mxLeitores);
		if(contLeitores==1) {
			pthread_mutex_unlock(&portalDoAluno);
		}
		contLeitores--;
		pthread_mutex_unlock(&mxLeitores);
	}

	void escritorC(void *i) {
		int id= *((int *) i);
		pthread_mutex_lock(&listaDeEspera);
		pthread_mutex_lock(&portalDoAluno);
		
		alterarCoeficiente();
		
		pthread_mutex_unlock(&portalDoAluno);
		pthread_mutex_unlock(&listaDeEspera);
	}
	

/////MAIN FUNCTION///////////////////////////////
int main(int argc, char* argv[]) {
	if(argc<4) {
		printf("Voce precisa escrever no minimo tres argumentos, da seguinte maneira:\n./programa tipoDePrioridade numLeitores numEscritores [novoCoeficiente1 novoCoeficiente2 ... nocoCoeficienteN] (N e o numEscritores)\nPrioridade: 1 - Leitores 2-Escritores 3- Sem Prioridade\n");
		exit(1);
	}
	
	int i, qtdLeitores=atoi(argv[2]), qtdEscritores=atoi(argv[3]), prioridade=atoi(argv[1]);
	pthread_t leitores[qtdLeitores],escritores[qtdEscritores];	
	
	contEscritores=contLeitores=0;
	versaoHistorico=0;
	
	historicoCoeficiente= (float*) malloc(sizeof(float) * (qtdEscritores+1));
	
	historicoCoeficiente[0]=0;
	for(i=1;i<qtdEscritores+1;i++) {
		historicoCoeficiente[i]=atof(argv[4+i]);
	}
	

	//inicializacao dos semaforos...
	pthread_mutex_init(&portalDoAluno, NULL);
	pthread_mutex_init(&mxLeitores, NULL);
	pthread_mutex_init(&mxEscritores, NULL);
	pthread_mutex_init(&leitura, NULL);
	pthread_mutex_init(&listaDeEspera, NULL);

	switch(prioridade) {
		case PRIORIDADE_LEITORES:
			for(i=0;i<qtdEscritores;i++){
				pthread_create( &escritores[i], NULL,(void *) escritorA, (void*)i);
			}

			for(i=0;i<qtdLeitores;i++){
				pthread_create( &leitores[i], NULL,(void *) leitorA, (void*)i+qtdEscritores);
			}
			break;
		
		case PRIORIDADE_ESCRITORES:
			for(i=0;i<qtdEscritores;i++){
				pthread_create( &escritores[i], NULL,(void *) escritorB, (void*)i);
			}

			for(i=0;i<qtdLeitores;i++){
				pthread_create( &leitores[i], NULL,(void *) leitorB, (void*)i+qtdEscritores);
			}			
			break;
		default: 
			for(i=0;i<qtdEscritores;i++){
				pthread_create( &escritores[i], NULL,(void *) escritorC, (void*)i);
			}

			for(i=0;i<qtdLeitores;i++){
				pthread_create( &leitores[i], NULL,(void *) leitorC, (void*)i+qtdEscritores);
			}				
			break;
	}


	for(i=0;i<qtdEscritores;i++){
		pthread_join(escritores[i], NULL);
	}

	for(i=0;i<qtdLeitores;i++){
		pthread_join(leitores[i], NULL);
	}
	
	exit(0);
}
