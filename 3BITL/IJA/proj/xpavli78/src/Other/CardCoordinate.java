/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Other;



/**
 *  Třída pro uložení a práci se souřadnicemi karty. Používá trojrozměrné adresování.
 *  - GameIndex
 *      - 0-3
 *          - 0 = karta lezi v dobbíracím balíčku
 *          - 1 = karta lezi v odkladacím balíčku
 *          - 2 = karta lezi v jedno z cílových balíčku
 *          - 3 = karta lezi v jedno z pracovních balíčku
 *          .
 *      .
 *   - DeckIndex
 *      - 0-4/7 slouží k rozlišení jednotlivých cílových/pracovních balíčků
 *      .
 *   - GameIndex
 *      - Kolikátou kartou v tom balíčku je.
 *      .
 * .
 * @author Patrik Chukir, xchuki00
 */
public class CardCoordinate {

    /**
     *
     */
    public int GameIndex;

    /**
     *
     */
    public int DeckIndex;

    /**
     *
     */
    public int CardIndex;

    /**
     *
     * @param GI
     * @param DI
     * @param CI
     * 
     * Základní kostruktor, který rovnou nastavuje i souřadnice.
     */
    public CardCoordinate(int GI, int DI, int CI) {
        GameIndex = GI;
        DeckIndex = DI;
        CardIndex = CI;
    }

    /**
     *Prázdný kostruktor, pro rozlišení má věechny 3 souřadnice rovné -1.
     * 
     */
    public CardCoordinate() {

        GameIndex = -1;
        DeckIndex = -1;
        CardIndex = -1;
    }

    /**
     *
     * @param GI
     * @param DI
     * @param CI
     * Metoda pro nastavení souřadnic v případě prázdného kostructoru.
     */
    public void set(int GI, int DI, int CI) {
        GameIndex = GI;
        DeckIndex = DI;
        CardIndex = CI;
    }

    /**
     *
     * @return
     * Override metody toString z java.Object pro získání člověkem čitelné formy souřadnic.
     */
    @Override
    public String toString() {
        String s = "";
        s += "GameIndex: " + GameIndex;

        s += "; DeckIndex: " + DeckIndex;

        s += "; CardIndex " + CardIndex;
        return s;
    }

    /**
     *
     * @return
     * * Override Metody hashCode z java.Object z důvodu Overridu metody equals
     */
    @Override
    public int hashCode() {
        int hash = 1;
        hash = hash*17 + GameIndex;
        hash = hash * 31 + DeckIndex;
        hash = hash *13 + CardIndex;
        return hash;
    }

    /**
     *
     * @param obj
     * @return
     * Override Metody equals z java.Object
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
        final CardCoordinate other = (CardCoordinate) obj;
        if (this.GameIndex != other.GameIndex) {
            return false;
        }
        if (this.DeckIndex != other.DeckIndex) {
            return false;
        }
        if (this.CardIndex != other.CardIndex) {
            return false;
        }
        return true;
    }
}
