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
public class KlondikeCard implements Card{
    
    private Card.Color colour;
    private int rate;
    boolean faceUp;
   
    public KlondikeCard(Card.Color colour, int value)
    {       
        rate = value;
        this.colour = colour;   
        faceUp = false;
    }
    
    @Override
    public int value()
    {
        return rate;
    }
    
    @Override
    public boolean isTurnedFaceUp()
    {
        return faceUp;
    }
    
    @Override
    public boolean turnFaceUp()
    {
        if(faceUp)
            return false;
        
        faceUp = true;
        
        return true;
    }
    
    @Override
    public int hashCode(){
        return 23*this.rate + this.colour.toString().hashCode();
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
        final KlondikeCard other = (KlondikeCard) obj;
        if (this.rate != other.rate) {
            return false;
        }
        if (this.colour != other.colour) {
            return false;
        }
        return true;
    }
    
    @Override
    public Card.Color color()
    {
        return colour;
    }
    
    @Override
    public boolean similarColorTo(Card c)
    {
        if(c == null)
            return false;
        
        return c.color().similarColorTo(colour);
    }
    
    @Override
    public int compareValue(Card c)
    {
        return rate - c.value();
    }
    
}
