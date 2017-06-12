/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package model.cards;

/**
 *
 * @author IzolaceZMleka
 */
public interface CardDeck {
    
    int size();
    
    boolean put(Card card);
    
    Card pop();
    
    Card get();
    
    Card get(int index);
    
    boolean isEmpty();
}
