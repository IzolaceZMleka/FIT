package ija.ija2016.homework1.cardpack;

import java.util.*;

/**
 *
 * @author Jan Pavlica
 */
public class CardStack {
    
    private ArrayList<Card> myStack;
    
    public CardStack(int size){
        myStack = new ArrayList<>(size);
    }
    
    public void put(Card card){
        myStack.add(card);
    }
    
    public void put(CardStack stack){
        myStack.addAll(stack.myStack);
    }
    
    public boolean isEmpty(){
            return myStack.isEmpty(); 
    }
    
    public int size(){
        return myStack.size(); 
    }
    
    
    public CardStack takeFrom(Card card){
        int index = myStack.lastIndexOf(card);
        
        if(index == -1)
            return null;
        
        List<Card> part = myStack.subList(index, myStack.size());
        
        CardStack newStack = new CardStack(part.size());
        newStack.myStack.addAll(part);
        
        myStack.removeAll(part);
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

        return other.myStack.equals(this.myStack);
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 67 * hash + Objects.hashCode(this.myStack);
        return hash;
    }
            
}
