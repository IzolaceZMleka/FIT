package Cards;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

/**
 * Třída implementující dobírací a odkládací blaíček.
 * @author Jan Pavlica, xpavli78
 */
public class KlondikeCardDeckFull implements CardDeck{
    
    private ArrayList<Card> myDeck;
    
    /**
     * Zamíchaní balíčku.
     */
    public void Shuffle()
    {
        long seed = System.nanoTime();
        Collections.shuffle(myDeck, new Random(seed));
    }
    
    /**
     * Vytvoření balíčku dané velikosti.
     * @param size Velikost balíčku.
     */
    public KlondikeCardDeckFull(int size){
        myDeck = new ArrayList<>(size);
    }
    
    /**
     * Zjistí poček karet balíčku.
     * @return Počet karet.
     */
    @Override
    public int size(){
        return myDeck.size();    
    }
    
    /**
     * Vloží kartu do balíčku.
     * @param card Karta, která se má vložit.
     * @return True při úspěchu.
     */
    @Override
    public boolean put(Card card){
        myDeck.add(card);
        return true;
    }
    
    /**
     * Vloží kartu naspod balíčku.
     * @param card Karta, která se má vložit.
     * @return True, při úspěchu.
     */
    public boolean putBot(Card card){
        myDeck.add(0,card);
        return true;
    }
    
    /**
     * Vrací kartu z vrcholu balíčku, kterou odebere.
     * @return Danou kartu v případě úspěchu, jinak null.
     */
    @Override
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
    
    /**
     * Vezmu kartu zespod balíčku, kterou odebere.
     * @return Karta zespod v případě úspěchu, jinak null.
     */
    public Card popBot(){
        if(myDeck.isEmpty()){
            return null;
        }
        else{
            Card top = myDeck.get(0);
            myDeck.remove(0);
            return top;
        }
    }

    /**
     * Vrací kartu z vrcholu balíčku.
     * @return Karta v případě úspěchu, jinak null.
     */
    @Override
    public Card get() {
        if(myDeck.isEmpty()){
            return null;
        }
        else{
            Card top = myDeck.get(0);
            return top;
        }
    }

    /**
     * Vrací kartu na požadované místě.
     * @param index Místo na kterém chceme vzít kartu.
     * @return Daná karta v případě úspěchu, jinak null.
     */
    @Override
    public Card get(int index) {
        if(myDeck.isEmpty() || myDeck.size()-1 < index || index < 0){
            return null;
        }
        else{
            Card top = myDeck.get(index);
            return top;
        }
    }

    /**
     * Testuje, zda je balíček prázdný.
     * @return True v případě úspěchu.
     */
    @Override
    public boolean isEmpty() {
        return myDeck.isEmpty();
    }
    
}
