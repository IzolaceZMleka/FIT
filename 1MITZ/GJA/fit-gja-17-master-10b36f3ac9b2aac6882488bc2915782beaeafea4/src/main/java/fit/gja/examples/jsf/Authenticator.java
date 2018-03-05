package fit.gja.examples.jsf;

import javax.inject.Named;
import javax.enterprise.context.RequestScoped;

/**
 *
 * @author Filip Pobořil
 */
@Named(value = "authenticator")
@RequestScoped
public class Authenticator {

    private String name;
    private String password;
    private boolean remember;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public boolean isRemember() {
        return remember;
    }

    public void setRemember(boolean remember) {
        this.remember = remember;
    }

    public void authenticate() {
        // do something ...
    }

}
