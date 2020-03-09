import java.util.Scanner;

public class P4 {
	public static void main(String[] args) {
		Scanner arguments = new Scanner(System.in);
		int n = arguments.nextInt();
		int factorial_num = 1;

		for(int i = 1; i <= n; ++i)
			factorial_num *= i;

		System.out.println("Factorial: " + factorial_num);
	}
}
