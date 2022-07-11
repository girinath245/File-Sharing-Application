#ifndef FILE_IMPL_H
#define FILE_IMPL_H

#include <iostream>
#include <future>
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

struct chunk
{
    const char *current_region = nullptr;
    size_t amount_last_retrieved = 0;
};

struct file
{
    private:
        std::string filename ; 
        FILE *f_ptr = nullptr;

        std::vector<char*> regions ;

        // contains useful value
        int8_t useful_region = 1;
        size_t remaining_file_size_to_be_sent = 0;

        bool first_time = true ; 
        size_t file_size  = 0;

        chunk return_val ;
        size_t retrieve_region();

        std::future<size_t> successful ;

    public:
        file(const std::string &path,uint64_t position = 0);
        size_t size_left();
        size_t size() ;        

        const chunk& load_region() ;

        bool is_unused() ;

        ~file();
};

#endif