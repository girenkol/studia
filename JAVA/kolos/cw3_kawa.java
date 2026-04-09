class Kelner {
    public void przyjmijZamowienie() {
        System.out.println("Dzien dobry, zamowienie przyjete!");
    }
}

class EkspresDoKawy implements Runnable {
    @Override
    public void run() {
        for(int i=0; i<3; i++) {
            System.out.println("Parzenie, kawy, etap...");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                System.out.println("Awaria");
            }
        }
    }
}

public class cw3_kawa {
    public static void main(String[] args){
        Kelner Grzesiu = new Kelner();
        Grzesiu.przyjmijZamowienie();

        EkspresDoKawy ekspresGrzesia = new EkspresDoKawy();
        Thread watek = new Thread(ekspresGrzesia);
        watek.start();

        System.out.println("Kelner podchodzi do nastepnego stolika");
    }
}