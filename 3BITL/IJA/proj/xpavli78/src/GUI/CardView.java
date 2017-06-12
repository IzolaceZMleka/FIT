package GUI;

import Boards.Platno;
import Boards.UndoHint;
import Cards.Card;
import Cards.CardStack;
import Cards.KlondikeCard;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;


/**
 * Trida reprezentujici GUI pro konkretni kartu. Stara se o vsechny presuny individualnich karet v ramci GUI.
 * @author Jan Pavlica, xpavli78
 */

public class CardView extends Button{
    
    /**
    * Pomocna trida pro zpracovani pohybu pri drag. 
    */
    
    class Delta {
        double x, y;
    }
    
    /**
    * Odkaz na reprezentaci karty v BackEndu.
    */
    KlondikeCard representedCard;
    /**
    * Reprezentace GUI karty nad aktualni.
    */
    CardView top = null;
    /**
    * Reprezentace GUI karty pod aktualni.
    */
    public CardView bot = null;
    /**
    * Reprezentace aktualni GUI karty.
    */
    CardView present = null;
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
    Platno p;
    /**
    * Pomocna promenna pro pohyb pri drag akci.
    */
    Delta dragDelta = new Delta();
    /**
    * Graficka reprezentace aktualniho Stacku ve kterem se karta nachazi.
    */
    StackView actualStack = null;
    /**
    * Graficka reprezentace aktualniho Targetu ve kterem se karta nachazi.
    */
    TargetView actualDeck = null;
    
