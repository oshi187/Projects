import java.util.Scanner;

public class Lab1_task6 {

    // Check if a character is a vowel
    public static boolean isVowel(char c) {
        c = Character.toLowerCase(c);
        return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("=====================================");
        System.out.println("        WORD COUNTER PROGRAM");
        System.out.println("=====================================");

        // Taking Input from the User
        System.out.println("Enter a sentence or paragraph to analyze:");
        String text = scanner.nextLine();

        // Declaring Variables
        int totalCharsWithSpaces = text.length();
        int totalCharsWithoutSpaces = 0;
        int vowels = 0;
        int consonants = 0;
        int sentences = 0;
        // Checking through each character
        for (int i = 0; i < text.length(); i++) {
            char c = text.charAt(i);

            // Counting characters, vowels, consonants, and sentences
            if (Character.isLetter(c)) {
                totalCharsWithoutSpaces++;
                if (isVowel(c)) vowels++;
                else consonants++;
            } else if (c != ' ') {
                totalCharsWithoutSpaces++;
            }

            // Counting sentences
            if (c == '.' || c == '!' || c == '?') {
                sentences++;
            }
        }

        // // Split text into words and count them
        String[] words = text.trim().split("\\s+");
        int wordCount = (text.trim().isEmpty()) ? 0 : words.length;

        // Find the longest word
        String longestWord = "";
        for (String word : words) {
            // remove punctuation for accuracy
            String cleanWord = word.replaceAll("[^a-zA-Z]", "");
            if (cleanWord.length() > longestWord.length()) {
                longestWord = cleanWord;
            }
        }

        // Display the report
        System.out.println("\n=== TEXT ANALYSIS REPORT ===");
        System.out.println("Original text: \"" + text + "\"\n");

        System.out.println("Character Statistics:");
        System.out.println("- Total characters (with spaces): " + totalCharsWithSpaces);
        System.out.println("- Total characters (without spaces): " +     totalCharsWithoutSpaces);
        System.out.println("- Number of vowels: " + vowels);
        System.out.println("- Number of consonants: " + consonants);

        System.out.println("\nWord Statistics:");
        System.out.println("- Total number of words: " + wordCount);
        System.out.println("- Longest word: \"" + longestWord + "\"");

        System.out.println("\nSentence Statistics:");
        System.out.println("- Total number of sentences: " + sentences);

        System.out.println("=====================================");
        System.out.println("       END OF ANALYSIS REPORT");
        System.out.println("=====================================");

        scanner.close();
    }
}