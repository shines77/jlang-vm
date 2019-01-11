
using System;
using System.Diagnostics;

namespace Fibonacci
{
    class Program
    {
        public static int ReadInt()
        {
            int number = 0;
            do
            {
                try
                {
                    string sNumber = Console.ReadLine();
                    number = Int32.Parse(sNumber);
                    return number;
                }
                catch
                {
                    Console.WriteLine("Error: Format error, input again please.");
                }
            }
            while (true);
        }

        public static int fibonacci32(int n)
        {
            if (n >= 3)
                return fibonacci32(n - 1) + fibonacci32(n - 2);
            else
                return 1;
        }

        public static void Main(string[] args)
        {
            Console.WriteLine("");
            Console.Write("Please input a number: [1-50] ? ");
            int n = ReadInt();

            Stopwatch sw = new Stopwatch();
            sw.Start();
            int fib_n = fibonacci32(n);
            sw.Stop();

            Console.WriteLine("");
            Console.WriteLine("Fibonacci({0}) = {1}", n, fib_n);
            Console.WriteLine("elapsed time: {0} ms.", sw.ElapsedMilliseconds);
            Console.WriteLine("");
        }
    }
}
