import os 

os.system("gcc -std=c17 -c networking_handler_sender.c networking_handler_recieve.c -O3")

os.system("gcc -std=c17 sender.c networking_handler_sender.o networking_handler_recieve.o -o sender.exe -O3 -Wall -Werror -lws2_32")

os.system("gcc -std=c17 recieve.c networking_handler_sender.o networking_handler_recieve.o -o recieve.exe -O3 -Wall -Werror -lws2_32")

os.system("del networking_handler_sender.o networking_handler_recieve.o sender.o recieve.o")

print("Done!!")

input()