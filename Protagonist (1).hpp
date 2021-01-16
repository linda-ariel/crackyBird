#include <iostream>
#include <algoviz/SVG.hpp>
#include "Hindernis.hpp"

using namespace std;


#ifndef _PROTAGONIST_
#define _PROTAGONIST_

class Protagonist {
    
    private:
    
        SVG *spielfeld;
        Text *anzeigeP;
        Text *anzeigeL;
        Image bild[3][2];              //je nach Bilderanzahl...1.Dimension: laufen, springen, kriechen
        Image tot;
        Image blitz;
        bool supersprungB;
        int bildwert1;                 //versch. Bilder bekommen einen "Bildwert"
        int bildwert2;
        int leben;                     //sagt wie viele Leben der Protagonist "noch" hat
        int punkte;
        int x;                         //Position: x-Koordinate
        int y;                      //Position: y-Koordinate
        int yOben;
        //string hindernisInfos;
        //Hindernis *hindernis;          // Zeiger auf Hindernis, dass aktuell bei x = 300 steht
        
    public: 
        
        //Konstruktor
        Protagonist(SVG *spielfeld, Text *anzeigeP, Text *anzeigeL) {
            this->spielfeld = spielfeld;
            this->anzeigeP = anzeigeP;
            this->anzeigeL = anzeigeL;
            supersprungB = false;
            bildwert1 = 1;
            bildwert2 = 0;
            leben = 3;
            punkte = 0;
            x = 150;
            y = 420;
            //hindernisInfos = "";
            yOben = 460;
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
            } else if ( bildwert1 == 1 && y >= yOben - 40 ) {
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
            y = y - 20;                                          // Differenz, die der Protagonist weiter nach oben springt
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
            y = y + 35;                                            // ...(auf dem er GELAUFEN ist)
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
        
        /*int getHindernisY() {
            int zaehler = 0;
            for ( int i = 0; i < hindernisInfos.length(); i = i + 10 ) {
                if ( hindernisInfos[i] == 44 ) {
                    zaehler++;
                }
            }
            if ( hindernisInfos == "" || hindernisInfos.length() / 10 == zaehler ){
                return 460;
            } else {
                int yObenAktuell = 460;
                for ( int i = 0; i < hindernisInfos.length(); i = i + 10 ) {
                    int y1 = 0;
                    int y2 = 0;
                    if ( hindernisInfos[i] != 47 ) {
                        y1 = (hindernisInfos[i] - 48) * 100;
                    }
                    if ( hindernisInfos[i+1] != 47 ) {
                        y2 = (hindernisInfos[i+1] - 48) * 10;
                    }
                    int y3 = hindernisInfos[i+2] - 48;
                    int yOben = y1 + y2 + y3;
                    
                    if ( yOben > getY() + 40 && yOben < yObenAktuell ) {
                        yObenAktuell = yOben;
                    }
                }
                cout << yObenAktuell << endl;
                return yObenAktuell;
            }
        }
        */
                
            /*if ( hindernis != nullptr ) {                                    // wenn gerade ein Hindernis bei x=300 steht
                return hindernis->getYoben();                                // Operation in Hindernis benötigt, die die y-Koordinate der...
            }                                                                // ... oberen Kante des Hinternisses zurückgibt.
            return 460;                                                      // wenn kein Hindernis bei x=300 steht, so ist das "Hindernis"
        }                                                                    // so hoch wie der Boden (460-40, siehe "springenrunter()")
        */
        
        int findYhindernis(list<Rect*> hindernis) {
            if (hindernis.empty()){
                return 460;
            }
            auto it = hindernis.begin();
            while ( it != hindernis.end() ) {
                Rect* block = *it;
                if ( block->getY() > y + 40 && block->getY() < yOben ) {
                    yOben = block->getY();
                }
                it++;
            }
            return yOben;
        }
            
    
        /*void setHindernisInfos(string hindernisInfos) {
            this->hindernisInfos = hindernisInfos;
        }*/
                            
        void setLeben(int plusminuseins) {
            leben = leben + plusminuseins;
            anzeigeL->setText("Leben: " + to_string(leben));
            if ( leben <= 0 ) {
                gameOver();
            }
        }
        
        void setPunkte(int punkte) {
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
            spielfeld->drawText("Game over", 220, 230, 40, "century");  // kann man als Abbruchbedingung in die Schleife im Hauptprogramm
        }                                                               // über "getText()" einbauen
    
        /*bool kollisionItem(list<Item*> *muenze) {      
            for ( Item* item : muenze ) {
                if ( item->getXitem() <= 190 && item->getXitem() >= 110 ) {
                    if ( item->getYitem() <= getY() + 40 && item->getYitem() >= getY() - 40 ) {
                        setPunkte(1);
                        return true;
                    }
                }
            }
            return false;
        }
    
        int kollisionFeind(list<Item*> *feind) {
            for ( Item* item : feind ) {
                if ( item->getXitem() <= 190 && item->getXitem() >= 110 ) {
                    if ( item->getYitem() - 90 >= getY() && item->getYitem() - 100 <= getY() ) {
                        setLeben(1);
                        return 1;                                    // Der Protagonist ist auf den Feind gesprungen
                    } else if ( item->getYitem() - 90 < getY() ) {
                        setLeben(-1);
                        return 0;                                    // Der Protagonist ist in den Feind gelaufen
                    }
                }
            }
            return -1;                                               // Es gibt keine Berührung
        }*/
    
        /*bool kollisionHindernis() {
            for ( int i = 0; i < hindernisInfos.length(); i = i + 10 ) {
                if ( hindernisInfos[i] != 44 ) {
                    int y1 = 0;
                    int y2 = 0;
                    if ( hindernisInfos[i] != 47 ) {
                        y1 = (hindernisInfos[i] - 48) * 100;
                    }
                    if ( hindernisInfos[i+1] != 47 ) {
                        y2 = (hindernisInfos[i+1] - 48) * 10;
                    }
                    int y3 = hindernisInfos[i+2] - 48;
                    int yOben = y1 + y2 + y3;
                    
                    int yU1 = 0;
                    int yU2 = 0;
                    if ( hindernisInfos[i+5] != 47 ) {
                        yU1 = (hindernisInfos[i+4] - 48) * 100;
                    }
                    if ( hindernisInfos[i+6] != 47 ) {
                        yU2 = (hindernisInfos[i+5] - 48) * 10;
                    }
                    int yU3 = hindernisInfos[i+6] - 48;
                    int yUnten = yU1 + yU2 + yU3;
                    
                    if ( getY() - 40 < yUnten && getY() + 40 > yOben ) {
                        setLeben(-1);
                        return true;
                    }
                }
            }
            return false;
        }*/
    
        void hindernisKollision(list<Rect*> hindernis) {
            auto it = hindernis.begin();
            while ( it != hindernis.end() ) {
                Rect* block = *it;
                int yUnten = block->getY() + block->getHeight();
                if ( y + 40 < yUnten && y - 40 > block->getY() ) {
                    y = yOben - 40;
                    bild[bildwert1][bildwert2].moveTo(x,y);
                    setLeben(-1);
                }
                it++;
            }
        }
                
} ;

#endif
            