#ifndef _ITEMVERWALTUNG_
#define _ITEMVERWALTUNG_

#include <algoviz/SVG.hpp>
#include <list>
#include <iostream>
using namespace std;

#include"Item.hpp"

class Itemverwaltung {
    
  private:
    //Liste in der alle Items gespeichert werden
        list<Item*> items;
        Item *item;
        SVG *spielfeld;
        //Liste in der Items gespeichert werden, die nah an Protagonisten sind
        list<Item*> naheItems;
 public:
    
    Itemverwaltung(SVG *spielfeld) {
        this->spielfeld = spielfeld;
        
    }
    
//füge Item dem Spielfeld hinzu
    
        void addItem(int hoehe, int art) {
            
            //for(int i = 0; i < 14 ; i++){ ??
                //stein
                if (art == 0) {
                    int ypos = 405 - hoehe * rand() % 101;
                    item = new Item(700, ypos, 0, spielfeld);
                    items.push_back(item);
                //muenze    
                } else if (art == 1) {
                    int ypos = 400 - hoehe * rand() % 10;
                    item = new Item(700, ypos, 1, spielfeld);
                    items.push_back(item); 
                //feind
                } else if (art == 2) {
                    item = new Item(700, 370, 2, spielfeld);
                    items.push_back(item);    
                }

            //}
            
        }
    
        //Bewegt jedes Item, welches in der Liste "items" gespeichert ist
        void moveItems(int v) {
            auto it = items.begin();
            while (it != items.end()) {
                
                int neuX = item->getXpos()-v;
                
                Item *item = *it;
                item->moveTo(neuX, item->getYpos());
                
                //überprüfen, ob es nah an Protagonist ist
                toList(neuX, item);
                
                //lösche Item aus SVG und Liste, wenn es nicht mehr zu sehen ist
                if (item->getXpos() <= 50) {
                    item->löschen();
                    items.erase(it);
                    
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
    
};

#endif