<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<jsp:include page="components/header.jsp">
    <jsp:param name="title" value="Hlavní strana"/>
</jsp:include>
<body>
<jsp:include page="components/top-panel.jsp" />
<jsp:include page="components/menu.jsp" />

<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="maven">
    <div class="row">
        <ol class="breadcrumb">
            <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
            <li>Maven</li>
            <li class="active">Soubor pom.xml</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Maven: Soubor pom.xml</h1>
        </div>
    </div>

    <div class="row no-margin">
        <div class="panel panel-primary">
            <div class="panel-heading">Soubor pom.xml</div>
            <div class="panel-body">
                <p class="custom-text-block">Soubor <i>pom.xml</i> (POM = Project Object Model) obsahuje informace pro kompilaci a sestavení programu
                    (závislosti, konfigurace pluginů,...).</p>
            </div>
        </div>
    </div><!-- Soubor pom.xml? -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Konfigurace jednotlivých pluginu</div>
            <div class="panel-body">
                <p class="custom-text-block">Konfigurace lze provést v souboru <i>pom.xml</i> Můžeme konfigurovat již existující pluginy nebo přidat nové.</p>
            </div>
            <div class="panel-footer">
                <section>
                    <h4>Příklad: plugin pro generování javadoc</h4>
                    <pre class="custom-code"><code class="xml">&lt;project&gt;
    ...
    &lt;build&gt;
        ...
        &lt;plugins&gt;
            &lt;plugin&gt;
                &lt;groupId&gt;org.apache.maven.plugins&lt;/groupId&gt;
                &lt;artifactId&gt;maven-javadoc-plugin&lt;/artifactId&gt;
                &lt;version&gt;3.0.0&lt;/version&gt;
                &lt;configuration&gt;
                    &lt;!-- Nastavení cílové složky pro generování dokumentace --&gt;
                    &lt;reportOutputDirectory&gt;&#36;{project.basedir}&lt;/reportOutputDirectory&gt;
                    &lt;destDir&gt;documentation&lt;/destDir&gt;
                &lt;/configuration&gt;
            &lt;/plugin&gt;
        &lt;/plugins&gt;
        ...
    &lt;/build&gt;
    ...
&lt;/project&gt;</code></pre>
                </section>
            </div>
        </div>
    </div><!-- Konfigurace pluginu -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Přidání nových závislostí</div>
            <div class="panel-body">
                <p>Na webu (<i>https://mvnrepository.com</i>) jsou dostupné veřejné balíčky.
                    Pro přidání balíčku do projektu stačí na webu najít potřebný balíček, zkopírvat o něm informace a vložit je do pom.xml projektu.
                </p>
            </div>
            <div class="panel-footer">
                <h4>Příklad: přidání jUnit</h4>
                <p>
                    Z <i>https://mvnrepository.com/artifact/junit/junit/4.12</i> stačí zkopírovat informace a vložit je do <i>pom.xml</i>.
                </p>
                <pre class="custom-code"><code class="xml">&lt;project&gt;
    ...
    &lt;dependencies&gt;
        ...
        &lt;dependency&gt;
            &lt;groupId&gt;junit&lt;/groupId&gt;
            &lt;artifactId&gt;junit&lt;/artifactId&gt;
            &lt;version&gt;4.12&lt;/version&gt;
            &lt;scope&gt;junit&lt;/scope&gt;
        &lt;/dependency&gt;
        ...
    &lt;/dependencies&gt;
    ...
&lt;/project&gt;
</code></pre>
            </div>
        </div>
    </div><!-- Přidání nových závislostí -->


    <jsp:include page="components/footer.jsp" />
</body>
</html>