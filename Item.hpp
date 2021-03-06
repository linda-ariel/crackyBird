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
        int hoehe;
        int breite;
    
        //3 verschiedene Items
        int art;
 
        //"Standartbild"
        Image bild;
        // Bildwechsel bei Kollision mit Stein
        Image bildKollision;
        Image bildZerstoert;
            
        SVG *spielfeld;
        Item* item;
        
      
    public:  
    
        //Standartonstruktor
        Item(int xpos, int ypos, int hoehe, int breite, int art, SVG *spielfeld) {
   
            this->xpos = xpos;
            this->ypos = ypos;
            this->hoehe = hoehe;
            this->breite = breite;
            this->art = art;
            
            //für drei verschiede Items gibt es jeweils andere Bilder
            if (art == 0) {
                //stein
                bild = Image("fels.png", xpos, ypos, hoehe, breite, spielfeld);
                bildKollision = Image("grabstein.png", xpos, ypos, hoehe, breite, spielfeld);
                bildKollision.hide();
                bildZerstoert = Image("stein_zerbrochen.png", xpos, ypos, hoehe, breite, spielfeld);
                bildZerstoert.hide();

            } else if (art == 1) {
                //münze
                bild = Image("muenze.png", xpos, ypos, hoehe, breite, spielfeld);
                
            } else if (art == 2) {
                //feind
                bild = Image("feind.png",xpos, ypos, hoehe, breite, spielfeld);

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
    
        void setBild(Image bild) {
            this->bild = bild;
        }
    
        void moveTo(int xpos, int ypos) {
            this->xpos = xpos;
            this->ypos = ypos;
            bild.moveTo(xpos, ypos);
            bildKollision.moveTo(xpos, ypos);
            bildZerstoert.moveTo(xpos, ypos);
        }
    
        void setSize(int hoehe, int breite) {
            this->hoehe = hoehe;
            this->breite = breite;
            bild.setSize(hoehe, breite);
        }
        
    
        //Operationen für Kollision
    
        //Münze    
        void muenzeGesammelt() { 
            //Münze wird immer kleiner
            for(int size = 30; size > 5; size-- ) {
                bild.setSize(size,size);
            }
        }

        //Marioblock
        //anderes Bild erscheint
        void steinZerstoert() {
            bild.hide();
            bildZerstoert.show();
            bildZerstoert.moveTo(xpos,ypos);
        }
    
        //anderes Bild erscheint
        void steinKollidiert() {
            bild.hide();
            bildKollision.show();
            bildKollision.moveTo(xpos,ypos);
        }
    
        //Feind
        //wird kleiner und verschwindet
        void feindTot() {
            for (int zaehler = 0; zaehler < 50; zaehler++) {
                bild.setSize (breite + zaehler, hoehe - zaehler);
                bild.moveTo(bild.getX(), bild.getY() + 1);
            }
        }
    
        //entfernt "Standartbild" von SVG
        void löschen() {
            bild.removeFromView();
        }
    
        //nur für Stein
        void löscheStein() {
            bild.removeFromView();
            bildKollision.removeFromView();
            bildZerstoert.removeFromView();
        }    
    
};
#endif