<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSP: AJAX"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSF</li>
                    <li class="active">AJAX</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSF: AJAX
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/faces/ajax.xhtml" class="btn btn-success">Živá ukázka</a>
                            <a href="${pageContext.request.contextPath}/example/jsf-all.zip" class="btn btn-success">Zdrojové kódy</a>
                        </span>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">JSF</div>
                    <div class="panel-body">
                        <p class="custom-text-block">AJAX jde k formuláři přidat poměrně snadno, stačí do prvku <code>h:commandButton</code> přidat <code>&lt;f:ajax execute=&quot;input&quot; render=&quot;output&quot; /&gt;</code>. Atribut <code>input</code> obsahuje seznam oddělený mezerami ID komponent, které se mají odeslat AJAX požadavkem a atribut <code>render</code> obsahuje ID komponent, které se mají po dokončení překreslit.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;
      xmlns:f=&quot;http://java.sun.com/jsf/core&quot;&gt;
    &lt;h:head&gt;
        &lt;title&gt;AJAX&lt;/title&gt;
    &lt;/h:head&gt;
    &lt;h:body&gt;
        &lt;h2&gt;Profile&lt;/h2&gt;
        &lt;h:panelGrid columns=&quot;2&quot; id=&quot;output&quot;&gt;
            &lt;h:outputLabel value=&quot;Nickname&quot; /&gt;
            &lt;h:outputText value=&quot;&num;{userProfile.nickname}&quot; /&gt;
            &lt;h:outputLabel value=&quot;Age&quot; /&gt;
            &lt;h:outputText value=&quot;&num;{userProfile.age}&quot; /&gt;
        &lt;/h:panelGrid&gt;

        &lt;h2&gt;Change&lt;/h2&gt;
        &lt;h:form&gt;
            &lt;h:panelGrid columns=&quot;2&quot; id=&quot;input&quot;&gt;
                &lt;h:outputLabel value=&quot;Nickname&quot; /&gt;
                &lt;h:inputText value=&quot;&num;{userProfile.nickname}&quot; /&gt;
                &lt;h:outputLabel value=&quot;Age&quot; /&gt;
                &lt;h:inputText value=&quot;&num;{userProfile.age}&quot; /&gt;
            &lt;/h:panelGrid&gt;

            &lt;h:commandButton value=&quot;Save&quot;&gt;
                &lt;f:ajax execute=&quot;input&quot; render=&quot;output&quot; /&gt;
            &lt;/h:commandButton&gt;
        &lt;/h:form&gt;
    &lt;/h:body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Bean</div>
                    <div class="panel-body">
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.jsf.bean;

import java.io.Serializable;
import javax.inject.Named;
import javax.enterprise.context.SessionScoped;

@Named(value = "userProfile")
@SessionScoped
public class UserProfile implements Serializable {

    private String nickname;
    private int age;
    private String language;

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public String getLanguage() {
        return language;
    }

    public void setLanguage(String language) {
        this.language = language;
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>