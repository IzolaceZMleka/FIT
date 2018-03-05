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
            <li class="active">Základní informace</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Maven: Základní informace</h1>
        </div>
    </div>

    <div class="row no-margin">
        <div class="panel panel-primary">
            <div class="panel-heading">Co je maven?</div>
            <div class="panel-body">
                Maven je systém pro správu a sestavování aplikací postavených nad platformou Java. Všechny informace pro
                kompilaci a sestavení programu jsou uvedeny ve speciálním souboru <a href="${pageContext.request.contextPath}/maven/pom">
                <i>pom.xml</i> (POM = Project Object Model)</a>, díky tomu odpadá závislost na použitém IDE.
            </div>
        </div>
    </div><!-- Co je maven? -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Konfigurace</div>
            <div class="panel-body">
                Možnosti konfigurace:
                <ul>
                    <li>settings.xml</li>
                    <ul>
                        <li>nachází se v <i>~/.m2/</i></li>
                        <li>obsahuje konfiguraci sdílenou mezi projekty</li>
                    </ul>
                    <li>.mvn</li>
                    <ul>
                        <li>nachází se v <i>ROOT_PROJEKTU/</i></li>
                        <li>obsahuje soubory <i>maven.config</i> a <i>extensions.xml</i></li>
                        <li>mění konfiguraci v rámci daného projektu</li>
                    </ul>
                </ul>
            </div>
            <div class="panel-footer">
                <section>
                    <h4>Příklad: settings.xml a konfigurace serveru Oracle </h4>
                    <pre class="custom-code"><code class="xml">&lt;settings xmlns="http://maven.apache.org/SETTINGS/1.0.0"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0
          https://maven.apache.org/xsd/settings-1.0.0.xsd"&gt;
    &lt;localRepository/&gt;
    &lt;interactiveMode/&gt;
    &lt;usePluginRegistry/&gt;
    &lt;offline/&gt;
    &lt;pluginGroups/&gt;
    &lt;servers&gt;
        &lt;server&gt;
            &lt;id&gt;maven.oracle.com&lt;/id&gt;
            &lt;username&gt;username&lt;/username&gt;
            &lt;password&gt;password&lt;/password&gt;
            &lt;configuration&gt;
                &lt;basicAuthScope&gt;
                    &lt;host&gt;ANY&lt;/host&gt;
                    &lt;port&gt;ANY&lt;/port&gt;
                    &lt;realm&gt;OAM 11g&lt;/realm&gt;
                &lt;/basicAuthScope&gt;
                &lt;httpConfiguration&gt;
                    &lt;all&gt;
                        &lt;params&gt;
                            &lt;property&gt;
                                &lt;name&gt;http.protocol.allow-circular-redirects&lt;/name&gt;
                                &lt;value&gt;%b,true&lt;/value&gt;
                            &lt;/property&gt;
                        &lt;/params&gt;
                    &lt;/all&gt;
                &lt;/httpConfiguration&gt;
            &lt;/configuration&gt;
        &lt;/server&gt;
    &lt;/servers&gt;
    &lt;mirrors/&gt;
    &lt;proxies/&gt;
    &lt;profiles/&gt;
    &lt;activeProfiles/&gt;
&lt;/settings&gt;</code></pre>
                </section>
            </div>
        </div>
    </div><!-- Možnosti konfigurace -->
    <div id="maven-lifecycle" class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Životní cyklus (základní fáze)</div>
            <div class="panel-body">
                <section>
                    <h4>validate</h4>
                    <p>ověří správnost projektu a dostupnost všech potřebných informací</p>
                </section>
                <div class="arrow-down"></div>
                <section>
                    <h4>compile</h4>
                    <p>kompilace zdrojového kódu</p>
                </section>
                <div class="arrow-down"></div>
                <section>
                    <h4>test</h4>
                    <p>otestuje kompilovaný zdrojový kód</p>
                </section>
                <div class="arrow-down"></div>
                <section>
                    <h4>package</h4>
                    <p>z kompilovaného kódu vytvoří balíček (jar)</p>
                </section>
                <div class="arrow-down"></div>
                <section>
                    <h4>verify</h4>
                    <p>provede akce pro kontrolu kvality a splnění kritérii (integrační testy)</p>
                </section>
                <div class="arrow-down"></div>
                <section>
                    <h4>install</h4>
                    <p>nainstaluje balíček do lokální uložitě pro použítí v jiných projektech</p>
                </section>
                <div class="arrow-down"></div>
                <section>
                    <h4>deploy</h4>
                    <p>zkopíruje konečný balíček do vzdáleného uložíště pro sdílení</p>
                </section>
            </div>
            <div class="panel-footer">
                <h4>Příklad spuštění</h4>
                <div class="custom-code-block">
                    <pre class="custom-code"><code class="script">$ mvn NAZEV_FAZE
$ mvn compile</code></pre>
                </div>
                Pro zadanou fázi se provedou i ty předcházející. Takže pro compile se provede nejdříve validate a poté compile.
            </div>
        </div>
    </div><!-- Životní cyklus -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Vytvoření nového projektu</div>
            <div class="panel-body">
                <p>Vygneruje se:</p>
                <ul>
                    <li>základní adresářová struktura</li>
                    <li>pom.xml</li>
                </ul>
                <h4>Příklad:</h4>
                <pre class="custom-code"><code class="script">$ mvn archetype:create \
    -DgroupId=com.projekt.test \
    -DartifactId=hello_world \
    -DarchetypeArtifactId=maven-archetype-quickstart</code></pre>
            </div>
        </div>
    </div><!-- Vytvoření projektu -->

    <jsp:include page="components/footer.jsp" />
</body>
</html>