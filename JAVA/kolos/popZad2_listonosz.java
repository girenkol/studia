class MailBox {
    // Stan naszej skrzynki
    boolean czyJestList = false;

    // Metoda z 'synchronized', bo uzywamy wait/notify
    public synchronized void zostawList() {
        try {
            // 1. Jesli w skrzynce jest juz list, listonosz musi poczekac (uzyj wait)
            while (czyJestList == true) wait();
            
            // 2. Skrzynka jest pusta, wiec listonosz wrzuca list
            czyJestList = true;
            System.out.println("Listonosz: Wrzucilem list do skrzynki.");
            
            // 3. Budzimy adresata, zeby zszedl po list (uzyj notifyAll)
            notifyAll();
            
            // 4. Polecenie z kolokwium: "Listonosz natomiast oczekuje na potwierdzenie"
            System.out.println("Listonosz: Czekam na potwierdzenie odbioru...");
            // (uzyj wait, zeby listonosz tu stanal i poczekal na akcje adresata)
            wait();
            
            System.out.println("Listonosz: Dostalem potwierdzenie, wracam do bazy.");

        } catch (InterruptedException e) {
            System.out.println("Blad listonosza");
        }
    }

    public synchronized void odbierzList() {
        try {
            // 1. Jesli nie ma listu (czyJestList == false), adresat musi poczekac (uzyj wait)
            while (czyJestList == false) wait();
            
            // 2. List juz jest! Adresat go wyciaga, wiec skrzynka znow robi sie pusta
            czyJestList = false;
            System.out.println("Adresat: Odebralem list ze skrzynki!");
            
            // 3. Budzimy listonosza, ktory wciaz czeka na potwierdzenie (uzyj notifyAll)
            notifyAll();
            
        } catch (InterruptedException e) {
            System.out.println("Blad adresata");
        }
    }
}

public class popZad2_listonosz {
    public static void main(String[] args) {
        MailBox skrzynka = new MailBox();

        Thread listonosz = new Thread(new Runnable() {
            @Override
            public void run() {
                skrzynka.zostawList();
            }
        });

        Thread adresat = new Thread(new Runnable() {
            @Override
            public void run() {
                skrzynka.odbierzList();
            }
        });

        listonosz.start();
        adresat.start();
    }
}