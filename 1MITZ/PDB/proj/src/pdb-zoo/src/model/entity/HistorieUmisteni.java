package model.entity;

import java.sql.Timestamp;

/**
 * Class for storing history of animal Enclosures
 *
 * @author Daniel Vosáhlo
 */
public class HistorieUmisteni implements IEntity{
    public int id;
    public String jmenoVybehu;
    public String jmenoZvirete;
    public Timestamp umisteniOd;
    public Timestamp umisteniDo;
    
    public HistorieUmisteni(int id, String jmenoV, String jmenoZ, Timestamp umOd, Timestamp umDo){
        this.id = id;
        this.jmenoVybehu = jmenoV;
        this.jmenoZvirete = jmenoZ;
        this.umisteniOd = umOd;
        this.umisteniDo = umDo;
    }
}




