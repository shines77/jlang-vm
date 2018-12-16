@echo off

javac .\src\net\i77soft\algorithm\Program.java

cd .\src\
java -Xint net.i77soft.algorithm.Program

cd ..\release\
@rem java -Xint -classpath .;fibonacci.jar net.i77soft.algorithm.Program

pause
