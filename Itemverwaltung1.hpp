#ifndef _ITEMVERWALTUNG_
#define _ITEMVERWALTUNG_

#include <algoviz/SVG.hpp>
#include <list>
#include <iostream>
#include"Item.hpp"
using namespace std;



class Itemverwaltung {
    
    private:
        
        Item *item;
        SVG *spielfeld;
        
        //Liste in der alle Items gespeichert werden
        list<Item*> items;
    
        //Liste in der Items gespeichert werden, die nah an Protagonisten sind
        list<Item*> naheItems;
    
    public:
    
        Itemverwaltung(SVG *spielfeld) {
            this->spielfeld = spielfeld;
        }

        //füge Item dem Spielfeld hinzu
        void addItem(int hoehe, int art) {
            
            //stein
            if (art == 0) {
                int ypos = 405 - hoehe * rand() % 101;
                item = new Item(700, ypos,30,30, 0, spielfeld);
                items.push_back(item);
                
            //muenze    
            } else if (art == 1) {
                int ypos = 400 - hoehe * rand() % 100;
                item = new Item(700, ypos, 20, 20, 1, spielfeld);
                items.push_back(item); 
                
            //feind
            } else if (art == 2) {
                item = new Item(300, 370, 60, 100, 2, spielfeld);
                items.push_back(item);    
            }
        }
    
        //Bewegt jedes Item, welches in der Liste "items" gespeichert ist
        void moveItems(int v) {
            
            auto it = items.begin();
            while (it != items.end()) {
                
                Item *item = *it;  
                
                int neuX = item->getXpos()-v;                
                item->moveTo(neuX, item->getYpos());
                
                //überprüfen, ob es nah an Protagonist ist und ggf zu Liste hinzufügen
                toList(neuX, item);
                
                //lösche Item aus SVG und Liste, wenn es nicht mehr zu sehen ist
                if (neuX <= -30) {
                    item->löschen();
                    it = items.erase(it);
                    
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
        int kollision(int protagonistY) {
            for (Item *item : naheItems) {
                 
                int abstandX = abs(item->getXpos() - 150); //steht protagonist auf 150?
                int abstandY = abs(item->getYpos() - protagonistY);
                
                //  1 = setLeben(+), 2 = setPunkte(+), 0 = keine Kollison
                // -1 = setLeben(-), -2 = setPunkte(-), neuerBoden = setY(neuerBoden) 
                
                //mit Münze
                if (item->getArt() == 1) {
                    if (abstandX <= 50 && abstandY <= 50) {
                        item->muenzeGesammelt();
                        items.remove(item);
                        return 2;                        
                    }
                }

                    
                //mit Feind
                if (item->getArt() == 2) {                    
                    //wenn Protagonist Feind besiegt (auf ihn springt)            in welchen schritten springt spieler? 80 ->10er schritte? 
                    if (item->getYpos() > protagonistY && abstandX <= 70 && abstandY <= 90 && abstandY >= 80) {
                        item->feindTot();
                        items.remove(item);
                        return 1;
                        
                    //wenn Protagonist gegen Feind läuft
                    } else if (abstandX <= 70 && abstandY <= 90) {  
                        return -1;
                    }
                }

                    
                //mit Stein
                if (item->getArt() == 0) {
                    //wenn Protagonist auf Stein springt                                        //10er Schritte?
                    if (item->getYpos() < protagonistY && abstandX <= 45 && abstandY <= 65 && abstandY >= 55) {
                        int neuerBoden = item->getYpos();
                        return neuerBoden;
                        
                    //wenn Protagonist auf von unten gegen Stein springt -> bringt 3 Punkte     //10er Schritte
                    } else if (item->getYpos() > protagonistY && abstandX <= 45 && abstandY <= 65 && abstandY >= 55) {
                        item->steinZerstoert();
                        items.remove(item);
                        return 2;
                        
                    //wenn Protagonist gegen Stein läuft  //maxV von Items == 5?
                    }else if (abstandX <= 45 && abstandX >= 40 && abstandY <= 50 ) {
                        return -2;                        
                    }
                }     
            }            
            return 0;
        }     
    
};

#endif
