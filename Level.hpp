#include <algoviz/SVG.hpp>
#include <cstdlib>
#include <list>
#include <stdlib.h>     /* abs */


using namespace std;

#ifndef _LEVEL_
#define _LEVEL_

class Level {

private:
    SVG *view;

    // 0 = leer   1 = hindernisse   2 = Münze   3 = Feind   4 =  Mario Block
    int levelMap[100][3];

    //Verwaltung von Hindernisse
    int hindernisAnzahl;
    list<int> hindernisPosition;
    int blockAnzahl = 0;
    bool max = false;
    int leer = 0;
    
    list <Rect*> bloeckeNah;

    //Anzahl von Items
    int marioBlockAnzahl;
    int muenzeAnzahl;
    int feindAnzahl;

    //strecke
    list<Rect*> strecke;
    Rect* hindernis;

    // geschwindigkeit
    double v;

public:
    //konstruktor
    Level(int v, int muenzeAnzahl, int feindAnzahl, int marioBlockAnzahl, SVG *view){
        
        this->v = v;
        this->muenzeAnzahl = muenzeAnzahl;
        this->feindAnzahl = feindAnzahl;
        this->marioBlockAnzahl = marioBlockAnzahl;
        this->view = view;
        
        fuellMap();
        plaziereMuenze(muenzeAnzahl);
        plaziereFeind(feindAnzahl);
        plaziereMarioBlock(marioBlockAnzahl);
    }

    // getter
    double getV() { return v; }

    // setter
    void setV(double v) { this->v = v; }

    //Plaziert die Hindernisse
    void fuellMap(){

        //Iterator für die länge
        for (int i = 0; i < 100; i++) {
            
            //Iterator für die Höhe
            for (int h = 0; h < 3; h++) {

                //falls 2 Blöcke schon nebeneinander sind, setzen wir 15 mal nichts auf
                if (max) {
                    
                    levelMap[i][h] = 0;
                    leer++;
                    
                    if (leer == 51) {
                        max = false;
                        leer = 0;
                    }
                //Sonst plazieren wir zufällig blöcke oder Leere    
                }else {
                    int random = rand() % 2;
                    levelMap[i][h] = random;
                    
                    //wir zählen die Blöcke
                    if (random == 1) {

                        blockAnzahl++;
                    }
                    
                    if (blockAnzahl == 2) {
                        
                        max = true;
                        blockAnzahl = 0;
                        
                    }

                }
            }
        }
    }

    //Plaziert eine beliebige Anzahl an muenze in einer rand()om Ort,  wo noch kein Hinderniss ist
    void plaziereMuenze(int anzahl){

        for (int i = 0; i < anzahl; i++) {

            int hoehe = rand() % 2 + 0;
            int positionX = rand() % 100 + 0;

            while (levelMap[positionX][hoehe] != 0) {

                positionX = rand() % 100 + 0;
                hoehe = rand() % 2 + 0;
            }

            levelMap[positionX][hoehe] = 2;
        }
    }

    //Plaziert eine beliebige Anzahl an Feinde in einer random Ort,  wo noch kein Hinderniss ist
    void plaziereFeind(int anzahl){

        for (int i = 0; i < anzahl; i++) {

            int positionX = rand() % 100 + 0;

            while (levelMap[positionX][0] != 0) {

                positionX = rand() % 100 + 0;
            }

            levelMap[positionX][0] = 3;
        }
    }

    //Plaziert eine beliebige Anzahl an Feinde in einer random Ort,  wo noch kein Hinderniss ist
    void plaziereMarioBlock(int anzahl){

        for (int i = 0; i < anzahl; i++) {

            int positionX = rand() % 100 + 0;

            while (levelMap[positionX][1] != 0) {

                positionX = rand() % 100 + 0;
            }

            levelMap[positionX][1] = 4;
        }
    }
    
    
    void print(){
        for (int i = 0; i < 100; i++ ){
            
            for (int j = 0; j < 100; j++ ){
                
                cout << levelMap[i][j];
            }   
        }   
    }

    // bewegt alle Rects nach links und löscht, die die schon zu weit sind
    void move(){

        //cout << "move" <<endl;

        //leere Liste sollte die Funktion gleich beenden
        if (strecke.empty()) {
            return;
        }

        int neuX = strecke.front()->getX();

        //geht durch die Liste und bewegt die Rechtecke
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
            
            toList(neuX, block);

            // Falls hindernis nicht mehr zusehen ist, Löschen wir es
            if (neuX <= -50) {
                
                delete (block);
                it = strecke.erase(it);
                //std::cout << "deletei" << endl;
            }
            else {
                
                it++;
            }
        }
    }
    
    
    //Speichert blöcke die Nah an der Spieler sind in einer separate Liste
    void toList(int x, Rect* block){
        
        if( x >= 80 && x <= 220 && find(bloeckeNah.begin(), bloeckeNah.end(), block) == bloeckeNah.end() ){
                
                bloeckeNah.push_back(block);
                cout <<"push: " << bloeckeNah.size() << endl ;
                
        }
            
        if(x < 80 ){

            if(find(bloeckeNah.begin(), bloeckeNah.end(), block) != bloeckeNah.end()){

                bloeckeNah.remove(block);
                cout <<"erase: " << bloeckeNah.size()<< endl ;
            }
        }
    }
    
    //geht die Liste durch und überprüft für kollisionen 
    bool kollidiert (int spielerY ){
         
       for (Rect* block : bloeckeNah){
           
           int blockY = block->getY();
           int blockX = block-> getX();
           int hoehe = block -> getHeight();
           int breite = block -> getWidth();
           
           int abstandX = abs(150-blockX);
           int abstandY = abs(spielerY-blockY);
           
           int grenzeY;
           int grenzeX;

           if(blockY < spielerY){
               
               grenzeY = hoehe;
                   
           }else{
               
               grenzeY = 80;
           } 
           
           if(blockX < 150){
               
               grenzeX = breite;
                   
           }else{
               
               grenzeX = 80;
           }
           
           if(abstandX <= grenzeX &&  abstandY <= grenzeY){
               
               return true;
               
           }
           
       }
        
       return false;    
    }   
    
    
    int woIstBoden (int spielerY ){
        
        int groessteY = 0;
        
        for (Rect* block : bloeckeNah){
            
            int blockY = block->getY();
            
            if(blockY < spielerY && blockY > groessteY){
                
                groessteY = blockY;
            
            }
        }
        
    return groessteY;  
        
    }
    
               

    //füngt ein Hindernis zu
    void add(int h){
        for(int i = 0; i < 14 ; i++){
        move();
        }
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        int height = rand() % 70 + 50;
        int y = 500 - height / 2 - h * 220;

        hindernis = new Rect(700, y, 70, height, &*view);
        hindernis->setFill(r, g, b);
        strecke.push_back(hindernis);
        
        }

    //schaut was auf die Karte ist
    int getValue(int x, int y){
        
        return levelMap[x][y];
    }
};

#endif