<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Příprava a konfigurace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Příprava a konfigurace</li>
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
                        <div class="panel-heading">Struktura projektu</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Ve vytvářených projektech budeme využívat nástroj pro správu aplikace Maven. V rámci něj si nejdříve vytvoříme konfigurační soubor pom.xml, který umístíme do kořenového adresáře. 
                                Následně pak složka src bude obsahovat veškeré zdrojové kódy aplikace a další nutné zdroje (např. šablony, obrázky apod.). Celá adresařová struktura může vypadat jako na níže uvedeném příkladu.</p> 
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="asciidoc hljs">└── src 
    └── main
        └── java
            └── fit
                └── gja
                    └── Application.java
                    └── configuration
                    └── controller
        └── resources
            └── static
        └── webapp
            └── WEB-INF
                └── jsp
└── pom.xml</code>
                                </pre>
                            </div>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Základní konfigurace</div>
                        <div class="panel-body">
                            <h3 class="custom-h3">Spring Boot</h3>
                            <p class="custom-text-block">
                                Nejjednodušší způsob jakým lze vytvořit nový projekt je <b>Spring boot</b>, který je dostupný na adrese <a href="http://www.projects.spring.io/spring-boot/" target="_blank">www.projects.spring.io/spring-boot</a>,
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
                                lze získat na <a href="http://www.projects.spring.io/spring-boot/" target="_blank">výše uvedené stránce</a> 
                                Základní informace k výchozímu aplikačnímu serveru Wildfly jsou uvedené níže.
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Wildfly konfigurace</div>
                        <div class="panel-body">
                            <p class="custom-text-block">V případě užití aplikačního serveru Wildfly je nutné upravit zejména pom.xml do kterého přidáme direktivu pro vytvoření WAR souboru při buildu.</p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;packaging&gt;war&lt;packaging&gt;</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Kromě způsobu zabalení výsledné aplikace musíme rovněž z knihovny <i>spring-boot-starter-web</i> vyjmout závislost na Tomcatu. To uděláme pomocí výjímky ze závislostí uvnitř dané knihovny.</p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;exclusions&gt;
    &lt;exclusion&gt;
        &lt;groupId&gt;org.springframework.boot&lt;/groupId&gt;
        &lt;artifactId&gt;spring-boot-starter-tomcat&lt;/artifactId&gt;
    &lt;/exclusion&gt;
&lt;/exclusions&gt;</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Celá definice závislosti <i>spring-boot-starter-web</i> tak bude vypadat následovně.</p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;dependency&gt;
    &lt;groupId&gt;org.springframework.boot&lt;/groupId&gt;
    &lt;artifactId&gt;spring-boot-starter-web&lt;/artifactId&gt;
    &lt;exclusions&gt;
        &lt;exclusion&gt;
            &lt;groupId&gt;org.springframework.boot&lt;/groupId&gt;
            &lt;artifactId&gt;spring-boot-starter-tomcat&lt;/artifactId&gt;
        &lt;/exclusion&gt;
    &lt;/exclusions&gt;
&lt;/dependency&gt;</code>
                                </pre>
                            </div>

                            <p class="custom-text-block">Volitelnou možností je v tomto případě přidání konfiguračních souboru (např. nastavení cesty ke statickým zdrojům) nebo úprava výchozí třídy Application.java.</p>
                            <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>

                        </div>
                    </div>
                </div>
            </div>

        </div>	<!--/.main-->

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>