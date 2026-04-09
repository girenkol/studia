class Spioch implements Runnable {
    @Override
    public void run() {
        try {
            Thread.sleep(10000); // usnij na 10000 ms
            System.out.println("Udanego snu!"); // komunikat udanego snu
            
        } catch (InterruptedException e) {
            System.out.println("Sen zostal przerwany"); // komunikat przerwanego snu
        }
    }
}

public class cw4_budzik {
    public static void main(String[] args) {
        Spioch mojSpioch = new Spioch();
        Thread watekSpiocha = new Thread(mojSpioch);
        watekSpiocha.start();

        try {
            Thread.sleep(2000);// watek glowny czeka 2000 ms
        } catch (InterruptedException e) {
            System.out.println("Blad systemu");
        }

        System.out.println("Budzik dzwoni!");
        watekSpiocha.interrupt();// tutaj zrob wybudzenie: watekSpiocha.interrupt();
    }
}