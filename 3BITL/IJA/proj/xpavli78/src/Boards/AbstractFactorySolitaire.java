package Boards;
import Cards.*;
/**
 * Abstraktní třída obsahující tovární metody.
 * @author Jan Pavlica, xpavli78
 */
public abstract class AbstractFactorySolitaire {

    /**
     * Vytváří objekt reprezentující balíček karet.
     * @return Balíček karet.
     */
    public abstract CardDeck createCardDeck();

    /**
     * Vytvoří objekt reprezentující kartu.
     * @param color Barva karty.
     * @param value Hodnota karty v rozsahu 1 až 13.
     * @return Objekt karty. Pokud je některý z parametrů neplatný (objekt nelze vytvořit), vrací null.
     */
    public abstract Card createCard(Card.Color color, int value);

    /**
     * Vytváří objekt reprezentující cílový balíček. Cílem hráče je vložit všechny karty zadané barvy do cílového balíčku.
     * @param color Barva celého balíčku.
     * @return Cílový balíček.
     */
    public abstract CardDeck createTargetPack(Card.Color color);

    /**
     * Vytváří objekt reprezentující pracovní pole pro karty.
     * @return Pracovní pole.
     */
    public abstract CardStack createWorkingPack();
}
