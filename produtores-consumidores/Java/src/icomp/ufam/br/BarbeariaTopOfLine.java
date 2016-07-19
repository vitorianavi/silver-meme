package icomp.ufam.br;

import java.util.Scanner;

public class BarbeariaTopOfLine {
	public int numBarbeiros;
	public final SalaEspera salaEspera = new SalaEspera(5);
	
	public BarbeariaTopOfLine(int numBarbeiros) {
		this.numBarbeiros = numBarbeiros;
	}
	
	public static void main(String[] args) {
		Scanner reader = new Scanner(System.in);  // Reading from System.in
		System.out.println("Número de barbeiros: ");
		int numBarbeiros = reader.nextInt();
		System.out.println("Número de clientes: ");
		int numClientes = reader.nextInt();
		reader.close();
		
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
