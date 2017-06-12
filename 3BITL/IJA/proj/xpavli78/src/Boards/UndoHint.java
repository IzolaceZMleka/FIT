package Boards;

import GUI.CardView;
import javafx.scene.Node;

/**
 * Trida pro uchovani akci pro pripadnou akci zpet.
 * @author Jan Pavlica, xpavli78
 */

public class UndoHint {
    /**
     * Pocet GUI karty pro kterou ma byt provedena akce zpet.
     */
    CardView card;
    /**
     * Uzel ze ktereho ma byt element odebran.
     */
    Node from;
    /**
     * Uzel do ktereho ma byt element pridan.
     */
    Node to;
    /**
     * Typ operace.
     */
    Type type;
    /**
     * Promenna slouzici k zaznamu, zda byla karta presunuta z jiz odkryte karty, ci nikoliv.
     */
    Boolean fromTop;
    
    /**
     * Kontruktor pro dany typ akce.
     * @param c Karta pro kterou ma byt operace provedena.
     * @param f Uzel ze ktereho ma byt karta odebrana.
     * @param t Uzel do ktereho ma byt karta pridana.
     * @param ty Typ operace zpet.
     * @param fT Zaznam, zda byla karta presunuta z jiz odkryte karty, ci nikoliv.
     */
    public UndoHint(CardView c, Node f, Node t, Type ty, Boolean fT)
    {
        from = f;
        card = c;
        to = t;
        type = ty;
        fromTop = fT;
    }
    
    /**
     * Vycet jednotlivych akci pro zpet.
     */
    public enum Type {

        /**
         * Presun ze pracovniho do pracovniho balicku.
         */
        STACKTOSTACK,

        /**
         * Presun z pracovniho balicku do odkladaci hromadky.
         */
        STACKTODISCARD,

        /**
         * Presun z pracovniho do ciloveho.
         */
        STACKTOTARGET,

        /**
         * Preun z odkladaciho balicku do dobiraciho.
         */
        DISCARDTODECK,

        /**
         * Presun z ciloveho do odkladaciho balicku.
         */
        TARGETTODISCARD,

        /**
         * Presun z ciloveho do pracovniho.
         */
        TARGETTOSTACK,

        /**
         * Protoceni dobiraciho a odkladaciho balicku.
         */
        REPEATDECK;
    }
}
