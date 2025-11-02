//Task5 - SimpleCalcultor Class
import java.util.Scanner;

public class SimpleCalculator {

    // ====== METHODS FOR OPERATIONS ======

    public static double add(double a, double b) {
        return a + b;
    }

    public static double subtract(double a, double b) {
        return a - b;
    }

    public static double multiply(double a, double b) {
        return a * b;
    }

    public static double divide(double a, double b) {
        if (b == 0) {
            System.out.println("Error!! Division by zero!");
            return 0; // return 0 by convention
        }
        return a / b;
    }
}

