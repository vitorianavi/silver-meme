package icomp.ufam.br;

import java.util.Random;

public class Leitor extends Thread {
	private Main disparador;
	private int id;
	
	Leitor(Main disparador, int id) {
		this.disparador = disparador;
		this.id = id;
	}
	
	//Prioridade para leitores
	@Override
	public void run() {
		Random rand = new Random();
		addLeitor();
		
		//le cadastro
		System.out.println("Leitor "+id+" lendo");
		int n = rand.nextInt(40);
		try {
			Thread.sleep(n);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		
		removeLeitor();
	}
	
	public synchronized void addLeitor() {
		disparador.numLeitores += 1;
		//System.out.println(disparador.numLeitores);
		if(disparador.numLeitores == 1) {
			try {
				System.out.println("Leitor "+id+" bloqueando sessão crítica para escritores e liberando para outros leitores.");
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public synchronized void removeLeitor() {
		System.out.println(disparador.numLeitores);
		disparador.numLeitores -= 1;
		if(disparador.numLeitores == 0) {
			System.out.println("Leitor "+id+" liberando sessão crítica para escritores.");
			notify();
		}
	}
}
