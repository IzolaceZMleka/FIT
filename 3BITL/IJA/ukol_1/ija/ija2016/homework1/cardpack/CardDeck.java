
package ija.ija2016.homework1.cardpack;

import java.util.ArrayList;

/**
 *
 * @author Jan Pavlica
 */
public class CardDeck {
    
    private ArrayList<Card> myDeck;
    
    public CardDeck(int size){
        myDeck = new ArrayList<>(size);
    }
    
    
    
    public static CardDeck createStandardDeck(){
        CardDeck fullDeck = new CardDeck(52);
        
        for (int i = 1; i <= 13; i++) { fullDeck.put(new Card(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { fullDeck.put(new Card(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { fullDeck.put(new Card(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { fullDeck.put(new Card(Card.Color.SPADES,i)); }
        
        
        return fullDeck;
    }
    
    public int size(){
        return myDeck.size();    
    }
    
    public void put(Card card){
        myDeck.add(card);
    }
    
    public Card pop(){
        if(myDeck.isEmpty()){
            return null;
        }
        else{
            Card top = myDeck.get(myDeck.size() - 1);
            myDeck.remove(myDeck.size() - 1);
            return top;
        }
    }
    
}
