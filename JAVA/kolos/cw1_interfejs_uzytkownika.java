class Ekran {
    public void rysuj() {
        System.out.println("Rysuje interfejs uzytkownika");
    }
}

class ZadaniePobierania implements Runnable {
    @Override
    public void run() {
        for(int i=0; i<4; i++){
            System.out.println("Pobieranie pliku: czesc " + i);
        }
    }
}

public class cw1_interfejs_uzytkownika {
    public static void main(String[] args) {
        //# utworz ekran i wywolaj rysuj
        Ekran mojEkran = new Ekran();
        mojEkran.rysuj();

        //# przygotuj zadanie pobierania
        ZadaniePobierania pobieranie = new ZadaniePobierania();

        //# utworz i wystartuj watek
        Thread watek = new Thread(pobieranie);
        watek.start();

        System.out.println("Watek glowny konczy dzialanie");
    }
}