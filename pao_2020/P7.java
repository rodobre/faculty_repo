import java.util.Scanner;

public class P7 {
	public static void main(String[] args) {
		Scanner arguments = new Scanner(System.in);
		int n = arguments.nextInt();
		int m = 0;

		for(int i = 2; i <= n; ++i) {
			if(n % i == 0) {
				boolean no_larger = true;

				for(int d = 2; d * d <= i; ++d) {
					if(i % d == 0)
						no_larger = false;
				}

				if(no_larger)
					m = i;
			}
		}

		System.out.println(m);
	}
}
