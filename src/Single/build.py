import os
import sys

## Make vs Python

def build_release():

    print("Building in release mode!!")

    if (os.name == 'nt'):

        os.system("g++ -std=c++23  sender_app.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp ../base_implementation/file_impl.cpp -o sender.exe -O3 -Wall -Werror -lws2_32")

        os.system("g++ -std=c++23 reciever_app.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o recieve.exe -O3 -Wall -Werror -lws2_32")
    
    else :
        os.system("g++-11 -std=c++23 sender_app.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp ../base_implementation/file_impl.cpp -o sender -O3 -Wall -Werror -lpthread")

        os.system("g++-11 -std=c++23 reciever_app.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o recieve -O3 -Wall -Werror")
    
    print("Done!!")

def build_debug(): 

    print("Building in debug mode!!")

    if (os.name == 'nt'):
        os.system("g++ -std=c++23  sender_app.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp ../base_implementation/file_impl.cpp -o sender.exe -g -Wall -Werror -lws2_32")

        os.system("g++ -std=c++23 reciever_app.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o recieve.exe -g -Wall -Werror -lws2_32")
    
    else :
        os.system("g++-11 -std=c++23 sender_app.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp ../base_implementation/file_impl.cpp -o sender -g -Wall -Werror -lpthread")

        os.system("g++-11 -std=c++23 reciever_app.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o recieve -g -Wall -Werror")
    
    print("Done!!")

file_to_lists = [
    "\"Demon Slayer.mkv\"",
    "Eternals.2021.720p.WEBRip.x264.AAC-[YTS.MX].mp4",
    "Nobody.2021.1080p.WEBRip.x264.AAC5.1-[YTS.MX].mp4"
]

def build_test():

    print("Building tests!!")

    if (os.name == 'nt'):

        os.system("g++ -std=c++23  test_sender.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp ../base_implementation/file_impl.cpp -o test_sender.exe -O3 -Wall -Werror -lws2_32")

        os.system("g++ -std=c++23 test_reciever.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o test_recieve.exe -O3 -Wall -Werror -lws2_32")
    
    else :
        os.system("g++-11 -std=c++23 test_sender.cpp ../base_implementation/sender.cpp ../base_implementation/network_base.cpp ../base_implementation/file_impl.cpp -o test_sender -O3 -Wall -Werror -lpthread")

        os.system("g++-11 -std=c++23 test_reciever.cpp ../base_implementation/reciever.cpp ../base_implementation/network_base.cpp -o test_recieve -O3 -Wall -Werror")
    
    print("Done!!")

def test() :
    file_from_lists = [
        "\"/media/girinath/Data/Movies and Tv/sample/Demon Slayer.mkv\"", 
        "\"/media/girinath/Data/Movies and Tv/sample/Eternals.2021.720p.WEBRip.x264.AAC-[YTS.MX].mp4\"",
        "\"/media/girinath/Data/Movies and Tv/sample/Nobody.2021.1080p.WEBRip.x264.AAC5.1-[YTS.MX].mp4\""
        ]   
    
    if (os.name == 'nt'):
        file_from_lists [
            "\"D:\\Movies and Tv\\sample\\Demon Slayer.mkv\"",
            "\"D:\\Movies and Tv\\sample\\Eternals.2021.720p.WEBRip.x264.AAC-[YTS.MX].mp4\"",
            "\"D:\\Movies and Tv\\sample\\Nobody.2021.1080p.WEBRip.x264.AAC5.1-[YTS.MX].mp4\""
        ] 

    i = 0 

    while i < len(file_from_lists):
        print("checking {to}".format(to = file_to_lists[i]))

        command = "diff {fromi} {to}".format(fromi = file_from_lists[i] , to = file_to_lists[i])
        
        if (os.name == 'nt'):
            command = "fc {fromi} {to}".format(fromi = file_from_lists[i] , to = file_to_lists[i])
        
        os.system(command)
        
        i += 1

def clean_test() :
    print("cleaning downloaded files!!")
    pass

if __name__ == "__main__":
    
    if (len(sys.argv) > 1 and sys.argv[1] == 'd') :
        build_debug()
    
    elif (len(sys.argv) > 1 and sys.argv[1] == 'bt') :
        build_test()

    elif (len(sys.argv) > 1 and sys.argv[1] == 'ct') :
        clean_test()

    elif(len(sys.argv) > 1 and sys.argv[1] == 't') :
        test()

    else :
        build_release()

    print("\n\nPress any key to exit")
    input()
