import java.util.Random;
import java.util.Scanner;

public class Lab1_task2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Random random = new Random();

        int target = random.nextInt(100) + 1;  // number between 1-100
        int guess = 0;
        int attempts = 0;

        System.out.println("===========================================");
        System.out.println("Welcome to the Number Guessing Game!");
        System.out.println("Try to guess the number between 1 and 100.");

        while (guess != target) {
            System.out.print("Enter your guess: ");
            guess = scanner.nextInt();
            attempts++;

            if (guess < target)
                System.out.println("Too small! Try again.\n");
            else if (guess > target)
                System.out.println("Too big! Try again.\n");
            else {
                System.out.println("You guessed it! Congratulations!!");
                System.out.println("The number was: " + target);
                System.out.println("It took you " + attempts + " attempt(s).");
            }
        }

        scanner.close();
    }
}
 