package icomp.ufam.br;

import java.util.Scanner;


public class BarbeariaTopOfLine {
	public int numBarbeiros;
	public final SalaEspera salaEspera = new SalaEspera(5);
	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_BLACK = "\u001B[30m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";
	public static final String ANSI_PURPLE = "\u001B[35m";
	public static final String ANSI_CYAN = "\u001B[36m";
	public static final String ANSI_WHITE = "\u001B[37m";
	
	public BarbeariaTopOfLine(int numBarbeiros) {
		this.numBarbeiros = numBarbeiros;
	}
	
	public static void main(String[] args) {
		 System.out.println(ANSI_YELLOW +"***********************************************************************" + ANSI_RESET);
	    System.out.println(ANSI_YELLOW +"*************************"+ANSI_RESET + ANSI_CYAN+"BARBEARIA TOP OF LINE"+ANSI_RESET+ANSI_YELLOW+"*************************"+ANSI_RESET);
	    System.out.println(ANSI_YELLOW +"***********************************************************************" + ANSI_RESET);

		Scanner reader = new Scanner(System.in);  // Reading from System.in
		System.out.println("Número de barbeiros: ");
		int numBarbeiros = reader.nextInt();
		System.out.println("Número de clientes: ");
		int numClientes = reader.nextInt();
		reader.close();
		System.out.println(ANSI_YELLOW + "***********************************************************************"+ ANSI_RESET);
	    if(numBarbeiros == 1){
	    	System.out.println("O expediente está começando e o barbeiro vai arrumar a barbearia!");
	    }else{
	    	System.out.println("O expediente está começando e os barbeiros vão arrumar a barbearia!");
	    }

		Barbeiro[] barbeiros = new Barbeiro[numBarbeiros];
		Cliente[] clientes = new Cliente[numClientes];
		
		BarbeariaTopOfLine barbearia = new BarbeariaTopOfLine(numBarbeiros);
		for (int i = 0; i < numBarbeiros; i++) {
			barbeiros[i] = new Barbeiro(barbearia, i+1);
			barbeiros[i].start();
		}
		for (int i = 0; i < numClientes; i++) {
			clientes[i] = new Cliente(barbearia, i+1);
			clientes[i].start();
		}
	}
	
	public int getCliente(int idBarbeiro) {
		int cliente = salaEspera.get(idBarbeiro);
		
		return cliente;
	}
	
	public void putCliente(int cliente) {
		salaEspera.entrar(cliente);
	}
}
