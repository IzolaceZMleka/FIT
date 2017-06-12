/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Other;

import Cards.Card;


/**
 *
 * @author Patrik Chukir, xchuki00
 * Třída pro prácí s pohyby karet v rámci získávání nápovědy.
 * 
 */
public class Move {

    /**
     * Výchozí pozice
     */
    public CardCoordinate befor;

    /**
     *Výsledná pozice
     */
    public CardCoordinate after;

    /**
     * Karta se kterou má být tah proveden  
     */
    public Card card;

    /**
     * Prázdný konstruktor
     * - befor a afte jsou naplněny souřadnicemi -1
     * - card  = null
     */
    public Move() {
        befor = new CardCoordinate();
        after = new CardCoordinate();
        card = null;

    }

    /**
     *
     * @param beforI
     * @param afterI
     * @param c
     */
    public Move(CardCoordinate beforI, CardCoordinate afterI, Card c) {
        befor = beforI;
        after = afterI;
        card = c;
    }

    /**
     *
     * @return
     * 
     */
    @Override
    public int hashCode() {
        int hash = 1;
        hash = hash * 17 + ((befor == null) ? 0 : befor.hashCode());
        hash = hash * 31 + ((after == null) ? 0 : after.hashCode());
        hash = hash * 13 + ((card == null) ? 0 : card.hashCode());
        return hash;
    }

    /**
     *
     * @param obj
     * @return
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Move other = (Move) obj;

        if (this.befor == other.befor) {
            return false;
        }
        if (this.after == other.after) {
            return false;
        }
        if (this.card == null && other.card == null) {
            return true;
        }
        if (this.card == other.card) {
            return false;
        }
        return true;
    }

    /**
     *
     * @return
     * Metoda vrátí pohyb opačným směrem, slouží třeba k porovnání jestli nejde o zpětný pohyb.
     */
    public Move inverse() {
        return new Move(after, befor, card);
    }
}
