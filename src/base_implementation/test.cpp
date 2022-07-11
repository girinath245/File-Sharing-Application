#include "file_impl.h"

using namespace std;

struct Timer 
{
    decltype(std::chrono::steady_clock::now()) start ;
    Timer() 
    {
        start = std::chrono::steady_clock::now();
    }
    
    double getTime() 
    {
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start; 
        return elapsed_seconds.count() ; 
    }
};

double new_approach()
{
    string path = "new.txt";

    file f(path);

    FILE *w = fopen("new_made_by_c++.txt", "wb");

    double sum = 0 ; int avg_count = 0 ;
    while (f.size_left() > 0)
    {
        chunk chunkp;
        {
            Timer t ;
            chunkp = f.load_region();
            sum += t.getTime() ;
            avg_count++;
        }
        
        // cout << "The value is " << chunk.amount_last_retrieved << endl ;

        fwrite(chunkp.current_region, sizeof(unsigned char), chunkp.amount_last_retrieved, w);

        // fseek(w, 0L, SEEK_END);
    }

    fclose(w) ;
    cout << sum / avg_count << endl;

    return sum / avg_count ;
}

double old_approach()
{
    const int64_t region_size = 10 * 1024 * 1024; 

    FILE *f = fopen("new.txt" , "rb") ;

    FILE *w = fopen("new_made_by_c++.txt", "wb");

    fseek(f, 0, SEEK_END);
    int64_t file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    auto region = malloc(region_size) ;

    double sum = 0 ; int avg_count = 0 ;
    while (file_size > 0) 
    {
        chunk chunkp;
        {
            Timer t ;
            fread(region, sizeof(unsigned char), std::min(region_size, file_size), f) ;
            sum += t.getTime() ;
            avg_count++;
        }
        
        fwrite(region, sizeof(unsigned char), std::min(region_size, file_size), w);

        file_size -= region_size ;
    }

    fclose(w) ;

    cout << sum / avg_count << endl;

    return sum / avg_count ;
}

int main()
{
    double gains = 0 ;
    double hgain = -10000 ;
    int total_instances = 5 ;

    for (int i = 1 ; i <= total_instances ; i++) {
        cout << "\nNew Approach:  " ;
        auto n = new_approach() ;

        cout << "Olds Approach:  " ;
        auto o = old_approach() ;

        cout << "Test Number " << i  << ": " << o / n << endl ;

        gains += o / n ;
        hgain = max (hgain  , o / n) ;
    }

    cout << "\nHighest Gain is " << hgain << endl ;
    cout << "Mean Gain is " << gains / total_instances << endl ;
    
    return 0;

}