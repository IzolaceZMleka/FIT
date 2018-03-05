package model.entity;

/**
 * Class DruhZvireteEntity
 * Entity class for DruhZvirete table.
 *
 * @author Ondřej Zemánek
 */
public class DruhZvireteEntity implements IEntity {

    public int id;
    public String jmenoDruhu;
    public String celed;
    public String oblast;
    public String info;

    public DruhZvireteEntity(int id, String jmenoDruhu, String celed, String oblast, String info) {
        this.id = id;
        this.jmenoDruhu = jmenoDruhu;
        this.celed = celed;
        this.oblast = oblast;
        this.info = info;
    }
}
