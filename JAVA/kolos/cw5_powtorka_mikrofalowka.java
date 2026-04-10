class Mikrofalowka implements Runnable {
    @Override
    public void run() {
        System.out.println("Mikrofalowka: Zaczynam podgrzewac obiad, ustawiono na 10 sekund...");
        try {
            Thread.sleep(10000);
        } catch(InterruptedException e) {
            System.out.println("Podgrzewanie zostalo przerwane, bo otworzono drzwiczki");
        }
    }
}

public class cw5_powtorka_mikrofalowka {
    public static void main (String[] args){
        System.out.println("Programista: Wstawiam obiad.");

        Mikrofalowka programistycznMikrofalowka = new Mikrofalowka();
        Thread watek = new Thread(programistycznMikrofalowka);
        watek.start();

        try {
            Thread.sleep(3000);
        } catch(InterruptedException e) {
            System.out.println("Blad systemu!");
        }

        System.out.println("Programista nie wytrzymal cisnienia i otworzyl drzwiczki");
        watek.interrupt();
    }
}