/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
 */
package Boards;

import java.util.ArrayList;
import Cards.Card;
import static Cards.Card.Color.CLUBS;
import static Cards.Card.Color.DIAMONDS;
import static Cards.Card.Color.HEARTS;
import static Cards.Card.Color.SPADES;
import Cards.KlondikeCard;
import Cards.KlondikeCardDeckFull;
import Cards.KlondikeCardDeckTarget;
import Cards.KlondikeCardStackWork;
import GUI.DeckView;
import GUI.DiscardPileView;
import GUI.GameMenu;
import GUI.StackView;
import GUI.TargetView;
import Other.Advisor;
import Other.Move;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Optional;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.scene.Node;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;

/**
 *
 * @author Patrik Chukir, xchuki00
 *
 * Třída pro správu logické částí hry.
 */
public class Platno {

    /**
     * Factory pro danou variantu pravidel
     */
    FactoryKlondike newPlatno = new FactoryKlondike();
    int score = 0;
    /**
     * reference na label do ScoreBaru
     */
    Label scoreLabel;

    /**
     * Logická reprezentace odkládacího balíčku.
     */
    public KlondikeCardDeckFull discardPile = (KlondikeCardDeckFull) newPlatno.createCardDeckEmpty();

    /**
     * Logická reprezentace dobíracího balíčku
     */
    public KlondikeCardDeckFull deck = (KlondikeCardDeckFull) newPlatno.createCardDeck();

    /**
     * Logická reprezentace cílových balíčků
     */
    public KlondikeCardDeckTarget[] targets = new KlondikeCardDeckTarget[4];

    /**
     * Logická reprezentace pracovních ploch
     */
    public KlondikeCardStackWork[] stacks = new KlondikeCardStackWork[7];

    /**
     * Reference do GameView na stackView pro vyhledávání cílových uzlů
     */
    public StackView[] stackViews = new StackView[7];

    /**
     * Reference do GameView na targetView pro vyhledávání cílových uzlů
     */
    public TargetView[] targetViews = new TargetView[4];

    /**
     * List s provedenými tahy, slouží jako paměť pro undo
     */
    public List<UndoHint> undo = new ArrayList<>();
    /**
     * Instance třídy Advisor z ní lze získat nápovědu.
     */
    private Advisor advisor;

    /**
     *
     * @param s předání reference ze ScoreBar
     *
     */
    public Platno(Label s) {
        scoreLabel = s;
        for (Card.Color c : Card.Color.values()) {
            targets[c.ordinal()] = (KlondikeCardDeckTarget) newPlatno.createTargetPack(c);
        }

        for (int i = 0; i < stacks.length; i++) {
            stacks[i] = (KlondikeCardStackWork) newPlatno.createWorkingPack();

            for (int j = 0; j < i + 1; j++) {
                KlondikeCard myCard = (KlondikeCard) deck.pop();

                if (j == i) {
                    myCard.turnFaceUp();
                }

                stacks[i].putStart(myCard);
            }

        }
        advisor = new Advisor(discardPile, deck, targets, stacks);
        //SaveData("game");
        //LoadData("game");
    }

    /**
     * Zjistí poslední provednou akci, dle seznamu undo, a zavolá příslušnou
     * metodu z třídy UndoHint.
     */
    public void Undo() {
        if (undo.isEmpty()) {
            return;
        }
        UndoHint undoO = undo.remove(undo.size() - 1);

        switch (undoO.type) {
            case STACKTOSTACK: {
                undoO.card.stackToStack((StackView) undoO.to, false, undoO.fromTop);
                break;
            }
            case STACKTODISCARD: {
                undoO.card.backToDiscard((DiscardPileView) undoO.to);
                break;
            }
            case STACKTOTARGET: {
                undoO.card.stackDiscardToTarget((TargetView) undoO.to, ((TargetView) undoO.to).index, false);
                break;
            }
            case DISCARDTODECK: {
                undoO.card.backToDeck((DiscardPileView) undoO.from, (DeckView) undoO.to);
                break;
            }
            case TARGETTODISCARD: {
                undoO.card.backToDiscard((DiscardPileView) undoO.to);
                break;
            }
            case TARGETTOSTACK: {
                undoO.card.targetToStack((StackView) undoO.to, false, undoO.fromTop);
                break;
            }
            case REPEATDECK: {
                ((DeckView) undoO.from).undoWholeDeck();
                break;
            }
            default:

        }
    }

