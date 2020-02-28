import java.util.Scanner;

public class P5 {
	public static void main(String[] args) {
		Scanner arguments = new Scanner(System.in);
		int n = arguments.nextInt();

		for(int i = 2; i*i <= n; ++i)
			if(n % i == 0)
			{
				System.out.println("Number is not prime");
				return;
			}

		System.out.println("Number is prime");
	}
}
