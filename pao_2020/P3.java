import java.util.Scanner;

public class P3 {
	public static void main(String[] args) {
		Scanner arguments = new Scanner(System.in);
		int n = arguments.nextInt(), sum = 0;

		for(int i = 3; i <= n; ++i)
		{
			if(i % 3 == 0 || i % 5 == 0)
				sum += i;
		}

		System.out.println("Sum: " + sum);
	}
}
