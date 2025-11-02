import java.util.Scanner;

public class RomanNumerals {
    public static int getValue(char c) {
        switch (c) {
            case 'I': return 1;
            case 'V': return 5;
            case 'X': return 10;
            case 'L': return 50;
            case 'C': return 100;
            case 'D': return 500;
            case 'M': return 1000;
            default:  return 0; // invalid symbol
        }
    }

    public static int romanToInt(String roman) {
        int total = 0;

        for (int i = 0; i < roman.length(); i++) {
            int current = getValue(roman.charAt(i));

            // Look ahead to the next symbol
            if (i < roman.length() - 1) {
                int next = getValue(roman.charAt(i + 1));

                if (current < next) {
                    // Subtractive case (e.g., IV, IX)
                    total -= current;
                } else {
                    total += current;
                }
            } else {
                // Last symbol always added
                total += current;
            }
        }

        return total;
    }

    public static void main(String[] args) {
            Scanner scanner = new Scanner(System.in);
            while(true) {
                System.out.print("Enter a Roman numeral as a string:");
                String roman = scanner.nextLine().toUpperCase(); /* reading characters always in uppercase */

                if (roman.equals("0")) {
                    System.out.println("Program ended. Goodbye!");
                    break; // exits the while loop
                }

                int result = romanToInt(roman);
                System.out.println("The integer value of " + roman + " is " + result);
            }
    }
}
