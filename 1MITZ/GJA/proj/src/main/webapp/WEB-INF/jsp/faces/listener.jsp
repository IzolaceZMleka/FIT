<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSP: Listener"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSF</li>
                    <li class="active">Listener</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSF: Listener
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/faces/listener.xhtml" class="btn btn-success">Živá ukázka</a>
                            <a href="${pageContext.request.contextPath}/example/jsf-all.zip" class="btn btn-success">Zdrojové kódy</a>
                        </span>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">JSF</div>
                    <div class="panel-body">
                        <p class="custom-text-block">V tomto příkladu je ukázka zpracování událostí v JSF. Výběrem nějaké položky ze seznamu se vyvolá zadaná akce. V prvním případě je to metoda na beanu <code>Language.selectedChange</code>. U druhého formuláře je to speciální třída <code>LanguageChangeListener</code>.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;
      xmlns:f=&quot;http://java.sun.com/jsf/core&quot;&gt;
    &lt;h:head&gt;
        &lt;title&gt;Facelet Title&lt;/title&gt;
    &lt;/h:head&gt;

    &lt;h:body&gt;
        &lt;h:form&gt;
            &lt;div style=&quot;margin: 10px&quot;&gt;
                &lt;h:selectOneMenu value=&quot;&num;{language.selected}&quot;
                                 onchange=&quot;submit()&quot;
                                 valueChangeListener=&quot;&num;{language.selectedChange}&quot; &gt;
                    &lt;f:selectItems value=&quot;&num;{language.available}&quot; /&gt;
                &lt;/h:selectOneMenu&gt;
            &lt;/div&gt;
        &lt;/h:form&gt;

        &lt;h:form&gt;
            &lt;div style=&quot;margin: 10px&quot;&gt;
                &lt;h:selectOneMenu value=&quot;&num;{language.selected}&quot;
                                 onchange=&quot;submit()&quot;&gt;
                    &lt;f:valueChangeListener type=&quot;example.jsf.listener.LanguageChangeListener&quot; /&gt;
                    &lt;f:selectItems value=&quot;&num;{language.available}&quot; /&gt;
                &lt;/h:selectOneMenu&gt;
            &lt;/div&gt;
        &lt;/h:form&gt;

        &lt;h1&gt;&lt;h:outputText value=&quot;&num;{language.greeting}&quot;/&gt;&lt;/h1&gt;
    &lt;/h:body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Bean</div>
                    <div class="panel-body">
                        <p class="custom-text-block">V tomto beanu je zajímavá metoda <code>selectedChange</code>, která odchytává událost vyvolanou změnou hodnoty ve formuláři. Tato metoda přijímá jako parametr danou událost, objekt ze kterého je možné získat jak původní tak i novou hodnotu daného formulářového prvku. V tomto příkladu se použije nová hodnota a přiřadí se do atributu instance.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.jsf.bean;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.enterprise.context.SessionScoped;
import javax.inject.Named;
import javax.faces.event.ValueChangeEvent;

@Named(value = &quot;language&quot;)
@SessionScoped
public class Language implements Serializable {

    private static Map&lt;String, String&gt; available;

    private String selected;

    static {
        available = new HashMap&lt;&gt;();
        available.put(&quot;Če&scaron;tina&quot;, &quot;cs&quot;);
        available.put(&quot;English&quot;, &quot;en&quot;);
        available.put(&quot;日本語&quot;, &quot;jp&quot;);
    }

    public Map&lt;String, String&gt; getAvailable() {
        return available;
    }

    public String getSelected() {
        return selected == null ? &quot;en&quot; : selected;
    }

    public void setSelected(String selected) {
        this.selected = selected;
    }

    public void selectedChange(ValueChangeEvent e) {
        setSelected(e.getNewValue().toString());
    }

    public String getGreeting() {
        switch (getSelected()) {
            case &quot;en&quot;:
            default:
                return &quot;Hello&quot;;
            case &quot;cs&quot;:
                return &quot;Ahoj&quot;;
            case &quot;jp&quot;:
                return &quot;こんにちは&quot;;
        }
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Listener</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Listener funguje podobně jako metoda z beanu, pouze je v něm navíc nutné získat instanci beanu, do kterého se má přiřadit hodnota.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.jsf.listener;

import javax.faces.event.*;
import example.jsf.bean.Language;
import static example.jsf.utils.BeanUtils.getBean;

public class LanguageChangeListener implements ValueChangeListener {
    @Override
    public void processValueChange(ValueChangeEvent event)
            throws AbortProcessingException {
        Language languageBean = (Language) getBean("language");
        languageBean.setSelected(event.getNewValue().toString());
    }
}</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.jsf.utils;

import javax.faces.context.FacesContext;

public class BeanUtils {
    /**
     * Get managed been by name.
     */
    public static Object getBean(String name) {
        // Get bean instance
        FacesContext context = FacesContext.getCurrentInstance();
        return context.getELContext().getELResolver()
                .getValue(context.getELContext(), null, name);
    }
}
</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>