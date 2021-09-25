import os 

os.system("g++ -std=c++17 sender_app.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp -o sender.exe -O3 -Wall -Werror -lws2_32")

os.system("g++ -std=c++17 reciever_app.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o recieve.exe -O3 -Wall -Werror -lws2_32")

print("Done!!")

input()