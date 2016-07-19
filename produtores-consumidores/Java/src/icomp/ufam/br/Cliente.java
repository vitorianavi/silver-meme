package icomp.ufam.br;

import java.util.Random;

public class Cliente extends Thread {
	private final BarbeariaTopOfLine barbearia;
	private int id;
	
	Cliente(BarbeariaTopOfLine barbearia, int cliente) {
		this.barbearia = barbearia;
		this.id = cliente;
	}
	
	@Override
	public void run() {
		try {
			Random rand = new Random();
			int n = rand.nextInt(200);
			Thread.sleep(n);
			System.out.println("Cliente "+id+" chegando no pedaço!");
			
			barbearia.putCliente(id);
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
		}
	}
}
