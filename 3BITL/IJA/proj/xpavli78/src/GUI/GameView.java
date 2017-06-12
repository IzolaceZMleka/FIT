package GUI;

import Boards.Platno;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Priority;
import javafx.scene.layout.RowConstraints;

/**
 * Trida reprezentujici graficke rozhrani pro jednotlive hry.
 * @author Jan Pavlica, xpavli78
 */
public class GameView {

    /**
     * Mrizka pro vsechny graficke elementy dane hry.
     */
    GridPane root;

    /**
     * Konstruktor, pro vytvoreni GUI pro novou hru.
     * @param myPlata Odkaz na promennou uchovavajici stav hry pro back end.
     */
    public GameView(Platno myPlata) {

        root = new GridPane();
        root.setStyle("-fx-background-color: green;");

        DiscardPileView dP = new DiscardPileView(root,myPlata);
        root.setHgap(10); //horizontal gap in pixel
        root.setVgap(10); //vertical gap in pixels
        root.setPadding(Insets.EMPTY);

        for (int rowIndex = 0; rowIndex < 2; rowIndex++) {

            double per = 30;

            if (rowIndex == 0) {
                per = 30;
            } else if (rowIndex == 1) {
                per = 70;
            }

            RowConstraints rc = new RowConstraints();
            rc.setVgrow(Priority.ALWAYS); // allow row to grow
            //rc.setFillHeight(true); // ask nodes to fill height for row
            rc.setPercentHeight(per);
            rc.setMinHeight(0);
            // other settings as needed...
            root.getRowConstraints().add(rc);

        }


        for (int colIndex = 0; colIndex < 9; colIndex++) {

            double per = 0.0;

            if (colIndex == 0 || colIndex == 8) {
                per = 1;
            } else {
                per = 14;
            }
            ColumnConstraints cc = new ColumnConstraints();
            cc.setHgrow(Priority.ALWAYS); // allow column to grow
            cc.setFillWidth(true); // ask nodes to fill space for column
            cc.setPercentWidth(per);
            // other settings as needed...

            root.getColumnConstraints().add(cc);
            
            
            
            
            if(colIndex != 0 && colIndex != 8  )
            { 
        
                StackView stack = new StackView(colIndex,myPlata,root);
                
                if(colIndex > 3)
                {
                    TargetView target = new TargetView(colIndex-4,myPlata,root);
                    root.add(target,colIndex,0);
                    myPlata.targetViews[colIndex -4] = target;
                }
                if(colIndex == 2)
                {
                    root.add(dP,colIndex,0);
                }
               if(colIndex == 1) 
               {
                   Pane p = new Pane();
                   
                   p.maxWidthProperty().bind(root.widthProperty().divide(9));
                    
                    DeckView deck = new DeckView(myPlata,root,dP);
                    p.getChildren().add(deck);
                    root.add(p,colIndex,0);
                    
               }
               myPlata.stackViews[colIndex -1] = stack;
               root.add(stack,colIndex,1);
                
           
            }
        }

    }
}
