/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.homework2.model.cards;

/**
 *
 * @author IzolaceZMleka
 */
public interface Card {
    
    public static enum Color {
        CLUBS, DIAMONDS, HEARTS, SPADES;
        
        public static enum BlackRed{
            BLACK, RED;
        }
        
        private Color.BlackRed general;
        
        static
        {
            CLUBS.general = BlackRed.BLACK;
            DIAMONDS.general = BlackRed.RED;
            HEARTS.general = BlackRed.RED;
            SPADES.general = BlackRed.BLACK;
        }
        
        @Override
        public String toString() {
        
            switch(this){
                case CLUBS:
                    return "C";
                case DIAMONDS:
                    return "D";
                case HEARTS:
                    return "H";
                case SPADES:
                    return "S";
                default:
                    return "?";
            }
        }
        
        public boolean similarColorTo(Card.Color c)
        {
            if(c.getGeneral().equals(general))
                return true;
            else
                return false;
        }
        
        public Color.BlackRed getGeneral()
        {
            return general;
        }
        
    }
    
    
    int value();
    
    boolean isTurnedFaceUp();
    
    boolean turnFaceUp();
    
    Color color();
    
    boolean similarColorTo(Card c);
    
    int compareValue(Card c);
}
