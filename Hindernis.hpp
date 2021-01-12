#include <iostream>
#include <algoviz/SVG.hpp>

using namespace std; 

#ifndef _HINDERNIS_
#define _HINDERNIS_

class Hindernis {
    
    private:
    
        int yOben;
    
    public:
    
        Hindernis() {
            yOben = 460;
        }
    
        void setYoben(int yOben){
            this->yOben = yOben;
        }
    
        int getYoben(){
            return yOben;
        }
    
} ;

#endif