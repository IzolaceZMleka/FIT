package model.board;
import model.cards.*;
/**
 *
 * @author Jan Pavlica
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
