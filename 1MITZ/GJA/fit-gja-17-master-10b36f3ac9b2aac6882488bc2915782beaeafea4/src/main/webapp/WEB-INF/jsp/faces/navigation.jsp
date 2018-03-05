<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSP: Navigace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSF</li>
                    <li class="active">Navigace</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSF: Navigace
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/faces/navigation/index.xhtml" class="btn btn-success">Živá ukázka</a>
                            <a href="${pageContext.request.contextPath}/example/jsf-all.zip" class="btn btn-success">Zdrojové kódy</a>
                        </span>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">JSF</div>
                    <div class="panel-body">
                        <h4 class="custom-h4">index</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;
      xmlns:f=&quot;http://java.sun.com/jsf/core&quot;&gt;
    &lt;h:head&gt;
        &lt;title&gt;Navigation&lt;/title&gt;
        &lt;style&gt;a { margin-right: 2em; }&lt;/style&gt;
    &lt;/h:head&gt;
    &lt;h:body&gt;
        &lt;h1&gt;Navigation&lt;/h1&gt;

        &lt;h2&gt;Implicit navigation&lt;/h2&gt;
        &lt;h:form&gt;
            &lt;h3&gt;Using Navigation.showPage1()&lt;/h3&gt;
            &lt;h:commandButton action=&quot;&num;{navigation.showPage1}&quot;
                             value=&quot;Page1&quot; /&gt;
        &lt;/h:form&gt;

        &lt;h2&gt;Conditional navigation&lt;/h2&gt;
        &lt;h:form&gt;
            &lt;h3&gt;Using Navigation.show()&lt;/h3&gt;
            &lt;h:commandLink action=&quot;&num;{navigation.show}&quot;
                           value=&quot;Page1&quot;&gt;
                &lt;f:param name=&quot;pageId&quot; value=&quot;1&quot; /&gt;
            &lt;/h:commandLink&gt;
            &lt;h:commandLink action=&quot;&num;{navigation.show}&quot;
                           value=&quot;Page2&quot;&gt;
                &lt;f:param name=&quot;pageId&quot; value=&quot;2&quot; /&gt;
            &lt;/h:commandLink&gt;
            &lt;h:commandLink action=&quot;&num;{navigation.show}&quot;
                           value=&quot;Home&quot;&gt;
                &lt;f:param name=&quot;pageId&quot; value=&quot;3&quot; /&gt;
            &lt;/h:commandLink&gt;
        &lt;/h:form&gt;

        &lt;h2&gt;Forward vs redirect&lt;/h2&gt;
        &lt;p&gt;Keep eye on the URL.&lt;/p&gt;
        &lt;h:form&gt;
            &lt;h3&gt;Forward&lt;/h3&gt;
            &lt;h:commandButton action=&quot;page1&quot; value=&quot;Page1&quot; /&gt;
            &lt;h3&gt;Redirect&lt;/h3&gt;
            &lt;h:commandButton action=&quot;page1?faces-redirect=true&quot; value=&quot;Page1&quot; /&gt;
        &lt;/h:form&gt;
    &lt;/h:body&gt;
&lt;/html&gt;</code></pre></div>
                        <h4 class="custom-h4">page1</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;&gt;
    &lt;h:head&gt;
        &lt;title&gt;Page1&lt;/title&gt;
    &lt;/h:head&gt;
    &lt;h:body&gt;
        &lt;h1&gt;Page1&lt;/h1&gt;
        &lt;h:form&gt;
            &lt;h:commandButton action=&quot;index?faces-redirect=true&quot;
                             value=&quot;Back&quot; /&gt;
        &lt;/h:form&gt;
    &lt;/h:body&gt;
&lt;/html&gt;</code></pre></div>
                        <h4 class="custom-h4">page2</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;?xml version='1.0' encoding='UTF-8' ?&gt;
&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Transitional//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://xmlns.jcp.org/jsf/html&quot;&gt;
    &lt;h:head&gt;
        &lt;title&gt;Page2&lt;/title&gt;
    &lt;/h:head&gt;
    &lt;h:body&gt;
        &lt;h1&gt;Page2&lt;/h1&gt;
        &lt;h:form&gt;
            &lt;h:commandButton action=&quot;index?faces-redirect=true&quot;
                             value=&quot;Back&quot; /&gt;
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
import javax.enterprise.context.RequestScoped;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;

/**
 *
 * @author Filip Pobořil
 */
// Here must be older ManagedBean instead of Named because we need eager=true,
// which Named annotation not supports.
@ManagedBean(name = "navigation", eager = true)
@RequestScoped
public class Navigation implements Serializable {

    /**
     * This managed property will read value from request parameter pageId.
     */
    // Works only when eager is true.
    @ManagedProperty(value = "&num;{param.pageId}")
    private String pageId;

    public String showPage1() {
        return "page1";
    }

    public String showPage2() {
        return "page2";
    }

    // processPage1() and processPage2() returns same value but forwards to
    // different. Thats because navation-rule config in faces-config.xml.

    public String processPage1() {
        return "page";
    }

    public String processPage2() {
        return "page";
    }

    /**
     * Conditional navigation based on pageId.
     *
     * If pageId is 1 then show page1, if pageId is 2 then show page2, else show
     * index.
     *
     * @return
     */
    public String show() {
        System.out.println(pageId);
        if (pageId == null) {
            return "index";
        }
        if (pageId.equals("1")) {
            return "page1";
        } else if (pageId.equals("2")) {
            return "page2";
        } else {
            return "index";
        }
    }

    public String getPageId() {
        return pageId;
    }

    public void setPageId(String pageId) {
        this.pageId = pageId;
    }

}</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Konfigurace</div>
                    <div class="panel-body">
                        <p class="custom-text-block"><code>WEB-INFO/faces-config.xml</code></p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="xml">&lt;?xml version='1.0' encoding='UTF-8'?&gt;
&lt;faces-config version=&quot;2.2&quot;
              xmlns=&quot;http://xmlns.jcp.org/xml/ns/javaee&quot;
              xmlns:xsi=&quot;http://www.w3.org/2001/XMLSchema-instance&quot;
              xsi:schemaLocation=&quot;http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-facesconfig_2_2.xsd&quot;&gt;
    &lt;navigation-rule&gt;
        &lt;from-view-id&gt;index.xhtml&lt;/from-view-id&gt;
        &lt;navigation-case&gt;
            &lt;from-action&gt;&num;{navigation.processPage1}&lt;/from-action&gt;
            &lt;from-outcome&gt;page&lt;/from-outcome&gt;
            &lt;to-view-id&gt;page1.xhtml&lt;/to-view-id&gt;
        &lt;/navigation-case&gt;
        &lt;navigation-case&gt;
            &lt;from-action&gt;&num;{navigation.processPage2}&lt;/from-action&gt;
            &lt;from-outcome&gt;page&lt;/from-outcome&gt;
            &lt;to-view-id&gt;page2.xhtml&lt;/to-view-id&gt;
        &lt;/navigation-case&gt;
    &lt;/navigation-rule&gt;
&lt;/faces-config&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>