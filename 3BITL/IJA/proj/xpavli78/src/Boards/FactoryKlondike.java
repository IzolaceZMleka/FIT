package Boards;
import Cards.*;
/**
 * Třída implemetující tovární metody.
 * @author Jan Pavlica, xpavli78
 */
public class FactoryKlondike extends AbstractFactorySolitaire{
    
    
    @Override
    public Card createCard(Card.Color color, int value)
    {   
        if(value < 1 || value > 13)
            return null;
        
        return new KlondikeCard(color,value);
    }

    @Override
    public CardDeck createCardDeck() {
        KlondikeCardDeckFull fullDeck = new KlondikeCardDeckFull(52);
        
        for (int i = 1; i <= 13; i++) { fullDeck.put(new KlondikeCard(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { fullDeck.put(new KlondikeCard(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { fullDeck.put(new KlondikeCard(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { fullDeck.put(new KlondikeCard(Card.Color.SPADES,i)); }
        
        fullDeck.Shuffle();
                
        return fullDeck;
    }
    
    /**
     * Vytvorení prázdného balíčku, který slouží jako odkládací.
     * @return Prázdný balíček.
     */
    public CardDeck createCardDeckEmpty() {
        KlondikeCardDeckFull fullDeck = new KlondikeCardDeckFull(52);
              
        return fullDeck;
    }

    @Override
    public CardDeck createTargetPack(Card.Color color) {
        return new KlondikeCardDeckTarget(color);
    }

    @Override
    public CardStack createWorkingPack() {
        return new KlondikeCardStackWork(); 
    }
    
}
