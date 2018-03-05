<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSP: Základní informace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSP</li>
                    <li class="active">Základní informace</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSP: Základní informace</h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Co je JSP?</div>
                    <div class="panel-body">
                        <p class="custom-text-block">JavaServer Pages (JSP) je technologie pro vývoj dynamických HTML stránek založená na Javě. Umožňuje do HTML stránek vkládat kód Javy. Přípona souborů je .jsp.</p>
                        <p class="custom-text-block">Z pohledu programátora se jedná o HTML stránky s vloženým kódem Javy. Na straně servlet kontejneru funguje ale jako běžný servlet.</p>
                        <p class="custom-text-block">Při prvním požadavku na zobrazení JSP stránky servlet kontejner načte .jsp soubor a převede ho na běžnou Java třídu servletu. Pokud dojde ke změně zdrojového .jsp souboru, servlet kontejner to zaznamená a invaliduje vygenerovaný servlet a při dalším požadavku se provede generování znovu, pro nový .jsp soubor. Po převodu na servlet se JSP chová stejně jako kterýkoliv jiný servlet. </p>
                        <p class="custom-text-block"><strong>JSF je starší technologie, pro nové projekty je vhodnější používat JSF (JavaServer Faces).</strong></p>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Příklad JSP stránky</div>
                    <div class="panel-body">
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">&lt;%@page contentType=&quot;text/html&quot; pageEncoding=&quot;UTF-8&quot;%&gt;
&lt;!DOCTYPE html&gt;
&lt;html&gt;
    &lt;head&gt;
        &lt;meta http-equiv=&quot;Content-Type&quot; content=&quot;text/html; charset=UTF-8&quot;&gt;
        &lt;title&gt;JSP Page&lt;/title&gt;
    &lt;/head&gt;
    &lt;body&gt;
        &lt;h1&gt;Hello World!&lt;/h1&gt;
        &lt;p&gt;Current time: &lt;%= new java.util.Date() %&gt;&lt;/p&gt;
    &lt;/body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>