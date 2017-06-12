/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Other;

import Cards.Card;
import Cards.KlondikeCardDeckFull;
import Cards.KlondikeCardDeckTarget;
import Cards.KlondikeCardStackWork;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javafx.util.Pair;

/**
 *Třída Advisor obsahuje strukturu potřebné pro popis logického stavu hry a v nich vyhledává optimální tahy.
 * @author Patrik Chukir, xchuki00
 */
public class Advisor {

    /**
     *Reference na discarPile Platno pro které generuje nápovědu.
     */
    public KlondikeCardDeckFull discardPile;

    /**
     *Reference na deck Platno pro které generuje nápovědu.
     */
    public KlondikeCardDeckFull deck;

    /**
     *Reference na targets Platno pro které generuje nápovědu.
     */
    public KlondikeCardDeckTarget[] targets;

    /**
     *Reference na stacks Platno pro které generuje nápovědu.
     */
    public KlondikeCardStackWork[] stacks;
    /**
     * Posledni napověda slouží ke snížení opakování nápověd
     */
    private Move LastHint;
    /**
     * Možné tahy vygenerované pro aktuální rozložení.
     * Metoda FindBest potom mezi nimi vyhledává nejlepší. 
     * Výhodnost tahu reprezentuje Integer jenž slouží i jako klíč, ten je vypočítán v rámci getAllMoveForCard
     * nebo pro speciální případy už v run.
     * 
     */
    private Map<Integer, Move> PosibleMove;

    /**
     *
     * @param discard
     * @param deckI
     * @param targetsI
     * @param stacksI
     * Očekává instance všech baličků zahrnutý ve hře a nadnimi potom bude provádět analýzu.
     */
    public Advisor(KlondikeCardDeckFull discard, KlondikeCardDeckFull deckI, KlondikeCardDeckTarget[] targetsI, KlondikeCardStackWork[] stacksI) {
        discardPile = discard;
        deck = deckI;
        targets = targetsI;
        stacks = stacksI;
        PosibleMove = new HashMap<Integer, Move>();
        LastHint = new Move();
    }

