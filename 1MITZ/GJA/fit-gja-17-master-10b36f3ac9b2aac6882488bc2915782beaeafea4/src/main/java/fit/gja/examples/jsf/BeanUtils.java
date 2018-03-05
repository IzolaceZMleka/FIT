package fit.gja.examples.jsf;

import javax.faces.context.FacesContext;

/**
 * @author Filip Pobořil
 */
public class BeanUtils {

    /**
     * Get managed been by name.
     *
     * @param name
     * @return bean
     */
    public static Object getBean(String name) {
        // Get bean instance
        FacesContext context = FacesContext.getCurrentInstance();
        return context.getELContext().getELResolver()
                .getValue(context.getELContext(), null, name);
    }

}
