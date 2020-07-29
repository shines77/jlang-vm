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
        System.out.printf("\n");
        @SuppressWarnings("resource")
        Scanner scanner = new Scanner(System.in);

        int n = 1;
        int max_n = 45;
        do {
            if (n == 0 || n > max_n) {
                System.out.printf("\n");
                System.out.printf("The number must be on range [1-%d].\n\n", max_n);
            }
            //System.out.print("Please enter a number from 1 to %d.\n", max_n);
            //System.out.print("n = ? ");
            System.out.printf("Input a number [1-%d]: ? ", max_n);
            n = scanner.nextInt();
            System.out.printf("\n");
        } while (n > max_n);

        test_fibonacci(n);
    }
}
