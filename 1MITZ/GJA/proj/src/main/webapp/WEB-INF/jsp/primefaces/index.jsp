<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="PrimeFaces: Základní informace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>PrimeFaces</li>
                    <li class="active">Základní informace</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">PrimeFaces: Základní informace</h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Co je PrimeFaces?</div>
                    <div class="panel-body">
                        <p class="custom-text-block">PrimeFaces je sada komponent pro použití v aplikacích využívající JSF. PrimeFaces obsahují přes 100 UI komponent, AJAX framework, validace na straně klienta a další.</p>
                        <p class="custom-text-block"><strong>Příklady zde obsahují pouze několik zajímavých komponent, kompletní výčet včetně ukázek je v <a href="https://www.primefaces.org/showcase/">dokumentaci PrimeFaces</a>.</strong></p>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Příklad PrimeFaces stránky</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Stránka s PrimeFaces je strukturou stejná jako JSF, pouze navíc obsahuje XML namespace <code>p</code> přes které jsou dostupné všechny komponenty:</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.01 Transitional//EN&quot; &quot;http://www.w3.org/TR/html4/loose.dtd&quot;&gt;
&lt;html xmlns=&quot;http://www.w3.org/1999/xhtml&quot;
      xmlns:h=&quot;http://java.sun.com/jsf/html&quot;
      xmlns:f=&quot;http://java.sun.com/jsf/core&quot;
      xmlns:p=&quot;http://primefaces.org/ui&quot;&gt;
    &lt;h:head&gt;
        &lt;title&gt;PrimeFaces&lt;/title&gt;
    &lt;/h:head&gt;
    &lt;h:body&gt;
        Hello PrimeFaces
    &lt;/h:body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>