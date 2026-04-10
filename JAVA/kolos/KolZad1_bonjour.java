import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class nibyKolosZad1 {
    public static void main(String[] args) {
        
        // # tworzymy precyzyjny zegar
        ScheduledExecutorService egzekutor = Executors.newScheduledThreadPool(1);

        Runnable zadanieBonjour = new Runnable() {
            int licznik = 0;

            @Override
            public void run() {
                System.out.println("Bonjour");
                licznik++;
                
                // # koniec po 10 razach
                if (licznik >= 10) {
                    egzekutor.shutdown();
                }
            }
        };

        // # start od 0, rytm co 1, jednostka sekundy
        egzekutor.scheduleAtFixedRate(zadanieBonjour, 0, 1, TimeUnit.SECONDS);
    }
}