    /**
     *
     * @param s relativní změna skore. Změní hodonotu atributu skore a přepíše
     * text v scoreLabel
     *
     */
    public void UpdateScore(int s) {
        score += s;
        if (score < 0) {
            score = 0;
        }

        if (scoreLabel != null) {

            scoreLabel.setText(Integer.toString(score));
        }

        boolean finished = true;

        for (KlondikeCardDeckTarget t : targets) {
            if (t.size() != 13) {
                finished = false;
            }
        }

        if (finished) {
            Alert alert = new Alert(AlertType.CONFIRMATION);
            alert.setTitle("You are the very best like no one ever was!");
            alert.setHeaderText("You've got amazing score: " + score);
            alert.setContentText("What would you like to do now?");

            ButtonType buttonTypeOne = new ButtonType("New Game");
            ButtonType buttonTypeTwo = new ButtonType("Cancel");
            ButtonType buttonTypeThree = new ButtonType("Quit game");

            alert.getButtonTypes().setAll(buttonTypeTwo);

            Optional<ButtonType> result = alert.showAndWait();

            if (result.get() == buttonTypeOne) {
                // ... user chose "New Game"
                // spusti novou hru
            } else if (result.get() == buttonTypeTwo) {
                alert.close();
                // ... user chose "Load Game"
                // nacte novou hru, pokud zadnou nezvoli aktualni se zavre
            } else if (result.get() == buttonTypeThree) {
                // ... user chose "Quit This Game"
                // zavre aktualni hru
            } else {
                // ... stejne jako quit
            }
        }

    }

    /**
     *
     * @param s Řetězec načtený ze souboru
     * @return Vrací KlondikeCard vytvořenou na základě s
     *
     * Dekoduje uložené karty.
     *
     *
     */
    public KlondikeCard LoadCard(String s) {
        String rate = "";
        rate += s.charAt(0);
        rate += s.charAt(1);
        int r = Integer.valueOf(rate);
        KlondikeCard c = null;
        switch (s.charAt(2)) {
            case 'S':
                c = (KlondikeCard) newPlatno.createCard(SPADES, r);
                break;
            case 'D':
                c = (KlondikeCard) newPlatno.createCard(DIAMONDS, r);
                break;
            case 'H':
                c = (KlondikeCard) newPlatno.createCard(HEARTS, r);
                break;
            case 'C':
                c = (KlondikeCard) newPlatno.createCard(CLUBS, r);
                break;
        }
        if (s.charAt(3) == 'T') {
            c.turnFaceUp();
        }
        return c;
    }

    /**
     * Pomocná funkce pro konzolový výtisk herního plátna.
     */
    public void printTabble() {
        int i = 0;
        for (KlondikeCardStackWork k : stacks) {
            k.printAll();
        }
        //System.out.println("DECKS");
        for (KlondikeCardDeckTarget k : targets) {
            k.printAll();
        }
    }

    /**
     *
     * @param c
     * @return Vrací zakodovaný string
     *
     * Koduje každou kartu na 4 znaky dle vzorce
     *
     * XXCB
     *
     * - XX - hodnota karty vždy na dvě místa 01 - 13 - C - Barva karty S/C/D/H
     * - B - jestli je otočená nebo ne T/F .
     */
    public String SaveCard(Card c) {
        String s = "";
        if (c.value() < 10) {
            s += '0';
        }
        s += c.value();
        switch (c.color()) {
            case SPADES:
                s += "S";
                break;

            case DIAMONDS:
                s += "D";
                break;

            case HEARTS:
                s += "H";
                break;

            case CLUBS:
                s += "C";
                break;

            default:
                break;
        }
        if (c.isTurnedFaceUp()) {
            s += 'T';
        } else {
            s += 'F';
        }
        return s;
    }

    /**
     * Metoda pro otočení balíčku potom co se dobere.
     */
    public void repeatDeck() {

        KlondikeCardDeckFull pom;
        pom = discardPile;
        discardPile = deck;
        deck = pom;
        UpdateScore(-100);
    }

