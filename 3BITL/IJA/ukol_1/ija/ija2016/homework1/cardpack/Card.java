package ija.ija2016.homework1.cardpack;

/**
 *
 * @author Jan Pavlica
 */
public class Card {
    
    private Color colour;
    private int rate;
    
    public static enum Color {
        CLUBS, DIAMONDS, HEARTS, SPADES;
        
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
        final Card other = (Card) obj;
        if (this.rate != other.rate) {
            return false;
        }
        if (this.colour != other.colour) {
            return false;
        }
        return true;
    }
    
     
     
    public Card(Card.Color c, int value){
        this.colour = c;
        this.rate = value;
    }
    
    public Color color(){
        return colour;
    }
    
    public int value(){
        return rate;
    }
    
    @Override
    public String toString() {
        String text;
        
        switch(rate){
            case 1:
                text = "A(";
                break;
            case 11:
                text = "J(";
                break;
            case 12:
                text = "Q(";
                break;
            case 13:
                text = "K(";
                break;
            default:
                text = Integer.toString(rate) + "(";
                break;
                    
        }
        
        switch(colour){
            case CLUBS:
                text = text+"C)";
                break;
            case DIAMONDS:
                text = text+"D)";
                break;
            case HEARTS:
                text = text+"H)";
                break;
            case SPADES:
                text = text+"S)";
                break;
        }

        return text;
    }
    
}
