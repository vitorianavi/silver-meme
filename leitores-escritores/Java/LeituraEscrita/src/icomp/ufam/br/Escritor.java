package icomp.ufam.br;

import java.util.Random;

public class Escritor extends Thread {
	private Main disparador;
	private int id;
	
	public Escritor(Main disparador, int id) {
		this.disparador = disparador;
		this.id = id;
	}
	
	//Prioridade para leitores
	@Override
	public void run() {
		Random rand = new Random();
		solicitarAcesso();
		//altera cadastro
		System.out.println("Escritor "+id+" escrevendo");
		int n = rand.nextInt(40);
		try {
			Thread.sleep(n);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		liberarAcesso();
	}
	
	private synchronized void solicitarAcesso() {
		try {
			System.out.println("Escritor "+id+" solicitando acesso!");
			wait();
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	
	private synchronized void liberarAcesso() {
		notify();
	}
	
}
