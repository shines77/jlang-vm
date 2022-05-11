@echo off

javac .\src\net\i77soft\algorithm\Program.java

::
:: Disabling the JIT
:: https://www.ibm.com/docs/en/sdk-java-technology/8?topic=compiler-disabling-jit
::

cd .\src\
:: java -Djava.compiler=NONE net.i77soft.algorithm.Program
java -Xint net.i77soft.algorithm.Program

:: cd ..\release\
:: java -Xint -classpath .;fibonacci.jar net.i77soft.algorithm.Program

pause
