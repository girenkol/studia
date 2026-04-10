import java.util.concurrent.locks.ReentrantLock;

class AClass {
    public synchronized void method1() {
        // instr1;
    }
    public synchronized void method2() {
        // instr2;
    }
}

class AClass_odp {

    ReentrantLock zamek = new ReentrantLock();

    public void method1() {
        zamek.lock();
        System.out.println("zamek locked");

        try {
            System.out.println("Wykonuje instr1");
        } finally {
            zamek.unlock();
            System.out.println("zamek unlocked");
        }
    }
    public void method2() {
        zamek.lock();

        try {
            System.out.println("Wykonuje instr2");
        } finally {
            zamek.unlock();
            System.out.println("zamek unlocked");
        }
    }
}

public class poprawa_z1 {
    public static void main(String[] args) {
        AClass_odp klucznik = new AClass_odp();
        klucznik.method1();
    }
}