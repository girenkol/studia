class Bohater {
    public void skacz() {
        System.out.println("Bohater podskakuje wysoko!");
    }
}

class OdtwarzaczMuzyki implements Runnable {
    @Override
    public void run() {
        for(int i=0; i<2; i++){
            System.out.println("Leci muzyczka z gry...");
            try {
                Thread.sleep(1000); //pauza na 1 sekunde
            } catch (InterruptedException e) {
                System.out.println("Odtwarzacz zepsuty"); // obsluga bledu
            }
        }
    }
}

public class cw2_zarzadzanie_postacia {
    public static void main(String[] args) {
        Bohater mojBohater = new Bohater();
        mojBohater.skacz();

        OdtwarzaczMuzyki mojOdtwarzacz = new OdtwarzaczMuzyki();
        
        Thread watek = new Thread(mojOdtwarzacz);
        watek.start();
    }
}