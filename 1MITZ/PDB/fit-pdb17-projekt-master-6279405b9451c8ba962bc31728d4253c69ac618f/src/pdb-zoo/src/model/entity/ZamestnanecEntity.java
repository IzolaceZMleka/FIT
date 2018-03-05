package model.entity;

/**
 * Class ZamestnanecEntity
 * Entity class for Zamestnanec table.
 *
 * @author Ondřej Zemánek
 */
public class ZamestnanecEntity implements IEntity {

    public int id;
    public String jmeno;
    public String prijmeni;
    public String login;
    public String heslo;

    public ZamestnanecEntity(int id, String jmeno, String prijmeni, String login, String heslo) {
        this.id = id;
        this.jmeno = jmeno;
        this.prijmeni = prijmeni;
        this.login = login;
        this.heslo = heslo;
    }
}
