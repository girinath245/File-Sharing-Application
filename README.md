# File-Sharing-Application

## Purpose

This application was created by making a few changes to the chat application which in turn was motivated by the book "Network Programming in C."

## Languages and Technologies

The application was originally implemented in C. But after certain features were needed like some Object Oriented Capabilities it transitioned to C++. 

## Features
1. Resume Capability
2. Reduced memory usage 
3. Multi File Handling Capability
4. Multi Client Handling Capability 
  
## Using the Application 

1. Just clone the repository and then move to directory of your OS and in that directory use python compile file to compile the program.
2. To send a file from system A to B (A -> B), we just need to run the sender on A and wait for the recieving client on B to connect to us. 
3. Then enter the location of the relative or absolute path of the file and wait.
4. After it has finished sending it will tell you about the total bytes sent. 
5.  Note - The system B needs to know the unique ip and port of system A in order to connect which can be communicated earlier. 
