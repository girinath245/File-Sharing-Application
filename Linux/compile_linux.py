import os 

os.system("gcc sender.c -o sender -Wall -Werror -O3")
os.system("gcc recieve.c -o recieve -O3 -Wall -Werror")

print("Done!!")