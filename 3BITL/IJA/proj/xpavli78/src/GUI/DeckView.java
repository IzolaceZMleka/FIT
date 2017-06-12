package GUI;

import Boards.Platno;
import Boards.UndoHint;
import Cards.KlondikeCard;
import java.util.ArrayList;
import java.util.List;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;


/**
 * Trida reprezentujici GUI pro odkladaci balicek.
 * @author Jan Pavlica, xpavli78
 */
public class DeckView extends Button{
    
    /**
    * Graficke rozhrani pro odkladaci balicek.
    */
    DiscardPileView discardPile;
    /**
    * Mrizka obsahujici vsechny graficke elementy.
    */
    GridPane grid;
    /**
    * Aktualni stav hry.
    */
    Platno pp;
    /**
    * GUI reprezentace aktualniho objektu pro zpracovani udalosti. 
    */
    DeckView dis;
    /**
    * Pomocne pole uchovavajici seznam aktualnich karet v baliku.
    */
    public List<CardView> cardsViews = new ArrayList<>();
    
    /**
     *
     * @param p Aktualni hra.
     * @param gridP Mrizka obsahujici vsechny graficke elementy dane hry.
     * @param discardP Graficke rozhrani pro odkladaci balicek.
     */
    public DeckView(Platno p, GridPane gridP, DiscardPileView discardP)
    {
        discardPile = discardP;
        grid = gridP;
        dis = this;
        pp = p;
        
        Image image3 = new Image(getClass().getResourceAsStream("/back.png"));
        ImageView img3 = new ImageView(image3);
        img3.fitWidthProperty().bind(grid.widthProperty().divide(9));
        img3.setPreserveRatio(true);
        
        this.setMaxWidth(Double.MAX_VALUE);
        this.setPadding(Insets.EMPTY);
        this.setGraphic(img3);
        
        for(int i = 0; i < p.deck.size(); i++)
        {
            KlondikeCard card = (KlondikeCard)p.deck.get(i);
            card.turnFaceUp();
            CardView button = new CardView(card, null, grid, p, discardPile);
            button.setMaxWidth(Double.MAX_VALUE);
            button.setPadding(Insets.EMPTY);
            cardsViews.add(button);
            card.turnFaceDown();
        }
        
        setOnAction(new EventHandler<ActionEvent>() {
            @Override public void handle(ActionEvent e) {
                
                KlondikeCard card = (KlondikeCard)p.deck.get();
                
                if(card == null)
                {
                    List<CardView> pom;
                    
                    pom = discardPile.cardsViews;
                    discardPile.cardsViews = cardsViews;
                    cardsViews = pom;
                    
                    p.undo.add(new UndoHint(null, dis, null, UndoHint.Type.REPEATDECK, false));
                    
                    discardPile.getChildren().clear();
                    p.repeatDeck();
                    dis.setGraphic(img3);
                    return;
                }
                
                if(!card.isTurnedFaceUp())
                {
                    p.UpdateScore(5);
                    card.turnFaceUp();
                }
                
                
                
                p.discardPile.put(p.deck.popBot());
                CardView pom = cardsViews.remove(0);
                discardP.cardsViews.add(pom);
                discardP.getChildren().add(pom);
                dis.setGraphic(img3);
                p.undo.add(new UndoHint(pom, discardPile, dis, UndoHint.Type.DISCARDTODECK, false));
                
                if(p.deck.isEmpty())
                {
                    Image image4 = new Image(getClass().getResourceAsStream("/back_refresh.png"));
                    ImageView img4 = new ImageView(image4);
                    img4.fitWidthProperty().bind(grid.widthProperty().divide(9));
                    img4.setPreserveRatio(true);
                    dis.setGraphic(img4);
                }
                
                
            }
            
            
        });
    }
    
    /**
     * Metoda, ktera obrati odkladaci balicek zpet na dobiraci pri operaci zpet.
     */
    public void undoWholeDeck()
    {
        Image image4 = new Image(getClass().getResourceAsStream("/back_refresh.png"));
        ImageView img4 = new ImageView(image4);
        img4.fitWidthProperty().bind(grid.widthProperty().divide(9));
        img4.setPreserveRatio(true);
        dis.setGraphic(img4);
        pp.UpdateScore(100);
        
        int size = cardsViews.size();
        
        for(int i = 0; i < size; i++)
        {
            pp.discardPile.put(pp.deck.popBot());
            CardView pom = cardsViews.remove(0);
            discardPile.cardsViews.add(pom);
            discardPile.getChildren().add(pom);
        }
    }
    
}
