#include "file_impl.h"

using namespace std;

int main() 
{
    
    string path ;
    getline(cin, path) ; 
    file f(path);

    FILE *w = fopen("new.mp4","wb") ;

    cout << f.size_left()  << endl;

        int i = 1 ;
        while (f.size_left() > 0)
    {   
        auto chunk  = f.load_region() ;
        //cout << "The value is " << chunk.amount_last_retrieved << endl ;
        
        fwrite(chunk.current_region, sizeof(unsigned char), chunk.amount_last_retrieved, w);

        // fseek(w, 0L, SEEK_END);

        i++;
    }

    cout << i << endl ;
    
    return 0 ;
}

// 1180424028