    /**
     *
     * @param arg Adresa uložené hry.
     *
     * Načte uloženou hru, formát uložení je popsán u metody SaveData.
     */
    public void LoadData(String arg) {
        String s = "";
        try {
            Path file = Paths.get(arg);
            byte[] bytes = Files.readAllBytes(file);
            s = new String(bytes);
        } catch (IOException e) {
            return;
        }
        // init discardPile;
        discardPile = (KlondikeCardDeckFull) newPlatno.createCardDeckEmpty();
        deck = (KlondikeCardDeckFull) newPlatno.createCardDeckEmpty();
        targets = new KlondikeCardDeckTarget[4];
        stacks = new KlondikeCardStackWork[7];
        for (Card.Color c : Card.Color.values()) {
            targets[c.ordinal()] = (KlondikeCardDeckTarget) newPlatno.createTargetPack(c);
        }
        for (int i = 0; i < 7; i++) {
            stacks[i] = (KlondikeCardStackWork) newPlatno.createWorkingPack();
        }
        char c = 'A';
        int k = 0;
        int p = 0;
        int count = 0;
        String Card = "";
        for (int i = 0; i < s.length(); i++) {

            c = s.charAt(i);
            // System.out.println(c);
            if (c == 'K') {
                k++;
                Card = "";
                p = 0;
                continue;
            }
            if (c == 'P') {
                p++;
                Card = "";
                continue;
            }
            if (c == 'X') {
                i++;
                Card = "";
                do {
                    c = s.charAt(i);
                    if (c != 'X') {
                        Card += c;
                    }
                    i++;
                } while (c != 'X');
                UpdateScore(Integer.parseInt(Card));
                break;
            }
            Card += c;
            count++;
            if (count % 4 == 0) {
                KlondikeCard card = LoadCard(Card);
                if (k == 0) {
                    // System.out.println(card.toString() + " " + discardPile.size());
                    discardPile.put(card);
                } else if (k == 1) {
                    deck.put(card);
                } else if (k == 2) {
                    targets[p].put(card);
                } else if (k == 3) {
                    stacks[p].putStart(card);
                }
                Card = "";
            }
        }
        // System.out.println(deck.size());
        //System.out.println(discardPile.size());
        /*for (int i = 0; i < 4; i++) {
            System.out.println(targets[i].size());
        }
        for (int i = 0; i < 7; i++) {
            System.out.println(stacks[i].size());
        }*/
    }

    /**
     *
     * @param arg Adresa souboru k uložení Uloží hru jako posloupnost kodovaných
     * karet metodou SaveCard a řídích znaků.
     *
     * K - pro konec jednoho balíku deck/discardPile/Targets/Stacks P - pro
     * odělení mezi jednotlivími cílovími/pracovními balíčky X - pro konec karet
     * a začátek skore a jeho konec opět
     */
    public void SaveData(String arg) {
        if (arg == null) {
            return;
        }
        String s = "";
        int p = 0;
        if (discardPile != null) {
            for (int i = 0; i < discardPile.size(); i++) {
                s += SaveCard(discardPile.get(i));

            }
        }
        s += "K";
        if (deck != null) {
            for (int i = 0; i < deck.size(); i++) {
                s += SaveCard(deck.get(i));

            }
        }
        s += "K";
        if (targets != null) {
            for (int j = 0; j < 4; j++) {
                if (targets[j] != null) {
                    for (int i = 0; i < targets[j].size(); i++) {
                        //System.out.println("DAW");
                        s += SaveCard(targets[j].get(i));
                    }
                }
                s += "P";
            }
        }
        s += "K";
        if (stacks != null) {
            for (int j = 0; j < 7; j++) {
                if (stacks[j] != null) {
                    for (int i = 0; i < stacks[j].size(); i++) {
                        s += SaveCard(stacks[j].get(i));

                    }
                }
                s += "P";
            }
        }
        s += "X";
        s += Integer.toString(score);
        s += "X";
        try {
            Path file = Paths.get(arg);
            byte[] bytes = s.getBytes();
            Files.write(file, bytes);
        } catch (IOException e) {
            return;
        }

        //m.out.println(s);
    }

    /**
     *
     * @return Vrátí nápovědu získanou z Advisor, ve třídě Move.
     *
     */
    public Move getHint() {
        if (advisor != null) {
            return advisor.getHint();
        }
        return null;
    }

}
