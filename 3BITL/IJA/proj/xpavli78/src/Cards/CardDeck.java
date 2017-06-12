package Cards;

/**
 * Rozhraní pro dobírací, odkládací a cílový balíček.
 * @author Jan Pavlica, xpavli78
 */
public interface CardDeck {
    
    /**
     *
     * @return Aktuální počet karet v balíčku.
     */
    int size();
    
    /**
     * Vloží kartu na vrchol balíčku.
     * @param card Vkládaná karta.
     * @return Úspěšnost akce.
     */
    boolean put(Card card);
    
    /**
     * Odebere kartu z vrcholu balíčku. Pokud je balíček prázdný, vrací null.
     * @return Karta z vrcholu balíčku.
     */
    Card pop();
    
    /**
     * Vrátí kartu z vrcholu zásobníku (karta zůstává na zásobníku). Pokud je balíček prázdný, vrací null.
     * @return Karta z vrcholu balíčku.
     */
    Card get();
    
    /**
     * Vrátí kartu na uvedenem indexu. Spodni karta je na indexu 0, vrchol je na indexu size()-1. Pokud je balíček prázdný, nebo index mimo rozsah, vrací null.
     * @param index Pozice karty v balicku.
     * @return Karta z vrcholu balíčku.
     */
    Card get(int index);
    
    /**
     * Test, zda je balíček karet prázdný.
     * @return Vrací true, pokud je balíček prázdný.
     */
    boolean isEmpty();
}
