package fit.gja.examples.jsf;

import javax.faces.event.*;
import fit.gja.examples.jsf.Language;
import static fit.gja.examples.jsf.BeanUtils.getBean;

/**
 *
 * @author Filip Pobořil
 */
public class LanguageChangeListener implements ValueChangeListener {

    @Override
    public void processValueChange(ValueChangeEvent event)
            throws AbortProcessingException {
        Language languageBean = (Language) getBean("language");
        languageBean.setSelected(event.getNewValue().toString());
    }

}
