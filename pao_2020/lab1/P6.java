import java.util.Scanner;

public class P6 {
	static int findNthFiboNumber(int N)
	{
		int A = 1, B = 1, sum = 0;

		for(int i = 2; i < N; ++i)
		{
			sum = A + B;
			A = B;
			B = sum;
		}

		return sum;
	}

	public static void main(String[] args) {
		Scanner arguments = new Scanner(System.in);
		int n = arguments.nextInt();

		System.out.println(findNthFiboNumber(n));
	}
}
