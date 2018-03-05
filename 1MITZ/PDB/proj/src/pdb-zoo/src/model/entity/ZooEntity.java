package model.entity;

import oracle.spatial.geometry.JGeometry;

/**
 * Class ZooEntity
 * Entity class for Zoo table.
 *
 * @author Ondřej Zemánek
 */
public class ZooEntity implements IEntity {

    public int id;
    public Integer idVybehu;
    public Integer idTyp;
    public JGeometry prostorovaData;

    public ZooEntity(int id, Integer idVybehu, Integer idTyp, JGeometry prostorovaData) {
        this.id = id;
        this.idVybehu = idVybehu;
        this.idTyp = idTyp;
        this.prostorovaData = prostorovaData;
    }
}
