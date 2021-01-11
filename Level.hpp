#include <algoviz/SVG.hpp>
#include <cstdlib>
#include <tuple>
#include <list>

using namespace std;

#ifndef _LEVEL_
#define _LEVEL_

class Level{

private:
    
    SVG view;
    
    int hindernisAnzahl;
    int itemsAnzahl;
    list <tuple <int,int> > itemsPosition;
    list<int> hindernisPosition;
    int levelMap[50][3];    
    
    
    //strecke
    list<Rect*> strecke;
    Rect* hindernis;
    
    // geschwindigkeit
    int v;

    
public:
    
    //konstruktor
    Level(int v, int itemsAnzahl, SVG &view){ 
        this -> v = v;
        this -> itemsAnzahl = itemsAnzahl;
        this -> view = view;
    }
    
    // getter
    void getV() { return v; }
  
    
    // setter
    void setV(int v) { this->v = v; }

    
    //Plaziert die Hindernisse 
    void fuellMap(){
        
        for (int i = 0; i < 50; i++) {
            
            for(int k = 0; k < 2 ; k++){
                
                int random = rand() % 2;
                levelMap[i][k] = random; 
                //std::cout << levelMap[i][k] << endl;

            }           
        }
    }

    //Plaziert eine beliebige Anzahl an Items in einer Random Ort,  wo noch kein Hinderniss ist
    void plaziereItems(int anzahl){
        
        for(int i = 0; i < anzahl; i++){
            int randomPosition = rand() % 50 + 0;
            int randomHeight = rand() % 2 + 0 ;
            itemsPosition.push_back(std::make_tuple (randomPosition, randomHeight));     
            
        }
        int positionX;
        int height;
        
        for ( auto i : itemsPosition){
            positionX = get<0>(i);
            height = get<1>(i);
            
            if( levelMap[positionX][height] == 0 ){
                levelMap[positionX][height] = 2;
                
            }else{
            
               if(height -1 >= 0 && levelMap[positionX][height-1] == 0 ){
                   levelMap[positionX][height-1] = 2;
                   
               } else{
                   levelMap[positionX][2] = 2;
               }
            }
        }      
    }
    
    // bewegt Rect nach links
    void move() {
        
        cout << "move" <<endl;
        
        //leere Liste sollte die Funktion gleich beenden
        if (strecke.empty()) {
            return;
        }
        
        // Falls hindernis nicht mehr zusehen ist, Löschen wir es
        int neuX = strecke.front()->getX();
        
        if (neuX < -50) {

            delete (strecke.front());
            strecke.pop_front();
            std::cout << "deletei" << endl;
        }

        auto it = strecke.begin();

        // für jede leere gehen wir 50 pixels
        for (int j = 0; j < 10; j++) {


          while (it != strecke.end()) {
            // hole ein zeiger
            Rect* block = *it;

            // hole seine position
            int x = block->getX();
            int y = block->getY();

            // berechne neue position
            neuX = x - v;

            // bewege block
            block->moveTo(neuX, y);
            AlgoViz::sleep(5);

            it++;
          }
        }
    } 
    
    void add(int h){
        
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        
        int height = rand() % 70 + 50;
        int y = 400 - h * 100;
        
        hindernis = new Rect(600, y, 50, 50, &view);
        hindernis->setFill(r,g,b);
        strecke.push_back(hindernis);
        cout << "add" <<endl;
    }
    
    
    bool ist1(int i, int j){
        
        if (levelMap[i][j] == 1) {
            
            return true;
        }
    
    return false;
        
    }
    
    
};

#endif