    /**
     * Konstruktor pro vytvoreni graficke reprezentace karty.
     * @param actualC Aktualni Stack ve kterem se dany CardView nachazi. V pripade, ze neni ve Stacku prijima null.
     * @param actualS Aktualni Target ve kterem se dany CardView nachazi. V pripade, ze neni v Target prijima null.
     * @param gridP Mrizka, ktera obsahuje vsechny karty.
     * @param pp Platno dane hry.
     * @param discardP Odkladaci hromadka v dane hre.
     */
    public CardView(KlondikeCard actualC, Object actualS, GridPane gridP, Platno pp, DiscardPileView discardP)
    {
        representedCard = actualC;
        
        if(actualS != null)
        {
            if(actualS.getClass()==TargetView.class){
                actualDeck = (TargetView)actualS;
            }else{
                actualStack = (StackView)actualS;
            }
        }
        
        Image img;
        
        if(representedCard.isTurnedFaceUp())
        {
            img = new Image(getClass().getResourceAsStream(actualC.filePath()));
        }
        else
        {
            img = new Image(getClass().getResourceAsStream("/back.png"));
            this.setDisable(true);
        }
        
        grid = gridP;
        
        ImageView imgView = new ImageView(img);
        imgView.fitWidthProperty().bind(grid.widthProperty().divide(9));
        imgView.setPreserveRatio(true);
        this.setGraphic(imgView);
        
        
        discardPile = discardP;
        p = pp;
        
        
        present = this;
        
        setOnMouseClicked(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                if(mouseEvent.getButton().equals(MouseButton.SECONDARY))
                    p.Undo();
                if(mouseEvent.getButton().equals(MouseButton.PRIMARY)){
                    if(mouseEvent.getClickCount() == 2){
                        
                        for(TargetView t : p.targetViews)
                        {
                            if(p.targets[t.index].put(representedCard))
                            {
                                if(present.discardPile != null)
                                {
                                    p.undo.add(new UndoHint(present, t, discardPile, UndoHint.Type.TARGETTODISCARD, false));
                                    present.discardPile.cardsViews.remove(present);
                                    present.discardPile = null;
                                    p.discardPile.pop();
                                }
                                else
                                {
                                    if(present.bot != null && !present.bot.representedCard.isTurnedFaceUp())
                                        p.undo.add(new UndoHint(present, t, present.actualStack, UndoHint.Type.TARGETTOSTACK, true));
                                    else
                                        p.undo.add(new UndoHint(present, t, present.actualStack, UndoHint.Type.TARGETTOSTACK, false));
                                    
                                    p.stacks[present.actualStack.indexB].pop();
                                }
                                present.finishCards(t);
                            }
                        }
                        
                        
                    }
                }
            }
        });
        
        setOnMousePressed(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                
                
                
                // record a delta distance for the drag and drop operation.
                dragDelta.x = present.getLayoutX() - mouseEvent.getSceneX();
                dragDelta.y = present.getLayoutY() - mouseEvent.getSceneY();
                present.setCursor(Cursor.MOVE);
                
                
                //button.setStyle("-fx-background-color: green;");
            }
        });
        
        setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                present.setCursor(Cursor.HAND);
                
                int col = getCol(grid.getScene().getWidth(),mouseEvent.getSceneX());
                int row = getRow(grid.getScene().getHeight(),mouseEvent.getSceneY());
                
                if(row == 1)
                {
                    StackView n = (StackView) getNodeFromGridPane(grid,col,1);
                    
                    if(present.getStack() != null && n != present.getStack()) // from stack to another stack
                    {
                        stackToStack(n,true,false);
                    }
                    else if(n != present.getStack() && present.discardPile != null) // from discard to stack
                    {
                        discardToStack(n,true,false);
                    }
                    else if(present.actualDeck != null) // from target to stack
                    {
                        targetToStack(n,true,false);
                    }
                }
                else if(row == 0 && col > 2 && present.top == null) // from stack/discard to target
                {
                    TargetView d = (TargetView) getNodeFromGridPane(grid,col,row);
                    
                    stackDiscardToTarget(d,col-3,true);
                    
                }
                
                present.goBack();
            }
        });
        
        setOnMouseDragged(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                present.setLayoutX(mouseEvent.getSceneX() + dragDelta.x);
                present.setLayoutY(mouseEvent.getSceneY() + dragDelta.y);
                
                if(present.getStack() != null)
                    present.getStack().toFront();
                else if (present.discardPile != null)
                    discardPile.toFront();
                else
                    present.actualDeck.toFront();
                
                
                present.toFront();
                
                if(present.top != null)
                {
                    present.top.moveTopCards(mouseEvent.getSceneX() + dragDelta.x,mouseEvent.getSceneY() + dragDelta.y);
                }
                
            }
        });
        setOnMouseEntered(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                present.setCursor(Cursor.HAND);
            }
        });
    }
    
    /**
     * Metoda zajistujici presun ze Stacku do Stacku.
     * @param n Stack do, ktereho ma byt vlozena karta.
     * @param undoOperation Promenna pro odliseni operace zpet.
     * @param fromTop Promenna slouzici pro zaznam, zda-li pri provedeni operace byla odkryta karta nize.
     */
    public void stackToStack(StackView n, boolean undoOperation, boolean fromTop)
    {
        CardStack pom = p.stacks[present.actualStack.indexB].pop(present.representedCard);
        
        if(p.stacks[n.indexB].put(pom))
        {
            if(undoOperation)
            {
                if(present.bot != null && !present.bot.representedCard.isTurnedFaceUp())
                    p.undo.add(new UndoHint(present, n, present.actualStack, UndoHint.Type.STACKTOSTACK, true));
                else
                    p.undo.add(new UndoHint(present, n, present.actualStack, UndoHint.Type.STACKTOSTACK, false));
            }
            present.addCards(n, true, undoOperation, fromTop);
        }
        else if(!undoOperation)
        {
            p.stacks[n.indexB].putForce(pom);
            present.addCards(n, true, undoOperation, fromTop);
        }
        else
            p.stacks[present.actualStack.indexB].putForce(pom);
    }
    
    /**
     * Metoda zajistujici presun ze Stacku do Targetu.
     * @param d Cilovy target.
     * @param index Index daneho targetu.
     * @param undoOperation Promenna pro odliseni operace zpet.
     */
    public void stackDiscardToTarget(TargetView d, int index, boolean undoOperation)
    {
        if(p.targets[index].put(present.representedCard))
        {
            if(present.discardPile != null)
            {
                
                if(undoOperation)
                {
                    p.undo.add(new UndoHint(this, d, discardPile, UndoHint.Type.TARGETTODISCARD,false));
                }
                present.discardPile.cardsViews.remove(present);
                present.discardPile = null;
                p.discardPile.pop();
            }
            else
            {
                if(undoOperation)
                {
                    if(present.bot != null && !present.bot.representedCard.isTurnedFaceUp())
                        p.undo.add(new UndoHint(present, d, present.actualStack, UndoHint.Type.TARGETTOSTACK, true));
                    else
                        p.undo.add(new UndoHint(present, d, present.actualStack, UndoHint.Type.TARGETTOSTACK, false));
                }
                p.stacks[present.actualStack.indexB].pop();
            }
            present.finishCards(d);
        }
    }
    
     /**
     * Metoda zajistujici presun z odkladaciho balicku do Stacku.
     * @param n Stack do, ktereho ma byt vlozena karta.
     * @param undoOperation Promenna pro odliseni operace zpet.
     * @param fromTop Promenna slouzici pro zaznam, zda-li pri provedeni operace byla odkryta karta nize.
     */
    public void discardToStack(StackView n, boolean undoOperation, boolean fromTop)
    {
        Card pom = p.discardPile.pop();
        if(p.stacks[n.indexB].put(pom))
        {
            if(undoOperation)
            {
                p.undo.add(new UndoHint(this, actualStack, discardPile, UndoHint.Type.STACKTODISCARD, false));
            }
            present.discardPile.cardsViews.remove(present);
            present.discardPile = null;
            present.addCards(n, true, undoOperation, fromTop);
            p.UpdateScore(5);
        }
        else
            p.discardPile.put(pom);
        
    }
    
     /**
     * Meto zajistujici presun z Target do Stacku.
     * @param n Stack do, ktereho ma byt vlozena karta.
     * @param undoOperation Promenna pro odliseni operace zpet.
     * @param fromTop Promenna slouzici pro zaznam, zda-li pri provedeni operace byla odkryta karta nize.
     */
    public void targetToStack(StackView n, boolean undoOperation, boolean fromTop)
    {
        Card pom = p.targets[present.actualDeck.index].pop();
        
        if(p.stacks[n.indexB].put(pom))
        {
            if(undoOperation)
                p.undo.add(new UndoHint(this, n, present.actualDeck, UndoHint.Type.STACKTOTARGET,false));
            present.actualDeck = null;
            present.addCards(n, true, undoOperation, fromTop);
            p.UpdateScore(-10);
        }
        else if(!undoOperation)
        {
            p.stacks[n.indexB].putForce(pom);
            present.actualDeck = null;
            present.addCards(n, true, undoOperation, fromTop);
            p.UpdateScore(-10);
        }
        else
            p.targets[present.actualDeck.index].put(pom);
    }
    
    /**
     * Metoda zajistujici presun karet pri drag.
     * @param x Hodnota posunu po ose X.
     * @param y Hodnota posunu po ose Y.
     */
    public void moveTopCards(double x, double y)
    {
        this.toFront();
        this.setLayoutX(x);
        this.setLayoutY(y);
        if(top != null)
            top.moveTopCards(x,y);
    }
    
    /**
     * Metoda nastavujici aktualni stack.
     * @param actualS Aktualni stack, kde se karta nachazi.
     */
    public void setStack(StackView actualS)
    {
        actualStack = actualS;
    }
    
    /**
     * Metoda presunujici kartu z Target nebo Stack zpet do odkladaciho balicku pri operaci zpet.
     * @param pile Odkaz na okladajici balicek.
     */
    public void backToDiscard(DiscardPileView pile)
    {
        if(actualStack != null)
        {
            p.discardPile.put(present.representedCard);
            pile.cardsViews.add(this);
            actualStack.RemoveCard();
            p.stacks[actualStack.indexB].pop();
            setTranslateY(0);
            actualStack = null;
            if(bot != null)
                bot.top = null;
            bot = null;
            
            discardPile = pile;
            p.UpdateScore(-5);
            pile.getChildren().add(this);
        }
        else if(actualDeck != null)
        {
            p.UpdateScore(-10);
            p.discardPile.put(present.representedCard);
            p.targets[actualDeck.index].pop();
            pile.cardsViews.add(this);
            actualDeck = null;
            discardPile = pile;
            
            pile.getChildren().add(this);
        }
        
    }
    
    /**
     * Metoda vracejici kartu z odkladaciho do dobiraciho balicku.
     * @param pile Odkaz na odkladaci balicek.
     * @param deck Odkaz na dobiraci balicek.
     */
    public void backToDeck(DiscardPileView pile, DeckView deck)
    {
        Image image3 = new Image(getClass().getResourceAsStream("/back.png"));
        ImageView img3 = new ImageView(image3);
        img3.fitWidthProperty().bind(grid.widthProperty().divide(9));
        img3.setPreserveRatio(true);
        deck.setGraphic(img3);
        p.UpdateScore(-5);
        present.representedCard.turnFaceDown();
        pile.getChildren().remove(this);
        
        deck.cardsViews.add(0,pile.cardsViews.remove(pile.cardsViews.size()-1));
        
        p.deck.putBot(present.representedCard);
        p.discardPile.pop();
    }
    
    /**
     * Metoda zajistujici presun karty do Target.
     * @param t Odkaz na cilovy target, do ktereho ma byt presunuta karta.
     */
    public void finishCards(TargetView t)
    {
        if(actualStack != null)
            this.getStack().RemoveCard();      
        this.actualDeck = t;
        this.actualStack = null;
        this.setTranslateY(0);
        this.top = null;
        
        if(bot != null)
        {
            
            bot.setTop(null);
            bot.representedCard.turnFaceUp();
            Image image3 = new Image(getClass().getResourceAsStream(bot.representedCard.filePath()));
            ImageView img3 = new ImageView(image3);
            img3.fitWidthProperty().bind(this.widthProperty());
            img3.setPreserveRatio(true);
            bot.setGraphic(img3);
            bot.setDisable(false);
            p.UpdateScore(5);
        }
        
        t.getChildren().add(this);
        p.UpdateScore(10); 
    }
    
    /**
     * Metoda zajistujici pridani karet do Stacku.
     * @param n Cilovy stack, do ktereho ma byt presunuta karta.
     * @param first Promenna, slouzici k odliseni prvni karty a pripadnych karet nad ni.
     * @param undoOp Promenna pro odliseni operace zpet.
     * @param fromTop Promenna slouzici pro zaznam, zda-li pri provedeni operace byla odkryta karta nize.
     */
    public void addCards(StackView n, boolean first, boolean undoOp, boolean fromTop)
    {
        if(actualStack != null)
            this.getStack().RemoveCard();
        
        this.setTranslateY(n.AddCard()*20);
        
        if(first)
        {
            if(bot != null)
            {
                bot.setTop(null);
                
                bot.representedCard.turnFaceUp();

                if(undoOp)
                    p.UpdateScore(5);                
                Image image3 = new Image(getClass().getResourceAsStream(bot.representedCard.filePath()));
                ImageView img3 = new ImageView(image3);
                img3.fitWidthProperty().bind(this.widthProperty());
                img3.setPreserveRatio(true);
                bot.setGraphic(img3);
                bot.setDisable(false);
            }
            
            if(n.presentCards > 1)
            {
                ObservableList<Node> pom = n.getChildren();
                CardView pom1 = (CardView)(pom.get(pom.size()-1));
                
                if(undoOp)
                {
                    pom1.setTop(this);
                    bot = pom1;
                }
                else
                {
                    p.UpdateScore(-5);
                    if(fromTop)
                    {
                        Image image3 = new Image(getClass().getResourceAsStream("/back.png"));
                        ImageView img3 = new ImageView(image3);
                        img3.fitWidthProperty().bind(this.widthProperty());
                        img3.setPreserveRatio(true);
                        pom1.setGraphic(img3);
                        pom1.representedCard.turnFaceDown();
                        fromTop = false;
                        pom1.setDisable(true);
                    }
                    present.bot = pom1;
                    pom1.top = this;
                }
                
            }
            else
                bot = null;
        }
        
        n.getChildren().add(this);
        this.setStack(n);
        if(top != null)
            top.addCards(n, false, true , false);
    }
    
    /**
     * Metoda vracejici karty na sve misto v pripade, ze byla pozadana nevalidni operace.
     */
    public void goBack()
    {
        this.setLayoutX(0);
        this.setLayoutY(0);
        
        if(top != null)
            top.goBack();
        
        
    }
    
    /**
     * Metoda vracejici aktualni stack.
     * @return Aktualni stack ve kterem se karta nachazi.
     */
    public StackView getStack()
    {
        return actualStack;
    }
    
    /**
     * Metoda vracejici aktualni target.
     * @return Aktualni target ve kterem se karta nachazi.
     */
    public TargetView getDeck()
    {
        return actualDeck;
    }
    
    /**
     * Metoda natavujici aktualni target, ve kterem se karta nachazi.
     * @param actualD Aktualni target.
     */
    public void setDeck(TargetView actualD)
    {
        actualDeck = actualD;
    }
    
    /**
     * Metoda nastavujici kartu nad aktualni kartou.
     * @param topCard GUI karty nad aktualni.
     */
    public void setTop(CardView topCard)
    {
        top = topCard;
    }
    
    /**
     * Metoda nastavujici kartu pod aktualni kartou.
     * @param botCard GUI karty pod aktualni.
     */
    public void setBot(CardView botCard)
    {
        bot = botCard;
    }
    
    /**
     * Vraci kartu nad aktualni.
     * @return Karta nad aktualni.
     */
    public CardView getTop()
    {
        return top;
    }
    
    /**
     * Vraci kartu pod aktualni.
     * @return Karta pod aktualni.
     */
    public CardView getBot()
    {
        return bot;
    }
    
    private int getCol (double width, double pos)
    {
        if(pos > grid.widthProperty().get())
            pos = pos - grid.widthProperty().get();
        
        int res = (int)(pos/(grid.widthProperty().get()/7));
        if(res > 6)
            return 6;
        else if(res < 0)
            return 0;
        
        return res;
    }
    
    private int getRow (double height, double pos)
    {
        
        if(pos > grid.heightProperty().get())
            pos = pos - grid.heightProperty().get();
        
        int res = (int)(pos/(grid.heightProperty().get()/10));
        
        
        if(res > 2)
            return 1;
        else
            return 0;
    }
    
    private Node getNodeFromGridPane(GridPane gridPane, int col, int row) {
        
        
        if(row == 0 && col > 2)
            return p.targetViews[col-3];
        else
            return p.stackViews[col];
        
    }
    
    
}
