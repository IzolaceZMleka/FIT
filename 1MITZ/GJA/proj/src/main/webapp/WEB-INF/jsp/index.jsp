<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="components/header.jsp">
        <jsp:param name="title" value="Hlavní strana"/>
    </jsp:include>
    <body>
        <jsp:include page="components/top-panel.jsp" />
        <jsp:include page="components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Dashboard</li>
                </ol>
            </div><!--/.row-->

            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Spring: Příprava a konfigurace</h1>
                </div>
            </div><!--/.row-->

            <div clas="row">
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">Co je to Spring?</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Spring je populární open-source framework určený k vývoji Java aplikací. Jádro frameworku tvoří IoC kontejner, který je zodpovědný za vytvoření
                                a provázání objektů. Ty je pak možné získat pomocí Dependency Injection. Framework nabízí celou řadu modulů, které jsou organizovany do několika skupin
                                a usnadňuji tak samotný vývoj webových aplikací.
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Základní konfigurace</div>
                        <div class="panel-body">
                            <h3 class="custom-h3">Spring Boot</h3>
                            <p class="custom-text-block">
                                Nejjednodušší způsob jakým lze vytvořit nový projekt je <b>Spring boot</b>, který je dostupný na adrese <a href="www.projects.spring.io/spring-boot/">www.projects.spring.io/spring-boot</a>,
                                Tato knihovna umožňuje velmi rychlou přípravu aplikace bez nutnosti rozsahlého nastavení.</p>
                            <h4 class="custom-h4">Pom.xml konfigurace</h4>
                            <p class="custom-text-block">Po vytvoření nového Maven projektu je nutné přidat do souboru pom.xml následující závislost.</p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;parent&gt;
    &lt;groupId&gt;org.springframework.boot&lt;/groupId&gt;
    &lt;artifactId&gt;spring-boot-starter-parent&lt;/artifactId&gt;
    &lt;version&gt;1.5.9.RELEASE&lt;/version&gt;
    &lt;relativePath/&gt;
&lt;/parent&gt;

&lt;dependencies&gt;
    &lt;dependency&gt;
        &lt;groupId&gt;org.springframework.boot&lt;/groupId&gt;
        &lt;artifactId&gt;spring-boot-starter-web&lt;/artifactId&gt;
    &lt;/dependency&gt;
&lt;/dependencies&gt;</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Jelikož v následujících ukázkách budeme pracovat rovněž se šablonovacím systémem <b>ThymeLeaf</b>, přídáme ještě do Maven projektu tuto knihovnu.</p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;dependencies&gt;
    &lt;dependency&gt;
        &lt;groupId&gt;org.springframework.boot&lt;/groupId&gt;
        &lt;artifactId&gt;spring-boot-starter-thymeleaf&lt;/artifactId&gt;
    &lt;/dependency&gt;
&lt;/dependencies&gt;</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">
                                V závislosti na použitém aplikačním serveru bude nutné provést další nastavení, veškeré informace
                                lze získat na <a href="www.projects.spring.io/spring-boot/">výše uvedené stránce</a> 
                                Informace k výchozímu aplikačnímu serveru Wildfly jsou uvedené níže.
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Wildfly konfigurace</div>
                        <div class="panel-body">
                            Další informace
                        </div>
                        </di>
                    </div>
                </div>

            </div>	<!--/.main-->

            <jsp:include page="components/footer.jsp" />
    </body>
</html>