package icomp.ufam.br;

import java.util.LinkedList;

class SalaEspera {
	private LinkedList<Integer> sala;
	private int tamanho;
	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_MAGENTA = "\u001B[35m";

	SalaEspera(int tamanho) {
		sala = new LinkedList<Integer>();
		this.tamanho = tamanho;
	}

	public synchronized boolean entrar(int idCliente) {
		if(sala.size() < tamanho) {
			System.out.println("Cliente "+idCliente+" entrando na sala de espera.");
			sala.addLast(idCliente);
			imprimir();
			notify();
			
			return true;
		}
		
		System.out.println("Salão cheio! Cliente cabeludo "+idCliente+" indo embora tristinho :(!");

		return false;
	}

	public synchronized int get(int idBarbeiro) {
		if(sala.size() == 0) {
			try {
				System.out.println(ANSI_GREEN + "Barbeiro "+idBarbeiro+" dormindo zZzZz" + ANSI_RESET);
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		int cliente = sala.getFirst();
		sala.removeFirst();
		System.out.println("Cliente "+cliente+" acordando barbeiro "+idBarbeiro+"!");
		imprimir();
		
		return cliente;
	}
	
	public synchronized void imprimir() {
		System.out.println(ANSI_YELLOW + "------SALA DE ESPERA------"+ANSI_RESET);
		if(sala.size()>0) {
			System.out.print(ANSI_MAGENTA + "|" + ANSI_RESET);
			for(Integer cliente : sala) {
				System.out.print(ANSI_MAGENTA+cliente+"|" + ANSI_RESET);
			}
			System.out.println();
		} else {
			System.out.println("VAZIA");
		}
	}
}
