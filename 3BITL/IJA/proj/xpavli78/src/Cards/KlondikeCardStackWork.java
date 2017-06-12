package Cards;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Implementace pracovního balíčku.
 * @author Jan Pavlica, xpavli78 
 */
public class KlondikeCardStackWork implements CardStack{
    
    private ArrayList<Card> myStack;
    private Card stackLast;
    
    /**
     * Vytvoří a inicializuje zásobník karet.
     */
    public KlondikeCardStackWork(){
        myStack = new ArrayList<>(13);
    }

    /**
     * Vytvoří a inicializuje zásobník karet.
     * @param size Počáteční velikost zásobníku karet.
     */
    public KlondikeCardStackWork(int size){
        myStack = new ArrayList<>(size);
    }
    
    /**
     * Vrací aktuální seznam karet.
     * @return Seznam karet.
     */
    @Override
    public ArrayList<Card> getStack()
    {
        return myStack;
    }
    
    /**
     * Vloží karty ze zásobníku stack na vrchol zásobníku. Karty vkládá ve stejném pořadí, v jakém jsou uvedeny v zásobníku stack. Karta na vrcholu zásobníku vkládaných karet stack bude i na vrcholu tohoto zásobníku.
     * @param stack Zásobník vkládaných karet.
     * @return True v případě úspěchu.
     */
    public boolean put(CardStack stack){
        Card top = stack.getStack().get(0);
        if(myStack.isEmpty() && top.value() == 13)
        {
            myStack.addAll(stack.getStack());
            stackLast = myStack.get(myStack.size() - 1);
            return true;
        }
        else if(stackLast != null && !(stackLast.similarColorTo(top)) && stackLast.compareValue(top) == 1)
        {
            myStack.addAll(stack.getStack());
            stackLast = myStack.get(myStack.size() - 1);
            return true;
        }
        
        return false;
    }
    
    /**
     * Vloží celý zásobník bez kontroly podmínek.
     * @param stack Vkládaný zásobník.
     * @return True.
     */
    public boolean putForce(CardStack stack){

            myStack.addAll(stack.getStack());
            stackLast = myStack.get(myStack.size() - 1);
            return true;
    }
    
    /**
     * Metoda, která vloží kartu na vrchol zásobníku, bez kontroly podmínek.
     * @param card Karta, která přijde vložit do zásobníku.
     * @return True v případě úspěchu.
     */
    public boolean putForce(Card card){

            myStack.add(card);
            stackLast = card;
            return true;
    }
    
    /**
     * Vloží kartu na vrchol zásobníku.
     * @param card Karta, která přijde vložit do zásobníku.
     * @return True v případě úspěchu.
     */
    public boolean put(Card card){
        
        if(myStack.isEmpty() && card.value() == 13)
        {
            myStack.add(card);
            stackLast = card;
            return true;
        }
        else if((stackLast != null && !(stackLast.similarColorTo(card))) && stackLast.compareValue(card) == 1)
        {
            myStack.add(card);
            stackLast = card;
            return true;
        }
        return false;
    }

    /**
     * Vloží kartu na vrchol zásobníku. Nekontroluje podmínky. Využívá se při incializaci balíku.
     * @param card Karta, která přijde vložit do zásobníku.
     * @return True v případě úspěchu.
     */
    public boolean putStart(Card card){
            myStack.add(card);
            stackLast = card;
            return true;
    }

    /**
     * Test, zda je zásobník prázdný.
     * @return Vrací true, pokud je zásobník prázdný.
     */
    @Override
    public boolean isEmpty(){
        return myStack.isEmpty(); 
    }
    
    /**
     * Zjištění počtu karet v zásobníku.
     * @return Aktuální počet karet v zásobníku.
     */
    @Override
    public int size(){
        return myStack.size(); 
    }
    
    /**
     * Vrací pracovní balíček od dané karty.
     * @param card Karta, od které chceme získat zbytek karet.
     * @return Pracovní balíček.
     */
    @Override
    public CardStack pop(Card card){
        int index = myStack.lastIndexOf(card);
        
        if(index == -1)
            return null;
        
        List<Card> part = myStack.subList(index, myStack.size());
        
        KlondikeCardStackWork newStack = new KlondikeCardStackWork(part.size());
        newStack.getStack().addAll(part);
        
        myStack.removeAll(part);
        if(myStack.isEmpty())
            stackLast = null;
        else
            stackLast = myStack.get(myStack.size() - 1);
        
        return newStack;
            
    }
    
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
        final CardStack other = (CardStack) obj;

        return other.getStack().equals(this.myStack);
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 67 * hash + Objects.hashCode(this.myStack);
        return hash;
    }

    /**
     * Vrací kartu z vrcholu zásobníku, kterou z něj odebere.
     * @return Karta na vrcholu zásobníku.
     */
    @Override
    public Card pop(){
        if(myStack.isEmpty()){
            stackLast = null;
            return null;
        }
        else{
            Card top = myStack.get(myStack.size() - 1);
            myStack.remove(myStack.size() - 1);
            if(myStack.isEmpty())
                stackLast = null;
            else
                stackLast = myStack.get(myStack.size() - 1);
            
            return top;
        }
    }

    /**
     * Vrací kartu z vrcholu zásobníku.
     * @return Karta na vrcholu zásobníku.
     */
    @Override
    public Card get() {
        if(myStack.isEmpty()){
            return null;
        }
        else{
            Card top = myStack.get(myStack.size() - 1);
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
        if(myStack.isEmpty() || myStack.size()-1 < index || index < 0){
            return null;
        }
        else{
            Card top = myStack.get(index);
            return top;
        }
    }
    
    /**
     * Vytiskne obsah zásobníku.
     * @return Počet karet v zásobníku.
     */
    public int printAll()
    {
        int count = 0;
        
        for(Card k : myStack)
        {
            System.out.print(k.color());
            System.out.print(k.value());
            System.out.print(" ");
            count++;
        }
        
        System.out.println("STACK");
        
        
        return count;
    }
}
