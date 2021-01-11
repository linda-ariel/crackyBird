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
        
    int levelMap[50][3];    
    
    //Verwaltung von Hindernisse
    int hindernisAnzahl;
    list<int> hindernisPosition;
    int blockAnzahl = 0;
    bool max = false;
    int leer = 0;
    
    //Verwaltung von Items
    int itemsAnzahl;
    list <tuple <int,int> > itemsPosition;
    
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
    int getV() { return v; }
  
    
    // setter
    void setV(int v) { this->v = v; }

    
    //Plaziert die Hindernisse 
    void fuellMap(){
        
        for (int i = 0; i < 50; i++) {
            
            for(int k = 0; k < 3 ; k++){
                
                if(max){
                   levelMap[i][k] = 0; 
                   leer ++;
                   if(leer ==15){
                       max = false;
                       leer = 0;
                   }
                }else{
                    int random = rand() % 2;
                    levelMap[i][k] = random; 
                    if (random == 1){
                        
                        blockAnzahl++;
                        
                        if(blockAnzahl == 2){
                            max = true;
                            blockAnzahl = 0; 
                        }
                    }
                }
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
    
    // bewegt alle Rects nach links und löscht, die die schon zu weit sind
    void move() {
        
        cout << "move" <<endl;
     
        //leere Liste sollte die Funktion gleich beenden
        if (strecke.empty()) {
            return;
        }
        
        int neuX = strecke.front()->getX();           
        
          //geht durch die Liste und bewegt die Rechtsecke
          auto it = strecke.begin();
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
              
                 // Falls hindernis nicht mehr zusehen ist, Löschen wir es
                if (neuX <= -50) {
                    delete (block);
                    it = strecke.erase(it);
                    std::cout << "deletei" << endl;
                }else{
                    it++;
                }
          }
    } 
    
    void add(int h){
        
        move();
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        
        int height = rand() % 70 + 50;
        int y = 500 - height/2 - h * 220;
        
        hindernis = new Rect(700, y, 70, height, &view);
        hindernis->setFill(r,g,b);
        strecke.push_back(hindernis);
        
        cout << "add" <<endl;
        //move();
    }
    
    
    bool ist1(int i, int j){
        
        if (levelMap[i][j] == 1) {
            
            return true;
        }
    
    return false;
        
    }
    
    
};

#endif