<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSF: Základní informace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSF</li>
                    <li class="active">Základní informace</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSF: Základní informace</h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Co je JSF?</div>
                    <div class="panel-body">
                        <p class="custom-text-block">JavaServer Faces (JSF) je framework pro vytváření HTML stránek. Je založen na komponentách, které jsou součástí základní distribuce, ale také lze snadno definovat vlastní komponenty. Je založeno na architektuře Model-View-Controller.</p>
                        <p class="custom-text-block">JSF aplikace se skládá ze samotných stránek JSF (přípona <code>.xhtml</code>), backing beanů (JavaBean komponenty), konfigurace (viz dále), validátorů, listenerů a definic vlastních komponent.</p>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Konfigurace</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Konfigurace JSF se nachází v souboru <code>WEB-INFO/faces-config.xml</code> a definují se v ní backing beany, navigace, komponenty a další.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="xml">&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
&lt;faces-config
    xmlns=&quot;http://xmlns.jcp.org/xml/ns/javaee&quot;
    xmlns:xsi=&quot;http://www.w3.org/2001/XMLSchema-instance&quot;
    xsi:schemaLocation=&quot;http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-facesconfig_2_2.xsd&quot;
    version=&quot;2.2&quot;&gt;
&lt;/faces-config&gt;
</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>