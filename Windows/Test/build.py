import os 

os.system("g++ -std=c++17 test_send.cpp ../base_implementation/sender.cpp  ../base_implementation/network_base.cpp -o test_send.exe -O3 -Wall -Werror -lws2_32")

os.system("g++ -std=c++17 test_recieve.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o test_recieve.exe -O3 -Wall -Werror -lws2_32")

print("Done!!")

input()