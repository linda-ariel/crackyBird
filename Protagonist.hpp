#include <iostream>
#include <algoviz/SVG.hpp>

using namespace std;


#ifndef _PROTAGONIST_
#define _PROTAGONIST_

class Protagonist {
    
    private:
    
        // Zeiger auf SVG und die Anzeigen im SVG (Punktestand, Leben)
        SVG *spielfeld;
        Text *anzeigeP;
        Text *anzeigeL;
        
        int leben;
        int punkte;
    
        // Bilder des Protagonisten
        Image bild[4][2];
    
        int bildwert1;
        int bildwert2;

        // Supersprung
        bool supersprungB;
        Image blitz;

        // nächstes Hindernis bzw. Boden unter dem Spieler (y-Koordinate)
        int yOben;  

        int x;
        int y;
        int geschwindigkeit;
    
        // Angezeigte "Unverwundbarkeit" nach Verlust eines Lebens
        bool blinken;
        bool gezeigt;
        
    public: 
        
        //Konstruktor
        Protagonist(SVG *spielfeld, Text *anzeigeP, Text *anzeigeL, int geschwindigkeit) {
            this->spielfeld = spielfeld;
            this->anzeigeP = anzeigeP;
            this->anzeigeL = anzeigeL;
            this->geschwindigkeit = geschwindigkeit;
            supersprungB = false;
            bildwert1 = 0;
            bildwert2 = 0;
            leben = 3;
            punkte = 0;
            x = 150;
            y = 440;
            yOben = 490;
            blinken = false;
            gezeigt = true;
            addBild();
        }
    
