#include "file_impl.h"

const size_t region_size = 10 * 1024 * 1024; // 10 MB

file::file(const std::string &path, uint64_t position )
{   
    filename = path;
    std::cout << filename << std::endl;
    f_ptr = fopen(filename.c_str(), "rb");

    if (f_ptr == (FILE*)0)
        std::cerr << "Error occured while opening the file\n";

    remaining_file_size_to_be_sent = this->size() - position ;

    std::cout << size() << std::endl;

    int val = 1 ;
    if (remaining_file_size_to_be_sent > region_size) val++;

    regions = std::vector<char *> (val , nullptr);

    if (position > 0) fseek(f_ptr , position , SEEK_SET) ;

    std::cout << "Created\n" ;
}

size_t file::size_left()
{
    return remaining_file_size_to_be_sent;
}

size_t file::size()
{
    if (first_time)
    {
        fseek(f_ptr, 0, SEEK_END);
        file_size = ftell(f_ptr);
        fseek(f_ptr, 0, SEEK_SET);
    }

    return file_size;
}

size_t file::retrieve_region()
{
    if (remaining_file_size_to_be_sent > 0) 
    {
        useful_region = 1 - useful_region; // flipped garbage now

        auto read_size = fread(this->regions[useful_region], sizeof(unsigned char), std::min(region_size, remaining_file_size_to_be_sent), f_ptr);

        std::cout << read_size << std::endl;

        return read_size;
    }

    return 0;
}

// The main logic concerning how to load a file
const chunk& file::load_region()
{
    return_val = {nullptr , 0};

    if (first_time)
    {
        regions[0] = (char *)malloc(region_size);
        if (!regions[0])
            std::cerr << "Error occured while allocating memory\n";
        
        if (file_size > region_size)
        {
            regions[1] = (char *)malloc(region_size);
            if (!regions[1])
                std::cerr << "Error occured while allocating memory\n";
        }
            
    }

    if (remaining_file_size_to_be_sent > 0)
    {
        if (!first_time && !(return_val.amount_last_retrieved = successful.get()))
            {
                std::cerr << "Error occured while loading the file\n";
                exit(0) ; 
            }
    
        return_val.current_region = regions[useful_region];

        successful = std::async(std::launch::async, &file::retrieve_region,this);

        if (first_time)
        {
            auto val = successful.get() ;
            // std::cout << "successful is " << val << "\n" ;

            if (!(return_val.amount_last_retrieved = val))
                std::cerr << "Error loading the file\n";

            return_val.current_region = regions[useful_region];
            successful = std::async(std::launch::async, &file::retrieve_region,this);

            first_time = false;
        }
        remaining_file_size_to_be_sent -= return_val.amount_last_retrieved ;
        return return_val;
    }
    return return_val;
}

//TODO : Implement this for cleaning files which are unused for a while and are consuming memory
bool file::is_unused()
{

    return true;
}

file::~file()
{
    for (int i = 0  ; i < 2 ; i++) 
        free(regions[i]);
    fclose(f_ptr);

    std::cout << "Destroyed\n" ;
}