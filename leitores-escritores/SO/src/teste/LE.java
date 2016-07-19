package teste;

class LE {
	public static final int PRIORIDADE_LEITORES = 1;
	public static final int PRIORIDADE_ESCRITORES =2;
	public static final int SEM_PRIORIDADES = 3;
	
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
				
				System.out.println("Leitor: " + id + " solicitando acesso");
				while (this.numEscritores > 0) {
					System.out.println("Leitor: " + id + " bloqueado");
					wait(); // leitura bloqueada
				}
				System.out.println("Leitor: " + id + " entrou na SC");
				this.numLeitores++;
				
				break;

			case PRIORIDADE_ESCRITORES:
				
				System.out.println("Leitor: " + id + " solicitando acesso");
				while (this.numEscritores > 0 || this.numEscritoresEsperando>0) {
					System.out.println("Leitor: " + id + " bloqueado");	
					wait(); // leitura bloqueada
				}
				System.out.println("Leitor: " + id + " entrou na SC");
				this.numLeitores++;
				
				break;
				
			case SEM_PRIORIDADES:
				System.out.println("Leitor: "+ id + " solicitando acesso");
				while(this.listaEspera!=0){
					wait();
				}
				this.listaEspera++;
				while ( this.numEscritores > 0) {
					System.out.println("Leitor: " + id + " bloqueado");	
					wait(); // leitura bloqueada
				}
				System.out.println("Leitor: " + id + " entrou na SC");
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
				System.out.println("Leitor: " + id + " saindo da SC");
				notifyAll(); //libera a SC
			}		
	}

	public synchronized void EntraEscritor (int id) {
		try {
			switch (prioridade) {
			case PRIORIDADE_LEITORES:
			
				System.out.println("Escritor: "+ id + " solicitando acesso");
				while ((this.numLeitores > 0) || (this.numEscritores > 0)) {
					System.out.println("Escritor: " + id + " bloqueado");
					wait(); 
				}
				System.out.println("Escritor: " + id + " entrou na SC");
				this.numEscritores++;
				
				break;
				
			case PRIORIDADE_ESCRITORES:
				
				System.out.println("Escritor: "+ id + " solicitando acesso");
				this.numEscritoresEsperando++;
				while ((this.numLeitores > 0) || (this.numEscritores > 0)) {
					System.out.println("Escritor: " + id + " bloqueado");
					wait(); 
				}
				this.numEscritoresEsperando--;
				System.out.println("Escritor: " + id + " entrou na SC");
				this.numEscritores++;
				
				break;
			
			case SEM_PRIORIDADES:
				System.out.println("Escritor: "+ id + " solicitando acesso");
				while(this.listaEspera!=0){
					wait();
				}
				this.listaEspera++;
				while ((this.numLeitores > 0) || (this.numEscritores > 0)) {
					System.out.println("Escritor: " + id + " bloqueado");
					wait(); 
				}
				
				System.out.println("Escritor: " + id + " entrou na SC");
				this.numEscritores++;
				
				break;
			}
		} catch (InterruptedException e) { }
	}
	
	public synchronized void SaiEscritor (int id) {
		System.out.println("Escritor: " + id + " saindo da SC");
		
		if(prioridade == SEM_PRIORIDADES ){
			this.listaEspera--;
		}
		
		this.numEscritores--;
		notifyAll(); //libera leitores
		
		
	}
}