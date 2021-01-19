#include <algoviz/SVG.hpp>
#include <cstdlib>
#include <list>
#include <stdlib.h>     /* abs */
#include "Queue.hpp"

using namespace std;

#ifndef _LEVEL2_
#define _LEVEL2_

class Level {

private:
    SVG *view;

    // 0 = leer   1 = hindernisse   2 = Münze   3 = Feind   4 =  Mario Block
    Queue<int> ebene1;
    Queue<int> ebene2;
    Queue<int> ebene3;

    //Verwaltung von Hindernisse
    int hindernisAnzahl;
    list<int> hindernisPosition;
    
    int levelMap[30][3];
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
    Level(int v, int muenzeAnzahl, int feindAnzahl, SVG *view){
        
        this->v = v;
        this->muenzeAnzahl = muenzeAnzahl;
        this->feindAnzahl = feindAnzahl;
        //this->marioBlockAnzahl = marioBlockAnzahl;
        this->view = view;
        
        fuellMap();
        platziereMuenze(muenzeAnzahl);
        platziereFeind(feindAnzahl);
       // platziereMarioBlock(marioBlockAnzahl);
        toQueue();
    }

    // getter
    double getV() { return v; }

    // setter
    void setV(double v) { this->v = v; }

    
    
        //Plaziert die Hindernisse
    void fuellMap(){

        //Iterator für die länge
        for (int i = 0; i < 30; i++) {
            
            //Iterator für die Höhe
            for (int h = 0; h < 3; h++) {

            
                int random = rand() % 2;
                levelMap[i][h] = random;
            }
        }
    }
    
    
    
    //Plaziert die Hindernisse
    void toQueue(){
        
        
        srand (time(nullptr));
        
        for ( int i = 0; i < 30; i++ ) {
            
            
            int pos2 = levelMap[i][1];
            ebene2.enqueue(pos2);
            
            int pos3 = levelMap[i][2];
            ebene3.enqueue(pos3);
            
           
            if ( pos2 == 1 && pos3 == 1 ) {
                
                ebene1.enqueue(0);
                
            } else {
                
                int pos1 = levelMap[i][0];
                ebene1.enqueue(pos1);
            }
        }
    }

    //Plaziert eine beliebige Anzahl an muenze in einer rand()om Ort,  wo noch kein Hinderniss ist
  void platziereMuenze(int anzahl){

        for (int i = 0; i < anzahl; i++) {

            int hoehe = rand() % 2 + 0;
            int positionX = rand() % 30 + 0;

            while (levelMap[positionX][hoehe] != 0) {

                positionX = rand() % 30;
                hoehe = rand() % 2 + 0;
            }

            levelMap[positionX][hoehe] = 2;
        }
    }

    //Plaziert eine beliebige Anzahl an Feinde in einer random Ort,  wo noch kein Hinderniss ist
    void platziereFeind(int anzahl){

        for (int i = 0; i < anzahl; i++) {

            int positionX = rand() % 30;

            while (levelMap[positionX][0] != 0) {

                positionX = rand() %30;
            }

            levelMap[positionX][0] = 3;
        }
    }

    //Plaziert eine beliebige Anzahl an Feinde in einer random Ort,  wo noch kein Hinderniss ist
    void platziereMarioBlock(int anzahl){

        for (int i = 0; i < anzahl; i++) {

            int positionX = rand() % 30;

            while (levelMap[positionX][1] != 0) {

                positionX = rand() % 30;
            }

            levelMap[positionX][1] = 4;
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
        
        if( x >= 10 && x <= 190 && find(bloeckeNah.begin(), bloeckeNah.end(), block) == bloeckeNah.end() ){
                
                bloeckeNah.push_back(block);
                cout <<"push: " << bloeckeNah.size() << endl ;
                
        }
            
        if(x < 10 ){

            if(find(bloeckeNah.begin(), bloeckeNah.end(), block) != bloeckeNah.end()){

                bloeckeNah.remove(block);
                cout <<"erase: " << bloeckeNah.size()<< endl ;
            }
        }
    }
    
    //geht die Liste durch und überprüft für kollisionen 
    int kollidiert (int spielerY ){
         
       for (Rect* block : bloeckeNah){
           
           int blockY = block->getY();
           int blockX = block-> getX();
           int hoehe = block -> getHeight();
           int breite = block -> getWidth();
           
           int abstandX = abs(150-blockX);                //Abstand x-Koordinate Spieler und X-Koordinate Block
           int abstandY = abs(spielerY-blockY);           //Abstand y-Koordinate Spieler und Y-Koordinate Block
           
           int grenzeY;
           int grenzeX;

           if( blockY < spielerY ){
               
               grenzeY = hoehe;
                   
           }else{
               
               grenzeY = 40;
           } 
           
           if(blockX < 150){
               
               grenzeX = breite;
                   
           }else{
               
               grenzeX = 40;
           }
           
           if(abstandX <= grenzeX && abstandY <= grenzeY){
               
               return blockY;
               
           }
           
       }
        
       return 600;    
    }   
    
    
    int woIstBoden (int spielerY ){
        
        int groessteY = 465;
        
        for (Rect* block : bloeckeNah){
            
            int blockY = block->getY();
            
            if(blockY < spielerY && blockY > groessteY){
                
                groessteY = blockY;
            
            }
        }
        
    return groessteY;  
        
    }
    
    int getEbene1() {
        return ebene1.dequeue();
    }
    
    int getEbene2() {
        return ebene2.dequeue();
    }
    
    int getEbene3() {
        return ebene3.dequeue();
    }
               

    //füngt ein Hindernis zu
    void add(int h){

        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        int height = rand() % 70 + 50;
        int y = 500 - height / 2 - h * 150;
        int x = 700 + ( rand() % ( 800 - 700 + 1 ) );

        hindernis = new Rect(x, y, 100, height, &*view);
        hindernis->setFill(r, g, b);
        strecke.push_back(hindernis);
        
        }

    //schaut was auf die Karte ist
    /*int getValue(int x, int y){
        
        return levelMap[x][y];
    }*/
};

#endif