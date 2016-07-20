package teste;

import java.util.ArrayList;
import java.util.Random;

class LeitorEscritor {
	public static final int PRIORIDADE_LEITORES = 1;
	public static final int PRIORIDADE_ESCRITORES =2;
	public static final int SEM_PRIORIDADES = 3;
	static final int L = 5;
	static final int E = 3;
	private Float coeficienteAtual;
	ArrayList<Float> listaDeEscrita;
	

	
	
	public LeitorEscritor() {
		int i;
		LE monitor = new LE(SEM_PRIORIDADES); // monitor
		Leitor[] l = new Leitor[L]; // leitores
		Escritor[] e = new Escritor[E]; // escritores

		this.coeficienteAtual=0f;
		this.listaDeEscrita = new ArrayList<>();
		
		listaDeEscrita.add(10f);
		listaDeEscrita.add(8f);
		listaDeEscrita.add(5f);
		
		for (i=0; i<L; i++) {
			l[i] = new Leitor(monitor, i+1); l[i].start();
		}
		for (i=0; i<E; i++) {
			e[i] = new Escritor(monitor, i+1); e[i].start();
		}
		
	}


	class Escritor extends Thread {
		//objeto monitor para coordenar a lógica de execução
		LE monitor;
		private int id;
		
		Escritor (LE m, int idEscritor) {
			this.monitor = m;
			this.id = idEscritor;
		}
		
		public void run () {
			this.monitor.EntraEscritor(id);
			alterarCoeficiente();
			this.monitor.SaiEscritor(id);
		}
		
		public void alterarCoeficiente() {
			Random rand = new Random();
			int n = rand.nextInt(20);
			
			Float valorAntigo = coeficienteAtual;
			coeficienteAtual = listaDeEscrita.get(0);
			listaDeEscrita.remove(0);
			
			System.out.println("Escritor "+id+" alterando coeficiente...");
			try {
				Thread.sleep(n*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			System.out.println("Escritor "+id+" alterou o valor de "+valorAntigo+" para "+coeficienteAtual+".");
		}


	}

	class Leitor extends Thread {
		//objeto monitor para coordenar a l´ogica de execu¸c~ao
		LE monitor;
		private int id;

		Leitor (LE m, int idEscritor) {
			this.monitor = m;
			this.id = idEscritor;
		}
		
		public void run () {
			Random rand = new Random();
			int n = rand.nextInt(20);

			try {
				Thread.sleep(n*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			this.monitor.EntraLeitor(id);
			consultarCoeficiente();
			this.monitor.SaiLeitor(id);
		}
		
		public synchronized void consultarCoeficiente() {
			Random rand = new Random();
			int n = rand.nextInt(20);
			
			System.out.println("Leitor "+id+" lendo coeficiente...");
			try {
				Thread.sleep(n*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			System.out.println("Leitor "+id+": "+coeficienteAtual);

		}
	}
	

	public static void main (String[] args) {
		LeitorEscritor rw = new LeitorEscritor();
	}
}
