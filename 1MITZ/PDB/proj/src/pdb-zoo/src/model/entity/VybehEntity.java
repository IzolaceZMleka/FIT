package model.entity;

/**
 * Class VybehEntity
 * Entity class for Vybeh table.
 *
 * @author Ondřej Zemánek
 */
public class VybehEntity implements IEntity {

    public int id;
    public String jmeno;
    public String info;

    public VybehEntity(int id, String jmeno, String info) {
        this.id = id;
        this.jmeno = jmeno;
        this.info = info;
    }
}
