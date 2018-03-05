package gui;
import javafx.beans.property.SimpleStringProperty;

public class Zamestnanec {
	private final SimpleStringProperty ID;
    private final SimpleStringProperty jmeno;
    private final SimpleStringProperty prijmeni;
    private final SimpleStringProperty login;
    
    public Zamestnanec(String ID, String jmeno, String prijmeni, String login) {
        this.ID = new SimpleStringProperty(ID);
        this.jmeno = new SimpleStringProperty(jmeno);
        this.prijmeni = new SimpleStringProperty(prijmeni);
        this.login = new SimpleStringProperty(login);
    }
    
 
    public String getID() {
        return ID.get();
    }

    public void setID(String ID) {
    	this.ID.set(ID);
    }

    public String getJmeno() {
        return jmeno.get();
    }

    public void setJmeno(String jmeno) {
    	this.jmeno.set(jmeno);
    }

    public String getPrijmeni() {
        return prijmeni.get();
    }

    public void setPrijmeni(String prijmeni) {
    	this.prijmeni.set(prijmeni);
    }
    
    public String getLogin() {
        return login.get();
    }

    public void setLogin(String login) {
    	this.login.set(login);
    }
    
    

}

