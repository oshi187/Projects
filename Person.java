public class Person {
    String firstName;
    String lastName;
    int day;
    int month;
    int year;

    //Constructor to define different values for the objects of the class
    public Person(String firstName, String lastName, int day, int month, int year){
        this.firstName = firstName;
        this.lastName = lastName;
        this.day = day;
        this.month = month;
        this.year = year;
    }

    //method to check if change is possible(satisfying the 3 year limit)
    public boolean changeCheck(int newDay,int newMonth,int newYear){
        int yearDiff = newYear - this.year;

        if(yearDiff > 3) return true;
        if (yearDiff < 3) return false;

        if(newMonth > this.month) return true;
        if(newMonth < this.month) return false;

        return (newDay >= this.day);
    }

    //displaying the information of all people
    public void printInfo(){
        System.out.println(firstName + " " + lastName + " " + "(changed on " +  day + "-" + month + "-" + year + ")");
    }

    //method to change name once verified by changeCheck method
    public void changeName(String newFirst, String newLast, int newDay, int newMonth, int newYear){
        this.firstName = newFirst;
        this.lastName = newLast;
        this.day = newDay;
        this.month = newMonth;
        this.year = newYear;
    }
}

