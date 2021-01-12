#include <iostream>
#include <algoviz/SVG.hpp>
#include "Hindernis.hpp"

using namespace std;


#ifndef _PROTAGONIST_
#define _PROTAGONIST_

class Protagonist {
    
    private:
    
        SVG *spielfeld;
        Image bild[3][2];              //je nach Bilderanzahl...1.Dimension: laufen, springen, kriechen
        Image tot;
        Image blitz;
        bool supersprungB;
        int bildwert1;                 //versch. Bilder bekommen einen "Bildwert"
        int bildwert2;
        int leben;                     //sagt wie viele Leben der Protagonist "noch" hat
        int punkte;
        int x;                         //Position: x-Koordinate
        double y;                      //Position: y-Koordinate
        Hindernis *hindernis;          // Zeiger auf Hindernis, dass aktuell bei x = 300 steht
        
    public: 
        
        //Konstruktor
        Protagonist(SVG *spielfeld) {
            this->spielfeld = spielfeld;
            supersprungB = false;
            bildwert1 = 1;
            bildwert2 = 0;
            leben = 3;
            punkte = 0;
            x = 300;
            y = 420;
            hindernis = nullptr;
            addBild();
        }
    
        void addBild() {
            bild[0][0] = Image("../img/Dungeon/Held.png",x,y,80,80,spielfeld);          // [0][0] und [0][1]: Bilder des Protagonisten beim Laufen
            bild[0][1] = Image("../img/Dungeon/Skelett.png",x,y,80,80,spielfeld);
            bild[0][1].hide();
            bild[1][0] = Image("../img/Dungeon/Zombie.png",x,y,80,80,spielfeld);           // [1][0]: Bild des Protagonisten beim Sprung
            bild[1][0].hide();
            bild[1][1] = Image("../img/Dungeon/Held.png",x,y,80,80,spielfeld);
            bild[1][1].hide();
            bild[2][0] = Image("../img/Tardis.png",x,y,80,80,spielfeld);
            bild[2][0].hide();                                    //[...]
            bild[2][1] = Image("../img/Kreise.png",x,y,80,80,spielfeld);
            bild[2][1].hide();
            tot = Image("../img/Dungeon/Skelett.png",300,250,150,150,spielfeld);            // Protagonist in tot, wenn "Game over"
            tot.hide();
            blitz = Image("../img/snake.png",x,320,70,30,spielfeld);
            blitz.hide();
        }
    
       
          //Folgendes aktives/passives Bildaustauschkonzept: das Vorwärtslaufen d. Protagonisten ist ein rein passiver Prozess, d.h. nicht 
          //beeinflussbar vom Nutzer; das Springen ist ein passiver sowie aktiver Prozess, da der Nutzer den Beginn des Sprunges über die 
          //Pfeiltaste nach oben bestimmt, dann jedoch den Prozess bis der Protagonist wieder löuft nicht beeinflussen kann; das 
          //Ducken/Kriechen (Pfeiltaste nach unten) ist ein aktiver sowie passiver Prozess, d.h. der Nutzer kann/muss selbst enscheiden, 
          //wann sich der Protagonist wieder aufstellt (Pfeiltaste nach oben), um einem Hindernis (von oben) auszuweichen -> unterdessen 
          //kriecht der Protagonist jedoch die ganze Zeit (passiv)
    
        void switchBildpassiv() {                                  // passiv: abwechselndes Laufen ; nach Sprung wieder beginnen zu laufen ;
            if ( bildwert1 == 0 && bildwert2 == 0 ) {              // abwechselndes Kriechen
                bildwert2 = 1;                                     // -> passiver Bildwechsel muss in jedem Schleifendurchlauf stattfinden
                bild[0][0].hide();
                bild[0][1].show();
            } else if ( bildwert1 == 0 && bildwert2 == 1 ) {
                bildwert2 = 0;
                bild[0][1].hide();
                bild[0][0].show();
            } else if ( bildwert1 == 1 && y >= getHindernisY() - 40 ) {
                bildwert1 = 0;
                if ( bildwert2 == 1 ){
                    bildwert2 = 0;
                    bild[1][1].hide();
                } else {
                    bild[1][0].hide();
                }
                bild[0][0].show();
            } else if ( bildwert1 == 2 && bildwert2 == 0 ) {
                bildwert2 = 1;
                bild[2][0].hide();
                bild[2][1].show();
            } else if ( bildwert1 == 2 && bildwert2 == 1 ) {
                bildwert2 = 0;
                bild[2][1].hide();
                bild[2][0].show();
            }
            blitz.hide();
        }
    
