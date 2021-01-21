#ifndef _ITEMVERWALTUNG_
#define _ITEMVERWALTUNG_

#include <algoviz/SVG.hpp>
#include <list>
#include <iostream>
#include "Item.hpp"

using namespace std;



class Itemverwaltung {
    
    private:
        
        Item *item;
        SVG *spielfeld;
        
        //Liste in der alle Items gespeichert werden
        list<Item*> items;
    
        //Liste in der Items gespeichert werden, die nah an Protagonisten sind
        list<Item*> naheItems;
    
        //Geschidigkeit
        int v;
    
    public:
    
        Itemverwaltung(int v, SVG *spielfeld) {
            this->spielfeld = spielfeld;
            this->v = v;
        }
        
        //getter
        int getV() {
            return v;
        }
    
        //setter
        void setV(int v){
            this->v = v;
        }
    
        //füge Item dem Spielfeld hinzu
        void addItem(int hoehe, int art) {
            
            //stein
            if (art == 0) {
                
                int ypos = 440 - hoehe * 150;
                //int ypos = 400 - hoehe * 100 - rand() % 100 ;
                item = new Item(700, ypos, 80, 80, 0, spielfeld);
                items.push_back(item);
                
            //muenze    
            } else if (art == 1) {
                
                int ypos = 440 - hoehe * 120;
                //int ypos = 400 - hoehe * 100 - rand() % 100 ;
                item = new Item(700, ypos, 80, 80, 1, spielfeld);
                items.push_back(item); 
                
            //feind
            } else if (art == 2) {
                item = new Item(700, 440, 80, 80, 2, spielfeld);
                items.push_back(item);    
            }
        }
    
        //Bewegt jedes Item, welches in der Liste "items" gespeichert ist
        void moveItems() {
            
            if (items.empty()) {
                return;
            }
            
            auto it = items.begin();
            while (it != items.end()) {
                
                Item *item = *it;  
                
                int neuX = item->getXpos() - v; 
                item->moveTo(neuX, item->getYpos());

                //überprüfen, ob es nah an Protagonist ist und ggf zu Liste hinzufügen
                toList(neuX, item);
                
                //lösche Item aus SVG und Liste, wenn es nicht mehr zu sehen ist
                if (neuX <= -30) {
                    /*if (item->getArt() == 0) {
                        item->löscheStein();
                    } else {
                        item->löschen();
                    }*/
                    
                    it = items.erase(it);
                    delete (item);
                    
                } else {
                    it++;    
                } 
            }
        }
    
    
        //Speichert Items, die nah an Spieler sind in einer separaten Liste "naheItems"
        void toList(int x, Item* item){
                                       //damit kein Item mehrmals gespeichert wird
            if( x >= 80 && x <= 220 && find(naheItems.begin(), naheItems.end(), item) == naheItems.end() ){
                    naheItems.push_back(item);
            }

            if(x < 80 ){
                if(find(naheItems.begin(), naheItems.end(), item) != naheItems.end()){
                     naheItems.remove(item);
                }
            }
        }
    
        
        //Kollisionsüberprüfung
        int kollision(int protagonistY, int protagonistBildwert) {

            if (naheItems.empty()) {
                return 0;
            }
            
            for (Item *item : naheItems) {
    
                int abstandX = abs(item->getXpos() - 150);
                int abstandY = abs(item->getYpos() - protagonistY);
                
                int grenzeY = 80;
                int grenzeX = 80;
                
               if ( protagonistBildwert == 2 ) {
                   
                   grenzeY = 65;
                   grenzeX = 65;
             
               }

                //  1 = setLeben(+1), 2 = setPunkte(+1), 3 = setPunkte(+3), 0 = keine Kollison
                // -1 = setLeben(-1), -3 = setPunkte(-3)
                
                //mit Münze
                if (item->getArt() == 1) {

                    if (abstandX <= grenzeX && abstandY <= grenzeY) {
                        item->muenzeGesammelt();
                        items.remove(item);
                        delete (item);
                        return 2;                        
                    }

                }
                    
                //mit Feind
                if (item->getArt() == 2) {                    
                    //wenn Protagonist Feind besiegt
                    if (protagonistBildwert == 3 && abstandX <= grenzeX && abstandY <= grenzeY ) {
                        item->feindTot();
                        items.remove(item);
                        delete (item);
                        return 1;
                        
                    //wenn Protagonist gegen Feind läuft
                    } else if (protagonistBildwert != 3 && abstandX <= grenzeX && abstandY <= grenzeY ) {
                        return -1;
                    }
                }
                    
                //mit Stein
                if (item->getArt() == 0) {
  
                    //wenn Protagonist mit Stein kollidiert
                    if (protagonistBildwert != 3 && abstandX <= grenzeX && abstandY <= grenzeY ) {
                        item->steinKollidiert();
                        return -3;                                           
                    //wenn Protagonist Stein "bekämpft"
                    } else if (protagonistBildwert == 3 && abstandX <= grenzeX && abstandY <= grenzeY ) {
                        item->steinZerstoert();
                    cout << "FIGHT" << endl;
                        return 3;
                    }
                }
            } return 0;
        }
               
    
};

#endif