package icomp.ufam.br;

public class Main {
	public int numLeitores=0;
	public int numEscritores=0;
	
	
	public static void main(String[] args) {
		Main disparador = new Main();
		
		Leitor[] leitores = new Leitor[3];
		Escritor[] escritores = new Escritor[3];
		
		for (int i = 0; i < 1; i++) {
			leitores[i] = new Leitor(disparador, i+1);
			leitores[i].start();
			escritores[i] = new Escritor(disparador, i+1);
			escritores[i].start();
		}
		
		for (int i = 1; i < 3; i++) {
			leitores[i] = new Leitor(disparador, i+1);
			leitores[i].start();
			escritores[i] = new Escritor(disparador, i+1);
			escritores[i].start();
		}
	}
}
