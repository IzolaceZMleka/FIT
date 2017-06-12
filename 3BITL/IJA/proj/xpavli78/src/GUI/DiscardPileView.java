package GUI;

import Boards.Platno;
import Cards.KlondikeCard;
import java.util.ArrayList;
import java.util.List;
import javafx.geometry.Insets;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;

/**
 * Trida reprezentujici GUI pro odkladaci balicek.
 * @author Jan Pavlica, xpavli78
 */
public class DiscardPileView extends Pane{
    
    /**
     * Pomocny list pro ulozeni aktualnich karet v odkladacim balicku.
     */
    public List<CardView> cardsViews = new ArrayList<>();
    /**
     * 
     * @param gp GridPane do kterého je discardPile přídán
     * @param pl Plátno příslušné hry
     */
    public DiscardPileView(GridPane gp,Platno pl){
        
        for(int i = 0; i < pl.discardPile.size(); i++)
        {
                      
            KlondikeCard card = (KlondikeCard)pl.discardPile.get(i);
            card.turnFaceUp();
            CardView button = new CardView(card, null,gp, pl, this);
            button.setPadding(Insets.EMPTY);
            cardsViews.add(button);
            this.getChildren().add(button);
            
        }
        
    }
    
}
