import java.util.Scanner;

public class Main{
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean running = true;

        while (running) {
            // Display menu
            System.out.println("\n=== Simple Calculator ===");
            System.out.println("1. Addition (+)");
            System.out.println("2. Subtraction (-)");
            System.out.println("3. Multiplication (*)");
            System.out.println("4. Division (/)");
            System.out.println("5. Exit");
            System.out.print("Choose an operation (1-5): ");

            int choice = scanner.nextInt();

            if (choice == 5) {
                System.out.println("Thank you for using the calculator!");
                running = false;
                continue;
            }

            // Input numbers
            System.out.print("Enter first number: ");
            double num1 = scanner.nextDouble();

            System.out.print("Enter second number: ");
            double num2 = scanner.nextDouble();

            double result = 0;

            // Choose operation
            switch (choice) {
                case 1:
                    result = SimpleCalculator.add(num1, num2);
                    System.out.println("Result: " + num1 + " + " + num2 + " = " + result);
                    break;
                case 2:
                    result = SimpleCalculator.subtract(num1, num2);
                    System.out.println("Result: " + num1 + " - " + num2 + " = " + result);
                    break;
                case 3:
                    result = SimpleCalculator.multiply(num1, num2);
                    System.out.println("Result: " + num1 + " * " + num2 + " = " + result);
                    break;
                case 4:
                    result = SimpleCalculator.divide(num1, num2);
                    System.out.println("Result: " + num1 + " / " + num2 + " = " + result);
                    break;
                default:
                    System.out.println("Invalid choice! Please select 1-5.");
            }
        }

        scanner.close();
    }

}
