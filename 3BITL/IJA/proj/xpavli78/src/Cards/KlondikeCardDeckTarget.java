package Cards;

import java.util.ArrayList;

/**
 * Třída implementující cílový balíček.
 * @author Jan Pavlica, xpavli78
 */
public class KlondikeCardDeckTarget implements CardDeck{
    
    /**
     * Seznam karet v cílovém balíčku.
     */
    private ArrayList<Card> myDeck;
    /**
     * Vrchní karta v cílovém balíčku.
     */
    private Card stackLast;
    /**
     * Typ karty pro cílový balíček.
     */
    private Card.Color myColor;
    
    /**
     * Metoda pro generování cesty k obrázku pro daný typ karet.
     * @return Jméno souboru pro daný typ karet.
     */
    public String colorPath()
    {
        return ("/"+myColor.toString()+"_bot.png");
    }

    /**
     * Metoda vracející barvu cílového balíčku.
     * @return Barva cílového balíčku.
     */
    public Card.Color color(){
        return myColor;
    }

    /**
     * Vytvoření cílového balíčku o požadované velikosti.
     * @param size Počet karet v cílovém balíčku.
     */
    public KlondikeCardDeckTarget(int size){
        myDeck = new ArrayList<>(size);
    }
    
    /**
     * Vytvoření cílového balíčku daného typu.
     * @param color Typ, kterému má odpovídat balíček.
     */
    public KlondikeCardDeckTarget(Card.Color color){
        myDeck = new ArrayList<>(13);
        myColor = color;
    }
    
    /**
     * Vrací počet karet v balíčku.
     * @return Počet karet.
     */
    @Override
    public int size(){
        return myDeck.size();    
    }
    
    /**
     * Vložení karty na vrchol balíčku.
     * @param card Karta, která má být vložena na vrchol balíčku.
     * @return True v případě úspěchu.
     */
    @Override
    public boolean put(Card card){
        if(myDeck.isEmpty() && card.value() == 1 && myColor == card.color())
        {
            myDeck.add(card);
            stackLast = card;
            return true;
        }
        else if(stackLast != null && (stackLast.color() == card.color()) && stackLast.compareValue(card) == -1)
        {
            myDeck.add(card);
            stackLast = card;
            return true;
        }
        
        return false;
    }

    /**
     * Vrací kartu z vrcholu balíčku, kterou z něj odebere.
     * @return Karta z vrcholu balíčku.
     */
    @Override
    public Card pop(){
        if(myDeck.isEmpty()){
            return null;
        }
        else{
            Card top = myDeck.get(myDeck.size() - 1);
            myDeck.remove(myDeck.size() - 1);
            if(myDeck.isEmpty())
            {
                stackLast = null;
                
            }
            else
                stackLast = myDeck.get(myDeck.size() - 1);
            return top;
        }
    }
    
    /**
     * Tisk všech karet v bbalíčku.
     * @return Počet karet v balíčku.
     */
    public int printAll()
    {
        int count = 0;
        
        for(Card k : myDeck)
        {
            System.out.print(k.color());
            System.out.print(k.value());
            System.out.print(" ");
            count++;
        }
        
        System.out.println("DECK");      
        
        return count;
    }
    
    /**
     * Vrací kartu z vrcholu balíčku.
     * @return Karta z vrcholu balíčku.
     */
    @Override
    public Card get() {
        if(myDeck.isEmpty()){
            return null;
        }
        else{
            Card top = myDeck.get(myDeck.size() - 1);
            return top;
        }
    }

    /**
     * Vrací kartu z pozice na daném indexu.
     * @param index Index v zásobníku.
     * @return Karta na požadované pozici.
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
     * Test, zda je balíček prázdný.
     * @return Vrací true, pokud je zásobník prázdný.
     */
    @Override
    public boolean isEmpty() {
        return myDeck.isEmpty();
    }
    
}