        void switchBildaktiv(string taste) {                      // Durch den Nutzer gesteuert: Die zwei Pfeiltasten (oben, unten) kriegen
            if ( taste == "ArrowUp" ) {                           // Bildwerte (oben:1, unten:2)
                if ( bildwert1 == 0 ) {                           //von laufen zu springen... (Pfeiltaste hoch)
                    bildwert1 = 1;
                    if ( bildwert2 == 1 ) {
                        bildwert2 = 0;
                        bild[0][1].hide();
                    } else {
                        bild[0][0].hide();
                    }
                    bild[1][0].show();
                } else if ( bildwert1 == 1 ) {
                    bildwert2 = 1;
                    bild[1][0].hide();
                    bild[1][1].moveTo(x,y);
                    bild[1][1].show();
                    bild[1][0].moveTo(x,420);
                } else if ( bildwert1 == 2 ) {                    // von kriechen zu laufen... (Pfeiltaste hoch)
                    bildwert1 = 1;
                    if ( bildwert2 == 1 ) {
                        bildwert2 = 0;
                        bild[2][1].hide();
                    } else {
                        bild[2][0].hide();
                    }
                    bild[0][0].show();
                }
            } else if ( taste == "ArrowDown" ) {                   // von laufen zu kriechen... (Pfeiltaste runter)
                if ( bildwert1 == 0 ) {
                    bildwert1 = 2;
                    if ( bildwert2 == 1 ) {
                        bildwert2 = 0;
                        bild[0][1].hide();
                    } else {
                        bild[0][0].hide();
                    }
                    bild[2][0].show();
                }
            }
        }
    
        void springenhoch() {                                     // aufgeteilt in hoch und runter, um erfassen zu können, ob jemand        
            if ( bildwert1 != 1 ) {
                switchBildaktiv("ArrowUp");                       //..zwischendurch nochmal gedrückt hat
            }
            y = y - 0.5;                                          // Differenz, die der Protagonist weiter nach oben springt
            if ( supersprungB == false ) {
                bild[1][0].moveTo(x,y);                           // [Hindernisse sollten oben einen freien Platz von mindestens 160 lassen]
            } else {
                bild[1][1].moveTo(x,y);
            }                                                     // ...,da: eine Sprungebene = 120 -> Sprungebene ausgehend vom Mittelpunkt
        }                                                         // ... M des Protagonisten, y-K. von M = Höhe des Hindernisses + 40
        
        void springenrunter() {                                   // (-40), da der Protagonist (80,80) groß ist
            if ( bildwert1 != 1 ) {
                switchBildaktiv("ArrowUP");                       // falls der Protagonist gerade von einem Hindernis runterkommt...
            }
            y = y + 1;                                            // ...(auf dem er GELAUFEN ist)
            if ( supersprungB == false ) {
                bild[1][0].moveTo(x,y);
            } else {
                bild[1][1].moveTo(x,y);
            }
        }
    
        void supersprung() {
            switchBildaktiv("ArrowUp");
            blitz.show();
            supersprungB = true;
        }
    
        void setSupersprung(bool supersprungB) {
            this->supersprungB = supersprungB;
        }
        
        int getX() {
            return x;
        }
                    
        int getY() {
            return y;
        }
    
        int getLeben() {
            return leben;
        }
    
        int getBildwert() {
            return bildwert1;
        }
        
        int getHindernisY() {
            if ( hindernis != nullptr ) {                                    // wenn gerade ein Hindernis bei x=300 steht
                return hindernis->getYoben();                                // Operation in Hindernis benötigt, die die y-Koordinate der...
            }                                                                // ... oberen Kante des Hinternisses zurückgibt.
            return 460;                                                      // wenn kein Hindernis bei x=300 steht, so ist das "Hindernis"
        }                                                                    // so hoch wie der Boden (460-40, siehe "springenrunter()")
        
        void setHindernis(Hindernis *hindernis) {
            this->hindernis = hindernis;
        }
                            
        void setLeben(Text *anzeigeL, int plusminuseins) {
            leben = leben + plusminuseins;
            anzeigeL->setText("Leben: " + to_string(leben));
            if ( leben <= 0 ) {
                gameOver();
            }
        }
        
        void setPunkte(Text *anzeigeP, int punkte) {
            this->punkte = this->punkte + punkte;
            anzeigeP->setText("Punkte: " + to_string(punkte));
        }
        
        void gameOver() {
            for ( int i = 0; i < 3; i++ ) {
                for ( int j = 0; j < 2; j++ ) {
                    bild[i][j].hide();
                }
            }
            tot.show();
            spielfeld->drawText("Game over", 180, 210, 40, "century");  // kann man als Abbruchbedingung in die Schleife im Hauptprogramm
        }                                                               // über "getText()" einbauen
        
} ;

#endif
            
