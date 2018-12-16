/**
 * @author shines77
 */
package net.i77soft.algorithm;

import java.util.Scanner;

/**
 * @author shines77
 *
 */
class StopWatch {
    private long startTime;
    private long endTime;

    StopWatch() {
        startTime = 0;
        endTime = 0;
    }

    public void start() {
        startTime = System.currentTimeMillis();
    }

    public void stop() {
        endTime = System.currentTimeMillis();
    }

    public long getElapsedMillsecs() {
        return (endTime - startTime);
    }
}

/**
 * @author shines77
 *
 */
public class Program {	
	/**
	 * @param n
	 */
    static int fibonacci(int n) {
        if (n >= 3)
            return fibonacci(n - 1) + fibonacci(n - 2);
        else
            return 1;
    }

	/**
	 * @param n
	 */
    static void test_fibonacci(int n) {
        StopWatch sw = new StopWatch();
        sw.start();
        int fib_n = fibonacci(n);
        sw.stop();
        System.out.printf("fibonacci(%d) = %d\n\n", n, fib_n);
        System.out.printf("elapsed time: %d ms.\n\n", sw.getElapsedMillsecs());
    }

	/**
	 * @param args
	 */
    public static void main(String[] args) {
        System.out.print("\n");
        System.out.print("Input a number (1-50): ? ");
        @SuppressWarnings("resource")
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        System.out.print("\n");

        test_fibonacci(n);
    }
}
