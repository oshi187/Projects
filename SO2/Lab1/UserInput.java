//Task1 - Main class
import java.util.Scanner;

public class UserInput {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in); //in built java class to get user input

        System.out.println("Moin"); //printing Good Day in Hamburg

        //Creating objects of class Person using new operator and values are given for the attributes of the class
        Person person_1 = new Person("Julia", "Müller", 18, 7, 2004);
        Person person_2 = new Person("Lena", "Melchert", 15, 1, 1990);
        Person person_3 = new Person("Noah", "Köner", 3, 4, 2003);


        while(true) {
            //Displaying the menu
            System.out.println("Who wants to change the name?");
            System.out.println("1 - Person 1");
            System.out.println("2 - Person 2");
            System.out.println("3 - Person 3");
            System.out.println("4 - Data of all the people");
            System.out.println("0 - Exit");

            System.out.print("Enter your choice:");
            int choice = scanner.nextInt(); //gets an integer as input
            scanner.nextLine();//clear input buffer as java doesn't consume the /n character

            if(choice == 0){
                System.out.println("Program has ended"); //program termination
                break;
            }else if(choice == 4){
                System.out.println("Stored data");
                person_1.printInfo();
                person_2.printInfo();
                person_3.printInfo();
            }else if(choice >=1 && choice <= 3) {
                Person selected;
                if (choice == 1) selected = person_1;
                else if (choice == 2) selected = person_2;
                else selected = person_3;


                System.out.print("Enter the new first name:");
                String newFirst = scanner.nextLine();

                System.out.print("Enter the new last name:");
                String newLast = scanner.nextLine();

                System.out.print("Enter the new day:");
                int newDay = scanner.nextInt();

                System.out.print("Enter the new month:");
                int newMonth = scanner.nextInt();

                System.out.print("Enter the new year:");
                int newYear = scanner.nextInt();

                if (selected.changeCheck(newDay, newMonth, newYear)) { //Checking if change is possible (returns true)
                    selected.changeName(newFirst,newLast,newDay,newMonth,newYear); //Changing name by calling relevant method
                    System.out.println("Name successfully changed");
                    selected.printInfo();
                }else{
                    System.out.println("Error!! Name can only be changed after 3 years");
                }
                scanner.nextLine();//clear input buffer
            }else{
                System.out.println("Invalid input. Try again.");
            }
        }
        scanner.close();//informing compiler that we are done reading user input
    }
}

