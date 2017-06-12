/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;

/**
 *
 * @author IzolaceZMleka
 */
public interface CardStack extends CardDeck{
    
    boolean put(CardStack stack);
    
    CardStack pop(Card card);
    
    public ArrayList<Card> getStack();
}
