package Cards;

/**
 * Rozhraní reprezentující jednu kartu. Karta obsahuje informaci o své hodnotě (1 až 13) a barvě. Tyto informace jsou nastaveny konstruktorem. Hodnota 1 reprezentuje eso (ace), 11 až 13 postupně kluk (jack), královna (queen) a král (king). Barvu definuje výčtový typ Color.
 * @author Jan Pavlica, xpavli78
 */
public interface Card {
    
    /**
     * Výčet typů karet.
     */
    public static enum Color {
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES;
        
        /**
         * Výčet barev.
         */
        public static enum BlackRed{
            BLACK,
            RED;
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
                    return "clubs";
                case DIAMONDS:
                    return "diamonds";
                case HEARTS:
                    return "hearts";
                case SPADES:
                    return "spades";
                default:
                    return "?";
            }
        }
        
        /**
         * Porovnává barvu (černá/červená).
         * @param c Porovnáváná barva karty.
         * @return True pokud jsou barvy shodné.
         */
        public boolean similarColorTo(Card.Color c)
        {
            if(c.getGeneral().equals(general))
                return true;
            else
                return false;
        }
        
        /**
         * 
         * @return Červená nebo černá barva karty.
         */
        public Color.BlackRed getGeneral()
        {
            return general;
        }
        
    }
    
    /**
     *
     * @return Hodnota karty.
     */
    int value();
    
    /**
     * Testuje, zda je karta otočená lícem nahoru
     * @return Výsledek testu: true = karta je otočená lícem nahoru.
     */
    boolean isTurnedFaceUp();
    
    /**
     * Otočí kartu lícem nahoru. Pokud tak už je, nedělá nic.
     * @return Informace, zda došlo k otočení karty (=true) nebo ne.
     */
    boolean turnFaceUp();
    
    /**
     * 
     * @return Barva karty.
     */
    Color color();
    
    /**
     * Testuje, zda má karta podobnou barvu jako karta zadaná. Podobnou barvou se myslí černá (piky, kříže) a červená (káry a srdce).
     * @param c Karta pro porovnání.
     * @return Informace o shodě barev karet.
     */
    boolean similarColorTo(Card c);
    
    /**
     * Porovná hodnotu karty se zadanou kartou c. Pokud jsou stejné, vrací 0. Pokud je karta větší než zadaná c, vrací kladný rozdíl hodnot.
     * @param c Karta, s kterou se porovnává.
     * @return Vrací rozdíl hodnot karet.
     */
    int compareValue(Card c);
}
