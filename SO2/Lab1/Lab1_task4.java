//Task4
import java.util.Scanner;

public class Lab1_task4 {

    // Method: Convert numerical grade to letter grade
    public static String getLetterGrade(double grade) {
        if (grade >= 90) return "A";
        else if (grade >= 80) return "B";
        else if (grade >= 70) return "C";
        else if (grade >= 60) return "D";
        else return "F";
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("=====================================");
        System.out.println("        GRADE MANAGER SYSTEM");
        System.out.println("=====================================");

        // Ask for number of students
        System.out.print("Enter number of students: ");
        int numStudents = scanner.nextInt();

        double[] grades = new double[numStudents];
        double total = 0;
        double highest = Double.MIN_VALUE;
        double lowest = Double.MAX_VALUE;
        int passed = 0;
        int failed = 0;

        // Input grades and calculate statistics
        for (int i = 0; i < numStudents; i++) {
            System.out.print("Enter grade for student " + (i + 1) + " (0–100): ");
            grades[i] = scanner.nextDouble();

            total += grades[i];
            if (grades[i] > highest) highest = grades[i];
            if (grades[i] < lowest) lowest = grades[i];
            if (grades[i] >= 60) passed++;
            else failed++;
        }

        double average = total / numStudents;

        // Display grade report
        System.out.println("\n=== Grade Report ===");
        for (int i = 0; i < numStudents; i++) {
            System.out.printf("Student %d: %.1f (%s)%n", (i + 1), grades[i], getLetterGrade(grades[i]));
        }

        // Display statistics
        System.out.println("\n=== Statistics ===");
        System.out.println("Passing Grade: 60 (D or higher)");
        System.out.println("-------------------------------------");
        System.out.printf("Average Grade: %.2f%n", average);
        System.out.printf("Highest Grade: %.1f%n", highest);
        System.out.printf("Lowest Grade: %.1f%n", lowest);
        System.out.printf("Students Passed: %d%n", passed);
        System.out.printf("Students Failed: %d%n", failed);

        System.out.println("=====================================");
        System.out.println("       End of Grade Report");
        System.out.println("=====================================");

        scanner.close();
    }
}

