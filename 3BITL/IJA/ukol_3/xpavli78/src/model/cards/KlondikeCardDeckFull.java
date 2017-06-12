/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package model.cards;

import java.util.ArrayList;

/**
 *
 * @author Jan Pavlica
 */
public class KlondikeCardDeckFull implements CardDeck{
    
    private ArrayList<Card> myDeck;
    
    public KlondikeCardDeckFull(int size){
        myDeck = new ArrayList<>(size);
    }
    
    @Override
    public int size(){
        return myDeck.size();    
    }
    
    @Override
    public boolean put(Card card){
        myDeck.add(card);
        return true;
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