    /**
     *
     * @return
     * Stěžejní funkce, řídí běh celého výpočtu a vyhledání nejlepšího tahu, a následně ošetřuje, aby nebyl stejný jako předchozí ani jeho inverze.
     */
    public Move getHint() {
        run();
        Integer mi = findBestMove();
        Move m = PosibleMove.get(mi);
        if(PosibleMove.size()==0){
            return null;
        }
        //System.out.println(PosibleMove.size());

        m.befor = getCoordOfCard(m.card);
        if (m == LastHint || m == LastHint.inverse()) {
          //  System.out.println("Stejna napoveda");
            PosibleMove.remove(mi);

            mi = findBestMove();
            m = PosibleMove.get(mi);
            m.befor = getCoordOfCard(m.card);
            if (m == LastHint || m == LastHint.inverse()) {
                PosibleMove.remove(mi);
                mi = findBestMove();
                m = PosibleMove.get(mi);
                m.befor = getCoordOfCard(m.card);
            }
        }
        LastHint = m;
        //System.out.println(m.befor.toString());
        //System.out.println(m.after.toString());
        //PosibleMove.put(5, LastHint);
        return m;
    }
/**
 * 
 * @return 
 * Naplní slovní PosibleMove všemi možnými tahy a jejich ohodnocením.
 * Přitom sama vyhodnotí jestli některá z getAllLast karet nemůže jít přímo do TargetuPacku, potom vloží tento Move do PosibleMove a ukočíse, ostatní tahy už vůbec nezkoumá.
 */
    private Move run() {
                PosibleMove.clear();
        for (Pair<Card, Integer> p : getAllLast()) {
            Card c = p.getKey();
            int t = canGoToTarget(c);
            if (t >= 0) {
                PosibleMove.put(p.getValue()+45, new Move(null, new CardCoordinate(2, t, targets[t].size()), c));
            }
        }
        List<Pair<Card, Integer>> list = getAllDeckDiscardCard();
        list.addAll(getAllStackCard());
        for (Pair<Card, Integer> c : list) {
            PosibleMove.putAll(getAllMovesForCard(c));
        }
        return null;
    }
    /**
     * 
     * @return
     * Vrací ArrayList obsahujíc Pair<Card,jeji hloubka> pro každou otočenou kartu v pracovních plochách,
     * kdy hloubka předsatvuje počet karet na ní položených.
     */
    private List<Pair<Card, Integer>> getAllStackCard() {
        List<Pair<Card, Integer>> ret = new ArrayList<Pair<Card, Integer>>();
        for (int i = 0; i < 7; i++) {
            if (!stacks[i].isEmpty()) {
                for (int j = 0; j < stacks[i].size(); j++) {
                    Card c = stacks[i].get(j);
                    if (c.isTurnedFaceUp()) {
                        ret.add(new Pair(c, j));
                    }
                }
            }
        }
        return ret;
    }
    /**
     * 
     * @return
     * 
     * Vrací ArrayList obsahujíc Pair<Card,- jeji hloubka> pro každou  kartu v dobírací a odkládacím balíčku,
     * kdy hloubka předsatvuje počet karet potrebých otoči abych tuto kartu dostali na vrchol odhazovacího balíčku.
     */
    private List<Pair<Card, Integer>> getAllDeckDiscardCard() {
        List<Pair<Card, Integer>> ret = new ArrayList<Pair<Card, Integer>>();

        if (!deck.isEmpty()) {
            //System.out.println(deck.size());
            for (int j = 0; j < deck.size(); j++) {

                ret.add(new Pair(deck.get(j), -j));
            }
        }
        if (!discardPile.isEmpty()) {

            //System.out.println(discardPile.size());
            for (int j = discardPile.size() - 1; j <= 0; j--) {
                if (j == discardPile.size() - 1) {

                    ret.add(new Pair(discardPile.get(j), 0));
                } else {
                    ret.add(new Pair(discardPile.get(j), -(j + deck.size())));
                }
            }
        }
        return ret;
    }
/**
 * 
 * @return
 *  * Vrací ArrayList obsahujíc Pair<Card,0> pro každou poslední kartu, tedy z každé pracovní plochy 
 * nejhornější kartu nebo nic pokud je prázdná, vrchol odkládacího balíčku a vrchní kartu dobíracího.

 */
    private List<Pair<Card, Integer>> getAllLast() {

        List<Pair<Card, Integer>> ret = new ArrayList<Pair<Card, Integer>>();
        if (!discardPile.isEmpty()) {
            ret.add(new Pair(discardPile.get(discardPile.size() - 1), 0));
        }
        if (!deck.isEmpty()) {
            ret.add(new Pair(deck.get(), 0));
        }
        for (int i = 0; i < 7; i++) {
            if (!stacks[i].isEmpty()) {
                ret.add(new Pair(stacks[i].get(), 0));
            }
        }
        return ret;
    }
/**
 * 
 * @param p
 * @return
 * Vrátí HashMap<Integer, Move> obsahující všechny možné tahy s danou kartou.
 * Kdy Move obsahuje kartu, polohu po a poloha předtím dohlednána až v metodě getHint,
 * a to pouze pro tah kterej se vrací Platno. A Integer představuje bodové hodnocení tahu, tedy jím přesunem karet, nebo kolik se jich musí otočit v dobíracím,
 * případně bonus pokud je to třeba přesun krále na volnou plochu nebo karty do cílové plochy.
 */
    private Map<Integer, Move> getAllMovesForCard(Pair<Card, Integer> p) {
        Map<Integer, Move> ret = new HashMap<Integer, Move>();
        for (int i = 0; i < 4; i++) {
            Card c = p.getKey();
            int t = canGoToTarget(c);
            if (t >= 0) {
                ret.put(p.getValue() + 30, new Move(null, new CardCoordinate(2, t, targets[t].size()), c));
            }
        }
        for (int i = 0; i < 7; i++) {
            Card t = p.getKey();
            if (stacks[i].isEmpty()) {
                if (t.value() == 13) {
                    ret.put(p.getValue() + 5, new Move(null, new CardCoordinate(3, i, 0), t));
                }
            } else {
                Card c = stacks[i].get();

                if (t.value() + 1 == c.value() && !t.similarColorTo(c)) {
                    ret.put(p.getValue(), new Move(null, new CardCoordinate(3, i, stacks[i].size()), t));
                }
            }
        }
        return ret;
    }
/**
 * 
 * @return
 * Projde PosibleMove a vrátí nejvyší index.
 */
    private Integer findBestMove() {
        Integer max = -50;
        for (Integer i : PosibleMove.keySet()) {
            if (i > max) {
                max = i;
            }
        }

        return max;
    }
/**
 * 
 * @param t
 * @return
 * Zabalení testu zda daná karta může jít do cílové Plochy.
 */
    private int canGoToTarget(Card t) {
        for (int i = 0; i < 4; i++) {
            Card c = targets[i].get();
            if (c == null) {
                if (t.value() == 1 && t.color() == targets[i].color()) {
                    return i;
                }
            } else {
                if (t.value() - 1 == c.value() && t.color() == targets[i].color()) {
                    return i;
                }
            }
        }
        return -1;
    }
/**
 * 
 * @param c
 * @return
 * Prohledání všech struktur a zjištzění kde karta leží.
 * Používá se v getHint pro získaní současné souřadnice karty.
 */
    private CardCoordinate getCoordOfCard(Card c) {
        if (!deck.isEmpty()) {
            for (int i = 0; i < deck.size(); i++) {
                if (deck.get(i) == c) {
                    return new CardCoordinate(0, 0, i);
                }
            }
        }
        if (!discardPile.isEmpty()) {
            for (int i = 0; i < discardPile.size(); i++) {
                if (discardPile.get(i) == c) {
                    return new CardCoordinate(1, 0, i);
                }
            }
        }
        for (int j = 0; j < 4; j++) {
            if (!targets[j].isEmpty()) {
                for (int i = 0; i < targets[j].size(); i++) {
                    if (targets[j].get(i) == c) {
                        return new CardCoordinate(2, j, i);
                    }
                }
            }
        }

        for (int j = 0; j < 7; j++) {
            if (!stacks[j].isEmpty()) {
                for (int i = 0; i < stacks[j].size(); i++) {
                    if (stacks[j].get(i) == c) {
                        return new CardCoordinate(3, j, i);
                    }
                }
            }
        }
        return null;
    }
}
