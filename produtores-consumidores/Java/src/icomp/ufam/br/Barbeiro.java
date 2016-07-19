package icomp.ufam.br;

import java.util.Random;

public class Barbeiro extends Thread {
	private int id;
	private final BarbeariaTopOfLine barbearia;
	
	Barbeiro(BarbeariaTopOfLine barbearia, int id) {
		this.barbearia = barbearia;
		this.id = id;
	}

	@Override
	public void run() {
		while(true) {
			try {
				Random rand = new Random();
				// O método getCliente() pega um cliente da sala de espera. Se não houver clientes, o barbeiro dorme.
				int cliente = barbearia.getCliente(id);
				int n;
				
				System.out.println("Barbeiro "+id+" cortando o cabelo do cliente "+cliente+".");
				n = rand.nextInt(200);
				Thread.sleep(n);
				
				System.out.println("Barbeiro "+id+" fazendo a barba do cliente "+cliente+".");
				n = rand.nextInt(200);
				Thread.sleep(n);
				
				System.out.println("Barbeiro "+id+" finalizou o serviço com o cliente "+cliente+".");
				System.out.println("Cliente "+cliente+" diz: \"Uau! Como estou beiçudinho!\".");
				
			} catch (InterruptedException e) {
				Thread.currentThread().interrupt();
				break;
			}
		}
	}
}
