package icomp.ufam.br;

import java.util.LinkedList;

class SalaEspera {
	private LinkedList<Integer> sala;
	private int tamanho;

	SalaEspera(int tamanho) {
		sala = new LinkedList<Integer>();
		this.tamanho = tamanho;
	}

	public synchronized boolean entrar(int idCliente) {
		if(sala.size() < tamanho) {
			System.out.println("Cliente "+idCliente+" entrando na sala de espera.");
			sala.push(idCliente);
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
				System.out.println("Barbeiro "+idBarbeiro+" dormindo zZzZz");
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
		System.out.println("------SALA DE ESPERA------");
		if(sala.size()>0) {
			System.out.print("|");
			for(Integer cliente : sala) {
				System.out.print(cliente+"|");
			}
			System.out.println();
		} else {
			System.out.println("VAZIA");
		}
	}
}
