package GUI;

import Boards.Platno;
import Cards.KlondikeCard;
import javafx.geometry.Insets;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;

/**
 * Trida reprezentujici GUI pro jednotlive stacky.
 * @author Jan Pavlica, xpavli78
 */
public class StackView extends Pane {
    
    /**
     * Pocet karet v GUI Stacku.
     */
    int presentCards = 0;
    /**
     * Index GUI stacku.
     */
    int indexB;

    /**
     * Pridani karty do Stacku.
     * @return Pocet karet ve Stacku.
     */
    public int AddCard()
    {
        return presentCards++;
    }
    
    /**
     * Odtraneni karty ze Stacku.
     * @return Pocet karet ve Stacku.
     */
    public int RemoveCard()
    {
        return presentCards--;
    }
    
    /**
     * Kontruktor pro vytvoreni graficke reprezentace Stacku.
     * @param index Index stacku.
     * @param p Odkaz na aktualni hru.
     * @param gridP Mrizka, kde se nachazi graficke elementy aktualni hry.
     */
    public StackView(int index, Platno p, GridPane gridP)
    {
        
        this.indexB = index-1;
        CardView prevCard = null;
        this.maxWidthProperty().bind(gridP.widthProperty().divide(9));
        
        for(int i = 0; i < p.stacks[index-1].size(); i++)
        {
            
            
            KlondikeCard card = (KlondikeCard)p.stacks[index-1].get(i);
            CardView button = new CardView(card, this, gridP, p, null);
            button.setPadding(Insets.EMPTY);
            button.setTranslateY(presentCards*20);
            
            presentCards++;
            
            
            this.getChildren().add(button);
            
            if(prevCard != null)
            {
                prevCard.setTop(button);
            }
            button.setBot(prevCard);
            
            prevCard = button;
            
        }
        
    }
    
}
