<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Dependency injection"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Dependency injection</li>
                </ol>
            </div><!--/.row-->

            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Spring: Dependency injection</h1>
                </div>
            </div><!--/.row-->

            <div clas="row">
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">Co je to Dependency injection?</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Metoda dependency injection (DI, česky vkládání závislosti) ve své podstatě rozšiřuje základní IoC kontejner. Tento návrhový vzor umožňuje objekty automaticky vytvořené frameworkem Spring provázat a doplnit nutné závislosti. Objekty jsou vzájemně provázané pomocí "set" metod nebo konstruktorů. 
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Constructor Dependency injection</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                V příkladu na vkládání závislosti do konstruktoru budeme pracovat se dvěma třídami - <i>Configurator</i>, která bude reprezentovat určité konfigurační nastavení a <i>CustomService</i>, která bude sloužit jako příklad určité služby. Abychom nemuseli instanci třídy <i>Configurator</i> přidávat do každé nové služby manuálně, využijeme nyní techniku Constructor Dependency injection. 
                            </p>
                            <p class="custom-text-block">
                                Nejdříve zaregistrujeme dané třídy v souboru beans.xml.
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;bean id=&quot;configurator&quot; class=&quot;fit.gja.beans.Configurator&quot;&gt;
    &lt;property name=&quot;option1&quot; value=&quot;Nastaveni hodnoty 1&quot;/&gt;
    &lt;property name=&quot;option2&quot; value=&quot;Nastaveni hodnoty 2&quot;/&gt;
&lt;/bean&gt;
  
&lt;bean id=&quot;customService&quot; class=&quot;fit.gja.services.CustomService&quot;&gt;&lt;/bean&gt;
</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">    
                                Poté již stačí uvést danou třídu <i>Configurator</i> jako nutný parametr pro konstruktor v rámci <i>CustomService</i>. O zbytek se již postará framework Spring. 
                            </p>     
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">private Configurator config;
    
public CustomService(Configurator config){
    this.config = config;
}</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>

                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Setter Dependency injection</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Obdobně jako výše uvedený příklad bude fungovat i technika setter dependency injection. V souboru beans.xml je potřeba odkázat na daný objekt pomocí speciálního atributu ref. 
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;bean id=&quot;configurator&quot; class=&quot;fit.gja.beans.Configurator&quot;&gt;
    &lt;property name=&quot;option1&quot; value=&quot;Nastaveni hodnoty 1&quot;/&gt;
    &lt;property name=&quot;option2&quot; value=&quot;Nastaveni hodnoty 2&quot;/&gt;
&lt;/bean&gt;
  
&lt;bean id=&quot;customService&quot; class=&quot;fit.gja.services.CustomService&quot;&gt;
    &lt;property name=&quot;configurator&quot; ref=&quot;configurator&quot;/&gt;
&lt;/bean&gt;
</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">    
                                Závislost poté bude přirozeně vložena přes "set" metodu <i>setConfigurator</i>.
                            </p>     
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">private Configurator config;
    
public void setConfigurator(Configurator config)
{
        this.config = config;
}</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>

                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Auto-wiring</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Auto-wiring je metoda, která umožňuje nastavit, aby Spring vyřešil některé závislosti sám bez nutnosti definice v beans.xml. Existuje několik jeho módu, my zde uvedeme mód "byName". Při jeho užití bude automaticky vyhledán parametr dle jména. 
                            </p>
                            <p class="custom-text-block">
                                V souboru beans.xml tedy můžeme vypustit definici parametru <i>configurator</i>.
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;bean id=&quot;configurator&quot; class=&quot;fit.gja.beans.Configurator&quot;&gt;
    &lt;property name=&quot;option1&quot; value=&quot;Nastaveni hodnoty 1&quot;/&gt;
    &lt;property name=&quot;option2&quot; value=&quot;Nastaveni hodnoty 2&quot;/&gt;
&lt;/bean&gt;
  
&lt;bean id=&quot;customService&quot; class=&quot;fit.gja.services.CustomService&quot; autowire=&quot;byName&quot;&gt;&lt;/bean&gt;
</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>

                        </div>
                    </div>
                </div>
            </div>

        </div>	<!--/.main-->
        
        <jsp:include page="../components/footer.jsp" />
    </body>
</html>
