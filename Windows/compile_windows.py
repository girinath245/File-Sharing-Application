import os 

os.system("gcc -std=c17 sender.c sender_helper.c -o sender.exe -O3 -Wall -Werror -lws2_32")

os.system("gcc -std=c17 reciever.c reciever_helper.c -o recieve.exe -O3 -Wall -Werror -lws2_32")

#os.system("del networking_handler_sender.o networking_handler_recieve.o sender.o recieve.o")

print("Done!!")

input()