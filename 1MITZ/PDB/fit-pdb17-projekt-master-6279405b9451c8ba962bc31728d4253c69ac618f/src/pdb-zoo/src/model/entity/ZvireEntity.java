package model.entity;

import java.util.Date;

import oracle.ord.im.OrdImage;

/**
 * Class ZvireEntity
 * Entity class for Zvire table.
 *
 * @author Ondřej Zemánek
 */
public class ZvireEntity implements IEntity {

    public int id;
    public Integer idDruh;
    public String jmeno;
    public String pohlavi;
    public Date datumNarozeni;
    public Date datumUmrti;
    public Date datumPorizeniFotky;
    public OrdImage fotka;

    public ZvireEntity(int id, Integer idDruh, String jmeno, String pohlavi,
                       Date datumNarozeni, Date datumUmrti, Date datumPorizeniFotky,
                       OrdImage fotka) {
        this.id = id;
        this.idDruh = idDruh;
        this.jmeno = jmeno;
        this.pohlavi = pohlavi;
        this.datumNarozeni = datumNarozeni;
        this.datumUmrti = datumUmrti;
        // If this is NULL the animal dont have a photo
        this.datumPorizeniFotky = datumPorizeniFotky;
        this.fotka = fotka;
    }
}
