package model.entity;

import java.util.Date;

/**
 * Class UmisteniEntity
 * Entity class for Umisteni table.
 *
 * @author Ondřej Zemánek
 */
public class UmisteniEntity implements IEntity {

    public int id;
    public Integer idVybehu;
    public Integer idZvire;
    public Date umisteniOd;
    public Date umisteniDo;

    public UmisteniEntity(int id, Integer idVybehu, Integer idZvire, Date umisteniOd, Date umisteniDo) {
        this.id = id;
        this.idVybehu = idVybehu;
        this.idZvire = idZvire;
        this.umisteniOd = umisteniOd;
        this.umisteniDo = umisteniDo;
    }
}
