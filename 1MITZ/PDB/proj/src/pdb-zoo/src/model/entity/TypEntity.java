package model.entity;

import javafx.scene.paint.Color;

/**
 * Class TypEntity
 * Entity class for Typ table.
 *
 * @author Ondřej Zemánek
 */
public class TypEntity implements IEntity {

    public int id;
    public String jmeno;
    public Color barva;

    public TypEntity(int id, String jmeno, String barva) {
        this.id = id;
        this.jmeno = jmeno;
        this.barva = Color.web(barva);
    }
}
