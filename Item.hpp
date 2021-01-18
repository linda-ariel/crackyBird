#ifndef ITEM
#define ITEM

#include <algoviz/SVG.hpp>
#include <list>
#include <iostream>
using namespace std;

class Item {
    
    private:
        int xpos;
        int ypos;
        int art; //3 verschiedene
        int hoehe;
        int breite;
        Image bild;
        SVG *spielfeld;
        

    public:  
    
        //Standartonstruktor
        Item(int xpos, int ypos, int hoehe, int breite, int art, SVG *spielfeld) {
   
            this->xpos = xpos;
            this->ypos = ypos;
            this->hoehe = hoehe;
            this->breite = breite;
            this->art = art;
            
            if (art == 0) {
                //stein
                bild = Image("../img/Dungeon/Held.png", xpos, ypos, hoehe, breite, spielfeld);

            } else if (art == 1) {
                //münze
                bild = Image("../img/Dungeon/Held.png", xpos, ypos, hoehe, breite, spielfeld);
                
            } else if (art == 2) {
                //feind
                bild = Image("../img/Dungeon/Held.png",xpos, ypos, hoehe, breite, spielfeld);
            }
        }
    
        //getter
        int getXpos() {
            return xpos;
        }
    
        int getYpos() {
            return ypos;
        }
    
        int getHoehe() {
            return hoehe;
        }
    
        int getBreite() {
            return breite;
        }
    
        int getArt() {
            return art;
        }
    
        Image getBild() {
            return bild;
        }
    
    
        //setter

        void setXpos(int xpos) {
            this->xpos = xpos;
            bild.moveTo(xpos, bild.getY());
        }
            
        void setYpos(int ypos) {
            this->ypos = ypos;
            bild.moveTo(bild.getX(), ypos);
        }
    
        void moveTo(int xpos, int ypos) {
            this->xpos = xpos;
            this->ypos = ypos;
            bild.moveTo(xpos, ypos);
        }
    
    
        void setSize(int hoehe, int breite) {
            this->hoehe = hoehe;
            this->breite = breite;
            bild.setSize(hoehe, breite);
        }
        
    
        //operationen
    
        //Münze
    
        void muenzeGesammelt() { 
            //Münze wird immer kleiner
            for(int size = 30; size > 5; size-- ) {
                bild.setSize(size,size);
            }
            bild.removeFromView();
        }

    
        void steinZerstoert() {
            for (int zaehler = 0; zaehler < 50; zaehler++ ) {
                bild.moveTo(bild.getX(), bild.getY() -1 );
            }
            bild.removeFromView();
        }
            
        //Feind
        void feindTot() {
            for (int zaehler = 0; zaehler < 50; zaehler++) {
                bild.setSize (breite + zaehler, hoehe - zaehler);
                bild.moveTo(bild.getX(), bild.getY() + 1);
            }
        }
    
   
        void löschen() {
            bild.removeFromView();
        }
    
        void moveItems(list<Item*> items, int v) {
            auto it = items.begin();
            while (it != items.end()) {

                Item *item = *it;
                //lösche item, wenn es nicht mehr zu sehen ist
                if (item->getXpos() <= 50) {
                item->löschen();
                }
                
                item->moveTo(item->getXpos() - v, item->getYpos());
                it++;
            }
        }
};
#endif