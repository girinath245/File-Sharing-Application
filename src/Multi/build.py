import os 

os.system("g++ -std=c++17 multi_hosting_server.cpp ../base_implementation/sender.cpp  ../base_implementation/network_base.cpp -o multi_send -O3 -Wall -lpthread")

os.system("g++ -std=c++17 multi_recieving_client.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o multi_recieve -O3 -Wall -lpthread")

print("Done!!")

input()
