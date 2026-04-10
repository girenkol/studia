// symulacja kasy biletowej w kinie: został tylko jeden, ostatni bilet na wielką
// premierę, a dwóch klientów klika przycisk "kupuj" w tej samej milisekundzie

import java.util.concurrent.locks.ReentrantLock;

class kasa_biletowa{
    int liczba_biletow = 1;
    ReentrantLock zamekKasy = new ReentrantLock();

    
    public void kupuj(String nazwa_klienta) {
        zamekKasy.lock();

        try {
            if(liczba_biletow == 1) {
                try {
                    Thread.sleep(500);
                } catch (Exception e) {
                    System.out.println("Błąd systemu.");
                }
                System.out.println(nazwa_klienta + " kupil bilet!");
                liczba_biletow = 0;
            }
            else System.out.println("Brak miejsc dla nowego klienta.");
        } finally {
            zamekKasy.unlock();
        }
    }
}

class Klient implements Runnable {
    String nazwa_klienta;
    kasa_biletowa kasa;

    public Klient(String nazwa_klienta, kasa_biletowa wskazana_kasa) {
        this.nazwa_klienta = nazwa_klienta;
        this.kasa = wskazana_kasa;
    }

    @Override
    public void run() {
        kasa.kupuj(nazwa_klienta);
    }
}

public class cw6_zamki {
    public static void main(String[] args) {
        kasa_biletowa kasaPierwsza = new kasa_biletowa();

        Klient osoba1 = new Klient("klient A", kasaPierwsza);
        Klient osoba2 = new Klient("klient B", kasaPierwsza);

        Thread klientA = new Thread(osoba1);
        Thread klientB = new Thread(osoba2);

        klientA.start();
        klientB.start();
    }
}