public class nibyKolosZad2 {
    
    // deklaracja statycznych watkow
    static Thread thread1;
    static Thread thread2;

    public static void main(String[] args) {
        
        thread1 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // usnij na 1000 ms
                    Thread.sleep(1000);

                    System.out.println("Watek 1: Wstaje i przerywam watek 2!");
                    // przerwij thread2
                    thread2.interrupt();
                } catch (InterruptedException e) {
                    System.out.println("Watek 1: Ktos przerwal mi sen!");
                }
            }
        });

        thread2 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // usnij na 2000 ms
                    Thread.sleep(2000);

                    System.out.println("Watek 2: Udalo mi sie pospac cale 2 sekundy");
                } catch (InterruptedException e) {
                    System.out.println("Watek 2: Przerwano mi sen! Zgodnie z poleceniem przerywam watek 1!");
                    // przerwij thread1
                    thread1.interrupt();
                }
            }
        });

        // wystartuj thread1
        thread1.start();
        // wystartuj thread2
        thread2.start();
    }
}