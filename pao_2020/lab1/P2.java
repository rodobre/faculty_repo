import java.util.Scanner;

public class P2 {
	public static void main(String[] args) {
		Scanner arguments = new Scanner(System.in);
		System.out.println("Input Data:");
		System.out.print("Input first integer: ");
		int a = arguments.nextInt();

		System.out.print("Input second integer: ");
		int b = arguments.nextInt();

		System.out.println("Expected output: ");

	        if(a < b)
	            System.out.println(a + " < " + b);
	        if(a > b)
	            System.out.println(a + " > " + b);
	        if(a == b)
	            System.out.println(a + " == " + b);
	        if(a != b)
	            System.out.println(a + " != " + b);
	        if(a <= b)
	            System.out.println(a + " <= " + b);
	        if(a >= b)
	            System.out.println(a + " < " + b);
	}
}
