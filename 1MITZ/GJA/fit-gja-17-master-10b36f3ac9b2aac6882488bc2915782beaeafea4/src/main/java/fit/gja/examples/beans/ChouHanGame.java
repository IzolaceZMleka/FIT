package fit.gja.examples.beans;

import java.util.Random;

/**
 * Cho-Han Bakuchi game.
 *
 * @author Filip Pobořil
 */
public class ChouHanGame {

    private final Random random = new Random();

    private int[] dice = new int[2];
    private int wager = -1;
    private int winCount = 0;

    public int getWager() {
        return wager;
    }

    public void setWager(String wager) {
        try {
            setWager(Integer.parseInt(wager));
        } catch (NumberFormatException e) {
            setWager(-1);
        }
    }

    public void setWager(int wager) {
        this.wager = wager;

        if (getSuccess()) {
            winCount++;
        } else {
            winCount = 0;
        }
    }

    public boolean getSuccess() {
        return (dice[0] + dice[1]) % 2 == wager;
    }

    public int getWinCount() {
        return winCount;
    }

    public int[] getDice() {
        return dice;
    }

    private String diceString(int value) {
        switch (value) {
            case 1:
                return "\u2680";
            case 2:
                return "\u2681";
            case 3:
                return "\u2682";
            case 4:
                return "\u2683";
            case 5:
                return "\u2684";
            case 6:
                return "\u2685";
        }
        return null;
    }

    public String getDiceAsString() {
        return diceString(dice[0]) + diceString(dice[1]);
    }

    public void shakeCup() {
        dice[0] = random.nextInt(6) + 1;
        dice[1] = random.nextInt(6) + 1;
    }

    public void reset() {
        wager = -1;
    }
}
