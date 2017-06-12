package Cards;

/**
 * Třída reprezentující jednu kartu. Karta obsahuje informaci o své hodnotě (1 až 13) a barvě. Tyto informace jsou nastaveny konstruktorem. Hodnota 1 reprezentuje eso (ace), 11 až 13 postupně kluk (jack), královna (queen) a král (king). Barvu definuje výčtový typ Color. Textová reprezentace karty má podobu "hodnota(barva)", např. srdcový král "K(H)". Informace o barvě vizte výčtový typ Card.Color.
 * @author Jan Pavlica, xpavli78
 */
public class KlondikeCard implements Card{
    
    private Card.Color colour;
    private int rate;
    boolean faceUp;
   
    /**
     * Konstruktor nastavující barvu a hodnotu karty.
     * @param colour Barva karty.
     * @param value Hodnota karty.
     */
    public KlondikeCard(Card.Color colour, int value)
    {       
        rate = value;
        this.colour = colour;   
        faceUp = false;
    }
    
    /**
     * Vrací hodnotu karty.
     * @return Hodnota karty.
     */
    @Override
    public int value()
    {
        return rate;
    }
    
    /**
     * Testuje, zda j e karta otočena.
     * @return True v případě úspěchu.
     */
    @Override
    public boolean isTurnedFaceUp()
    {
        return faceUp;
    }
    
    /**
     * Otočí kartu obrazem nahoru.
     * @return True v případě, že se otočí.
     */
    @Override
    public boolean turnFaceUp()
    {
        if(faceUp)
            return false;
        
        faceUp = true;
        
        return true;
    }
    
    /**
     * Otočí kartu obrazem dolů.
     */
    public void turnFaceDown()
    {        
        faceUp = false;
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
    
    /**
     * Vrací Z karty.
     * @return Barva karty.
     */
    @Override
    public Card.Color color()
    {
        return colour;
    }
    
    /**
     * Testuje, zda se jedná o stejnou barvu.
     * @param c Karta s kterou se porovnává.
     * @return True v příadě, že barva stejná.
     */
    @Override
    public boolean similarColorTo(Card c)
    {
        if(c == null)
            return false;
        
        return c.color().similarColorTo(colour);
    }
    
    /**
     * Vrací rozdíl mezi hodnotami karet.
     * @param c Karta se kterou se porovnává.
     * @return Rozdíl hodnot.
     */
    @Override
    public int compareValue(Card c)
    {
        return rate - c.value();
    }
    
    /**
     * Vrací jméno souboru pro obraz dané karty.
     * @return Název souboru pro požadovaný obrázek.
     */
    public String filePath()
    {
        switch(rate){
            case 1:
                return "/ace"+"_of_"+colour.toString()+".png";
            case 11:
                return "/jack"+"_of_"+colour.toString()+".png";
            case 12:
                return "/queen"+"_of_"+colour.toString()+".png";
            case 13:
                return "/king"+"_of_"+colour.toString()+".png";
            default:
                return "/"+String.valueOf(rate)+"_of_"+colour.toString()+".png";
        }
    }
    
}
