package teste;

class LE {
	public static final int PRIORIDADE_LEITORES = 1;
	public static final int PRIORIDADE_ESCRITORES =2;
	public static final int SEM_PRIORIDADES = 3;
	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_BLACK = "\u001B[30m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";
	public static final String ANSI_PURPLE = "\u001B[35m";
	public static final String ANSI_CYAN = "\u001B[36m";
	public static final String ANSI_WHITE = "\u001B[37m";

	private int numLeitores, numEscritores,prioridade, numLeitoresEsperando, numEscritoresEsperando, listaEspera;
	// Construtor
	LE(int prioridade) {
		this.numLeitores = 0;
		this.numEscritores = 0;
		this.prioridade=prioridade;
		this.numLeitoresEsperando = 0;
		this.numEscritoresEsperando = 0;
		this.listaEspera = 0;
	}

	//prioridade para leitores

	public synchronized void EntraLeitor(int id){
		try {
			switch (prioridade) {
			case PRIORIDADE_LEITORES:

				System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + " solicitando acesso");
				while (this.numEscritores > 0) {
					System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + ANSI_RED+" bloqueado" + ANSI_RESET);
					wait(); // leitura bloqueada
				}
				System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + " entrou na SC");
				this.numLeitores++;

				break;

			case PRIORIDADE_ESCRITORES:

				System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + " solicitando acesso");
				while (this.numEscritores > 0 || this.numEscritoresEsperando>0) {
					System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + ANSI_RED+" bloqueado" + ANSI_RESET);
					wait(); // leitura bloqueada
				}
				System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + " entrou na SC");
				this.numLeitores++;

				break;

			case SEM_PRIORIDADES:
				System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + " solicitando acesso");
				while(this.listaEspera!=0){
					wait();
				}
				this.listaEspera++;
				while ( this.numEscritores > 0) {
					System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET + ANSI_RED+" bloqueado" + ANSI_RESET);
					wait(); // leitura bloqueada
				}
				System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET+" entrou na SC");
				this.listaEspera--;
				this.numLeitores++;

				break;
			default:
				break;
			}
		} catch (InterruptedException e) { }
	}
	public synchronized void SaiLeitor(int id) {
		this.numLeitores--;
		if (this.numLeitores == 0){
			System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET+" saindo da SC");
			notifyAll(); //libera a SC
		}
	}

	public synchronized void EntraEscritor (int id) {
		try {
			switch (prioridade) {
			case PRIORIDADE_LEITORES:

				System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+" solicitando acesso");
				while ((this.numLeitores > 0) || (this.numEscritores > 0)) {
					System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+ ANSI_RED +" bloqueado"+ANSI_RESET);
					wait();
				}
				System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+" entrou na SC");
				this.numEscritores++;

				break;

			case PRIORIDADE_ESCRITORES:

				System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+" solicitando acesso");
				this.numEscritoresEsperando++;
				while ((this.numLeitores > 0) || (this.numEscritores > 0)) {
					System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+ ANSI_RED +" bloqueado"+ANSI_RESET);
					wait();
				}
				this.numEscritoresEsperando--;
				System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+" entrou na SC");
				this.numEscritores++;

				break;

			case SEM_PRIORIDADES:
				System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+" solicitando acesso");
				while(this.listaEspera!=0){
					wait();
				}
				this.listaEspera++;
				while ((this.numLeitores > 0) || (this.numEscritores > 0)) {
					System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+ANSI_RED +" bloqueado"+ANSI_RESET);
					wait();
				}

				System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+" entrou na SC");
				this.numEscritores++;

				break;
			}
		} catch (InterruptedException e) { }
	}

	public synchronized void SaiEscritor (int id) {
		System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET + " saindo da SC");

		if(prioridade == SEM_PRIORIDADES ){
			this.listaEspera--;
		}

		this.numEscritores--;
		notifyAll(); //libera leitores


	}
}
