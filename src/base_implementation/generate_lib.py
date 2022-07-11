import os 

os.system("g++ -std=c++2a network_base.cpp -c -O3 -Wall -Werror -lws2_32")

os.system("g++ -std=c++2a sender.cpp -c -O3 -Wall -Werror -lws2_32")

os.system("g++ -std=c++2a reciever.cpp  -c -O3 -Wall -Werror -lws2_32")

print("Done!!")

input()