package teste;

import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

class LeitorEscritor {
	public static final int PRIORIDADE_LEITORES = 1;
	public static final int PRIORIDADE_ESCRITORES =2;
	public static final int SEM_PRIORIDADES = 3;
	//static final int L = 6;
	//static final int E = 2;
	private Float coeficienteAtual;
	ArrayList<Float> listaDeEscrita;

	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_BLACK = "\u001B[30m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";
	public static final String ANSI_PURPLE = "\u001B[35m";
	public static final String ANSI_CYAN = "\u001B[36m";
	public static final String ANSI_WHITE = "\u001B[37m";



	public LeitorEscritor(int prioridade,int numLeitores, int numEscritores, ArrayList<Float> lista) {
		int i;
		LE monitor = new LE(prioridade); // monitor
		Leitor[] l = new Leitor[numLeitores]; // leitores
		Escritor[] e = new Escritor[numEscritores]; // escritores

		this.coeficienteAtual=0f;
		this.listaDeEscrita = lista;

		listaDeEscrita.add(10f);
		listaDeEscrita.add(8f);
		//	listaDeEscrita.add(5f);

		for (i=0; i<numLeitores; i++) {
			l[i] = new Leitor(monitor, i+1); l[i].start();
		}

		for (i=0; i<numEscritores; i++) {
			e[i] = new Escritor(monitor, i+1); e[i].start();
		}


	}


	class Escritor extends Thread {
		//objeto monitor para coordenar a l�gica de execu��o
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

			System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET+"alterando coeficiente...");
			try {
				Thread.sleep(n*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			System.out.println(ANSI_PURPLE +"Escritor "+id+":"+ANSI_RESET + valorAntigo+" para "+coeficienteAtual+".");
		}


	}

	class Leitor extends Thread {
		//objeto monitor para coordenar a l�ogica de execu�c~ao
		LE monitor;
		private int id;

		Leitor (LE m, int idEscritor) {
			this.monitor = m;
			this.id = idEscritor;
		}

		public void run () {
			/*Random rand = new Random();
			int n = rand.nextInt(20);

			try {
				Thread.sleep(n*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}*/

			this.monitor.EntraLeitor(id);
			consultarCoeficiente();
			this.monitor.SaiLeitor(id);
		}

		public synchronized void consultarCoeficiente() {
			Random rand = new Random();
			int n = rand.nextInt(20);

			System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET +" lendo coeficiente...");
			try {
				Thread.sleep(n*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			System.out.println(ANSI_CYAN+"Leitor "+ id +":" +ANSI_RESET+coeficienteAtual);

		}
	}


	public static void main (String[] args) {
		System.out.println(ANSI_YELLOW +"***********************************************************************" + ANSI_RESET);
		System.out.println(ANSI_YELLOW +"****************************"+ANSI_RESET + ANSI_CYAN+"PORTAL DO ALUNO"+ANSI_RESET+ANSI_YELLOW+"****************************"+ANSI_RESET);
		System.out.println(ANSI_YELLOW +"***********************************************************************" + ANSI_RESET);
		Scanner reader = new Scanner(System.in);  // Reading from System.in
		System.out.println("Número de professores(Escritores): ");
		int numEscritores = reader.nextInt();
		System.out.println("Número de consultas ao coeficiente(Leitores):");
		int numLeitores = reader.nextInt();
		System.out.println("Prioridades (1-Leitores, 2-Escritores, 3-Sem prioridade): ");
		int prioridades  = reader.nextInt();
		ArrayList<Float> listaDeEscrita = new ArrayList<>();
		System.out.println("Alterações dos coeficientes(" + ANSI_RED + "Números de alerações deve ser igual ao número de escritores"+ANSI_RESET +")");
		if(reader.hasNextFloat()) {
			Float d = reader.nextFloat();
			listaDeEscrita.add(d);
		}
		reader.close();

		LeitorEscritor rw = new LeitorEscritor(prioridades,numEscritores,numLeitores, listaDeEscrita);

	}
}
