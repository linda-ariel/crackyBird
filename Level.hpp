#include <algoviz/SVG.hpp>
#include <cstdlib>
#include <list>
#include <stdlib.h>     /* abs */
#include "Queue.hpp"

using namespace std;

#ifndef _LEVEL_
#define _LEVEL_

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
    
    
    //Karte für das level
    int karte[30][3];
    
    //alle Hindernisse nah an Protagonist
    list <Rect*> bloeckeNah;

    //Anzahl von Items
    int stein;
    int muenzeAnzahl;
    int feindAnzahl;

    //strecke
    list<Rect*> strecke;
    Rect* hindernis;

    // geschwindigkeit
    double v;
    
 
public:
    
    //konstruktor
    Level(int v, int muenzeAnzahl, int feindAnzahl,int stein, SVG *view){
        
        this->v = v;
        this->muenzeAnzahl = muenzeAnzahl;
        this->feindAnzahl = feindAnzahl;
        this->stein = stein;
        this->view = view;
        
        
        buildLevel(muenzeAnzahl, feindAnzahl, stein);
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
                karte[i][h] = random;
            }
        }
    }
    
    
    
    //matrix wird in Queues umgewandelt
    void toQueue(){
        
        
        srand (time(nullptr));
        
        for ( int i = 0; i < 30; i++ ) {
            
            
            int pos2 = karte[i][1];
            ebene2.enqueue(pos2);
            
            int pos3 = karte[i][2];
            ebene3.enqueue(pos3);
            
           int pos1;
            if ( pos2 == 1 && pos3 == 1 ) {
                
                pos1 = 0;
                ebene1.enqueue(pos1);
                
            } else {
                
                pos1 = karte[i][0];
                ebene1.enqueue(pos1);
            }
        }
    }

   //Plaziert eine beliebige Anzahl an muenze in einer random Ort,  wo noch kein Hindernisse sind + keine Items im selben Position X 
  void platziereMuenze(int anzahl){

        for (int i = 0; i < anzahl; i++) {
            
            int hoehe = rand() % 3;
            int positionX = rand() % 30;
            int zaehler = 0;

            while((karte[positionX][hoehe] != 0 || karte[positionX][0] > 1 || karte[positionX][1] > 1 || karte[positionX][2] > 1) && zaehler != 15 ){

                zaehler++;
                positionX = rand() % 30;
                int hoehe = rand() % 3;
            }

            if ( zaehler != 15 ) {
                karte[positionX][hoehe] = 2;
            }
        }
            
  }

    //Plaziert eine beliebige Anzahl an Feinde in einer random Ort,  wo noch kein Hinderniss ist
    void platziereFeind(int anzahl){

       for (int i = 0; i < anzahl; i++) {

            int positionX = rand() % 30 + 0;
            int zaehler = 0;

            while (karte[positionX][0] != 0 && zaehler != 15) {

                zaehler++;
                positionX = rand() % 30 + 0;
            }

            if ( zaehler != 15 ) {
                karte[positionX][0] = 3;
            }
        }
    }

    //Plaziert eine beliebige Anzahl an Stein in einer random Ort, wo noch kein Hindernisse sind + keine Items im selben Position X
    void platziereStein(int anzahl){

     for (int i = 0; i < anzahl; i++) {
            
            int positionX = rand() % 30;
            int zaehler = 0;

            while ((karte[positionX][1] != 0 || karte[positionX][0] > 1 || karte[positionX][2] > 1) && zaehler != 15 ) {
                
                zaehler++;
                positionX = rand() % 30;

            }

            if ( zaehler != 15 ) {
                karte[positionX][1] = 4;
            }
        } 
        
  }
    
    
    
    //Füllt die ganze Karte mit alle items und Hindernisse + macht alle Elemente in die Queues rein
    void buildLevel(int muenze, int feinde, int stein){
        
        fuellMap();
        platziereFeind(feinde);
        platziereMuenze(muenze);
        platziereStein(stein);
        toQueue();
    }
        
    

    // bewegt alle Rects nach links und löscht, die die schon zu weit sind
    void move(){

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
                
        }
            
        if(x < 10 ){

            if(find(bloeckeNah.begin(), bloeckeNah.end(), block) != bloeckeNah.end()){

                bloeckeNah.remove(block);

            }
        }
    }
    
    //geht die Liste durch und überprüft für kollisionen 
   int kollidiert (int spielerY, int bildwert){
         
       for (Rect* block : bloeckeNah){
           
           int blockY = block->getY();
           int blockX = block-> getX();
           int hoehe = block -> getHeight();
           int breite = block -> getWidth();
           
           int abstandX = abs(150-blockX);                //Abstand x-Koordinate Spieler und X-Koordinate Block
           int abstandY = abs(spielerY-blockY);           //Abstand y-Koordinate Spieler und Y-Koordinate Block
           
           int grenzeY;
           int grenzeX;

           if ( bildwert != 2 ) {
               
               if ( blockY < spielerY ) {
                   
                   grenzeY = hoehe + 40;
                  
               } else {
                   
                   grenzeY = 40;
                   
               }
               
               if ( blockX < 150 ) {
                   
                   grenzeX = breite + 40;
                  
               } else {
                   
                   grenzeX = 40;
                   
               }
               
           } else {
            
               if ( blockY < spielerY ) {
                   
                   grenzeY = hoehe + 25;
                  
               } else {
                   
                   grenzeY = 25;
                   
               }
               
               if ( blockX < 150 ) {
                   
                   grenzeX = breite + 25;
                  
               } else {
                   
                   grenzeX = 25;
                   
               }
             
           }

           if (abstandX <= grenzeX && abstandY <= grenzeY) {
               
               return blockY;
               
           }
           
       }
        
       return 600;    
    } 
    
    
    
    //Spieler kann über die Hindernisse laufen
    int woIstBoden (int spielerY ){
        
        int groessteY = 490;
        
        for (Rect* block : bloeckeNah){
            
            int blockY = block->getY();
            
            if(blockY > spielerY + 45 && blockY < groessteY){
                
                groessteY = blockY;
            
            }
        }
        
    return groessteY;  
        
    }
    
    //gibt erste Wert von ebene1 zurück
    int getEbene1() {
        return ebene1.dequeue();
    }
    
    //gibt erste Wert von ebene2 zurück
    int getEbene2() {
        return ebene2.dequeue();
    }
    
    //gibt erste Wert von ebene3 zurück
    int getEbene3() {
        return ebene3.dequeue();
    }
               

    //fügt ein Hindernis zu
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

   
};

#endif