        void addBild() {
            
            //Bilder beim Laufen
            bild[0][0] = Image("laufen1.png",x,y,80,80,spielfeld);
            bild[0][1] = Image("laufen2.png",x,y,80,80,spielfeld);
            bild[0][1].hide();
            
            // Bilder im (Super-)Sprung
            bild[1][0] = Image("sprung.png",x,y,80,80,spielfeld);
            bild[1][0].hide();
            bild[1][1] = Image("superSprung.png",x,y,80,80,spielfeld);
            bild[1][1].hide();
            
            // Bild beim "Kriechen"
            bild[2][0] = Image("kriechen.png",x,y,50,50,spielfeld);
            bild[2][0].hide();
            
            // Bild im Zerstörer-Modus
            bild[3][0] = Image("zerstoerer.png",x,y,80,80,spielfeld);
            bild[3][0].hide();
            
            // dynamisches Bild unter Protagonisten beim Supersprung
            blitz = Image("boom.png",100,455,50,50,spielfeld);
            blitz.hide();
        }
    
        
        // passiver Bildwechsel (wird nicht vom Nutzer beeinflusst, läuft über Zähler)
        void switchBildpassiv() {                                  
                
                bild[bildwert1][bildwert2].show();
            
                // Protagonist im "ersten" Laufbild -> Protagonist im "zweiten" Laufbild
                if ( bildwert1 == 0 && bildwert2 == 0 ) {
                    bildwert2 = 1;
                    bild[0][0].hide();
                    bild[0][1].show();
                    bild[0][1].moveTo(x,y);
                    
                // Protagonist im "zweiten" Laufbild -> Protagonist im "ersten" Laufbild
                } else if ( bildwert1 == 0 && bildwert2 == 1 ) {
                    bildwert2 = 0;
                    bild[0][1].hide();
                    bild[0][0].show();
                    bild[0][0].moveTo(x,y);
                    
                // Protagonist im Sprungbild -> Protagonist im "ersten" Laufbild
                } else if ( bildwert1 == 1 && y >= yOben - 50 ) {
                    bild[1][bildwert2].hide();
                    bildwert1 = 0;
                    bildwert2 = 0;
                    bild[0][0].show();
                    bild[0][0].moveTo(x,y);
                }
                blitz.hide();
            
            // nach Kollision mit Hindernis/Feind bzw. Verlust eines Lebens
            if ( blinken == true ) {
                if ( gezeigt == true ) {
                    bild[bildwert1][bildwert2].hide();
                    gezeigt = false;
                } else {
                    bild[bildwert1][bildwert2].show();
                    gezeigt = true;
                }
                blitz.hide();
            }   
                
        }
    
    
        // aktiver Bildwechsel (vom Nutzer gesteuert)
        void switchBildaktiv(string taste) {
            
            if ( taste == "ArrowUp" ) {
                
                // Von laufen zu springen
                if ( bildwert1 == 0 ) {
                    bild[0][bildwert2].hide();
                    bildwert1 = 1;
                    bildwert2 = 0;
                    bild[1][0].show();
                    bild[1][0].moveTo(x,y);
                
                // Von kriechen zu laufen
                } else if ( bildwert1 == 2 ) {
                    bild[2][bildwert2].hide();
                    bildwert1 = 0;
                    bildwert2 = 0;
                    bild[0][0].show();
                    bild[0][0].moveTo(x,y);
                }
            } else if ( taste == "Shift" ) {
                
                // Von laufen zu Supersprung
                if ( bildwert1 == 0 ) {
                    bild[0][bildwert2].hide();
                    bildwert1 = 1;
                    bildwert2 = 1;
                    bild[1][1].show();
                    bild[1][1].moveTo(x,y);
                }
            } else if ( taste == "ArrowDown" ) {
                
                // Von laufen zu kriechen
                if ( bildwert1 == 0 ) {
                    bild[0][bildwert2].hide();
                    bildwert1 = 2;
                    bildwert2 = 0;
                    bild[2][0].show();
                    bild[2][0].moveTo(x,y);
                }
            
            // in den "Zerstörer-Modus"
            } else if ( taste == "fight" ) {
                bild[bildwert1][bildwert2].hide();
                bildwert1 = 3;
                bildwert2 = 0;
                bild[3][0].show();
                bild[3][0].moveTo(x,y);
                
            // Von "Zerstörer-Modus" zu laufen
            } else if ( taste == "OutOfFight" ) {
                bild[3][0].hide();
                bildwert1 = 1;
                bild[1][0].show();
                bild[1][0].moveTo(x,y);
            }
        }
    
    
        void springenhoch() {
            
            // Bildwechsel, wenn kein Sprungbild oder "Zerstörer"-Bild aktuell ist
            if ( bildwert1 != 1 && bildwert1 != 3 ) {
                switchBildaktiv("ArrowUp");
            }

            // Verschiebung der y-Koordinate in Abhängigkeit der Art des Sprunges
            // ... kleiner Sprung oder Supersprung
            if ( supersprungB == false ) {
                y = y - geschwindigkeit - 1;
            } else {
                y = y - geschwindigkeit - 4;
            }
            bild[bildwert1][bildwert2].moveTo(x,y);
        }

    
        void springenrunter() {
            
            // Bildwechsel, wenn kein Sprungbild oder "Zerstörer"-Bild aktuell ist
            if ( bildwert1 != 1 && bildwert1 != 3 ) {
                switchBildaktiv("ArrowUP");
            }
            
            // Verschiebung der y-Koordinate
            y = y + geschwindigkeit + 2;
            bild[bildwert1][bildwert2].moveTo(x,y);
        }
        
    
        void moveTo( int y ) {
            this->y = y;
            bild[bildwert1][bildwert2].moveTo(x,y);
        }

    
        // Aufruf, wenn der Supersprung vom Nutzer ausgelöst wurde
        void supersprung() {
            switchBildaktiv("Shift");
            blitz.moveTo(x,y + 15);
            blitz.show();
            supersprungB = true;
        }

        // Aufruf, wenn der Supersprung beendet ist
        void setSupersprung(bool supersprungB) {
            this->supersprungB = supersprungB;
        }

        // Die Getter
    
        int getY() {
            return y;
        }
    
        int getLeben() {
            return leben;
        }
    
        int getPunkte() {
            return punkte;
        }
    
        int getBildwert() {
            return bildwert1;
        }
    
        
        // Die Setter
    
        void setYoben( int yOben ) {
            this->yOben = yOben;
        }
    
        void setV( int geschwindigkeit ) {
            this-> geschwindigkeit = geschwindigkeit;
        }
                            
        void setLeben(int plusminuseins) {
            leben = leben + plusminuseins;
            
            // Anpassung der Anzeige im Spielfeld
            anzeigeL->setText("Leben: " + to_string(leben));
        }
        
        void setPunkte(int punkte) {
            this->punkte = this->punkte + punkte;
            if ( this->punkte < 0 ) {
                this->punkte = 0;
            }
            
            // Anpassung der Anzeige im Spielfeld
            anzeigeP->setText("ECTS: " + to_string(this->punkte));
        }

        void setBlinken(bool blinken) {
            this->blinken = blinken;
        }
} ;

#endif
            