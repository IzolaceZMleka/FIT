<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSP: Vlastní komponenty"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSF</li>
                    <li class="active">Vlastní komponenty</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSF: Vlastní komponenty
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/faces/composite.xhtml" class="btn btn-success">Živá ukázka</a>
                            <a href="${pageContext.request.contextPath}/example/jsf-all.zip" class="btn btn-success">Zdrojové kódy</a>
                        </span>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">JSF</div>
                    <div class="panel-body">
                        <p class="custom-text-block">XML namespace pro vlastní komponenty se skládá z <code>http://xmlns.jcp.org/jsf/composite/</code> následovaného adresářem, kde se definice nacházejí. V tomto příkladu je definice komponent umístěna v adresáři <code>custom</code>. Adresáře s definicemi komponent se musejí nacházet v adresáři <code>resources</code>.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;
      xmlns:custom=&quot;http://xmlns.jcp.org/jsf/composite/custom&quot;&gt;
    &lt;!-- &quot;custom&quot; reffers to folder name --&gt;
    &lt;h:head&gt;
        &lt;title&gt;Custom component&lt;/title&gt;
    &lt;/h:head&gt;
    &lt;h:body&gt;
        &lt;h1&gt;Custom component&lt;/h1&gt;

        &lt;custom:login nameLabel=&quot;Name&quot;
                      nameValue=&quot;&num;{authenticator.name}&quot;
                      passwordLabel=&quot;Password&quot;
                      passwordValue=&quot;&num;{authenticator.password}&quot;
                      rememberLabel=&quot;Remember&quot;
                      rememberValue=&quot;&num;{authenticator.remember}&quot;
                      submitText=&quot;Login&quot;
                      submitAction=&quot;&num;{authenticator.authenticate()}&quot; /&gt;
    &lt;/h:body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>
            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Definice komponenty</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Definice komponenty se nachází v souboru <code>resources/custom/login.xhtml</code>. Nejprve se definuje rozhraní komponenty, to jaké bude mít atributy apod. Dále se komponenta implementuje, tj. z jakých komponent, HTML elementů se bude skládat apod.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;
      xmlns:f=&quot;http://java.sun.com/jsf/core&quot;
      xmlns:composite=&quot;http://java.sun.com/jsf/composite&quot;&gt;

    &lt;composite:interface&gt;
        &lt;composite:attribute name=&quot;nameLabel&quot; /&gt;
        &lt;composite:attribute name=&quot;nameValue&quot; /&gt;

        &lt;composite:attribute name=&quot;passwordLabel&quot; /&gt;
        &lt;composite:attribute name=&quot;passwordValue&quot; /&gt;

        &lt;composite:attribute name=&quot;rememberLabel&quot; /&gt;
        &lt;composite:attribute name=&quot;rememberValue&quot; /&gt;

        &lt;composite:attribute name=&quot;submitText&quot; /&gt;
        &lt;composite:attribute name=&quot;submitAction&quot;
                             method-signature=&quot;java.lang.String action()&quot; /&gt;
    &lt;/composite:interface&gt;

    &lt;composite:implementation&gt;

        &lt;h:form&gt;
            &lt;h:message for=&quot;textPanel&quot; /&gt;

            &lt;h:panelGrid columns=&quot;2&quot; id=&quot;textPanel&quot;&gt;
                &lt;h:outputLabel for=&quot;name&quot; value=&quot;&num;{cc.attrs.nameLabel}&quot; /&gt;
                &lt;h:inputText id=&quot;name&quot; value=&quot;&num;{cc.attrs.nameValue}&quot; /&gt;

                &lt;h:outputLabel for=&quot;password&quot; value=&quot;&num;{cc.attrs.passwordLabel}&quot; /&gt;
                &lt;h:inputSecret id=&quot;password&quot; value=&quot;&num;{cc.attrs.passwordValue}&quot; /&gt;

                &lt;h:outputLabel for=&quot;remember&quot; value=&quot;&num;{cc.attrs.rememberLabel}&quot; /&gt;
                &lt;h:selectBooleanCheckbox id=&quot;remember&quot; value=&quot;&num;{cc.attrs.rememberValue}&quot; /&gt;
            &lt;/h:panelGrid&gt;

            &lt;h:commandButton action=&quot;&num;{cc.attrs.submitAction}&quot;
                             value=&quot;&num;{cc.attrs.submitText}&quot; /&gt;
        &lt;/h:form&gt;

    &lt;/composite:implementation&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Bean</div>
                    <div class="panel-body">
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.jsf.bean;

import javax.inject.Named;
import javax.enterprise.context.RequestScoped;

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
}</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>