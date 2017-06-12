/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 *
 * @author IzolaceZMleka
 */
public class KlondikeCardStackWork implements CardStack{
    private ArrayList<Card> myStack;
    private Card stackLast;
    
    
    public KlondikeCardStackWork(){
        myStack = new ArrayList<>(13);
    }
    public KlondikeCardStackWork(int size){
        myStack = new ArrayList<>(size);
    }
    
    @Override
    public ArrayList<Card> getStack()
    {
        return myStack;
    }
    
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
    
    public boolean isEmpty(){
        return myStack.isEmpty(); 
    }
    
    public int size(){
        return myStack.size(); 
    }
    
    
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

    @Override
    public Card pop(){
        if(myStack.isEmpty()){
            return null;
        }
        else{
            Card top = myStack.get(myStack.size() - 1);
            myStack.remove(myStack.size() - 1);
            return top;
        }
    }

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
}
