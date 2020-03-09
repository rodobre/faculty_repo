import java.util.Scanner;

public class P1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int[] studentGrades = new int[25];

        int s = 0;
        int i = 0;

        while(true) {
            if(i >= 25)
                break;
            int grade = scanner.nextInt();
            if(grade == -1) {
                System.out.println("Average: " + s/i);
                break;
            }
            else {
                studentGrades[i] = grade;
                s += grade;
                ++ i;
            }
        }
    }
}
