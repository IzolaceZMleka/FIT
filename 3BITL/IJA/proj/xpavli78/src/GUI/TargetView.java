package GUI;

import Boards.Platno;
import Cards.KlondikeCard;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;

/**
 * Graficka reprezentace Targetu.
 * @author Jan Pavlica, xpavli78
 */
public class TargetView extends Pane {

    /**
     * Index GUI targetu.
     */
    public int index;

    /**
     * Konstruktor pro vytvoření grafické reprezentace Targetu.
     * @param index Index Targetu.
     * @param p Odkaz na aktualni hru.
     * @param gridP Mrizka, kde se nachazi graficke elementy aktualni hry.
     */
    public TargetView(int index, Platno p, GridPane gridP) {
        this.index = index;

        CardView prevCard = null;
        Button btn = new Button();
        Image image3 = new Image(getClass().getResourceAsStream(p.targets[index].colorPath()));
        ImageView img3 = new ImageView(image3);
        img3.fitWidthProperty().bind(gridP.widthProperty().divide(9));
        img3.setPreserveRatio(true);
        btn.setGraphic(img3);
        btn.setMaxWidth(Double.MAX_VALUE);
        btn.setPadding(Insets.EMPTY);
        this.getChildren().add(btn);

        for (int i = 0; i < p.targets[index].size(); i++) {

            KlondikeCard card = (KlondikeCard) p.targets[index].get(i);

            CardView button = new CardView(card, (Object) this, gridP, p, null);

            button.setPadding(Insets.EMPTY);
           // button.setTranslateY(presentCards * 20);
            button.top = null;
            button.bot = null;

            this.getChildren().add(button);

        }
        
    }
}
