/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;

/**
 *
 * @author Jan Pavlica
 */
public class KlondikeCardDeckTarget implements CardDeck{
    
    private ArrayList<Card> myDeck;
    private Card stackLast;
    private Card.Color myColor;
    
    public KlondikeCardDeckTarget(int size){
        myDeck = new ArrayList<>(size);
    }
    
    public KlondikeCardDeckTarget(Card.Color color){
        myDeck = new ArrayList<>(13);
        myColor = color;
    }
    
    @Override
    public int size(){
        return myDeck.size();    
    }
    
    @Override
    public boolean put(Card card){
        if(myDeck.isEmpty() && card.value() == 1 && myColor.similarColorTo(card.color()))
        {
            myDeck.add(card);
            stackLast = card;
            return true;
        }
        else if(stackLast != null && (stackLast.similarColorTo(card)) && stackLast.compareValue(card) == -1)
        {
            myDeck.add(card);
            stackLast = card;
            return true;
        }
        
        return false;
    }
    
        
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

    @Override
    public boolean isEmpty() {
        return myDeck.isEmpty();
    }
    
}
