<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Správa beans"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Správa beans</li>
                </ol>
            </div><!--/.row-->

            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Spring: Správa beans</h1>
                </div>
            </div><!--/.row-->

            <div clas="row">
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">Co jsou to beans?</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Jelikož Spring framework funguje na principu IoC kontejneru, je třeba definovat jak a jaké objekty má Spring vytvářet. Tyto objekty se nazývají beans a jejich automatická tvorba závisí na načtení konfiguračního souboru beans.xml, který obsahuje jejich definice.
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Konfigurace beans.xml</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Konfigurační soubor beans.xml na cestě resources/Beans.xml obsahuje mezi tagy beans definice dílčích tříd s hodnoty parametrů jejich konstruktorů. Tyto parametry mohou být libovolného typu. V tomto případě však pro začátek využijeme pouze jednoduché hodnoty. Příklad definice dvou tříd <i>Configurator</i> a <i>Employee</i> může být následující.</p> 
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="xml hljs">&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
&lt;beans xmlns=&quot;http://www.springframework.org/schema/beans&quot;
    xmlns:xsi=&quot;http://www.w3.org/2001/XMLSchema-instance&quot;
    xsi:schemaLocation=&quot;http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans-3.0.xsd&quot;&gt;
    
  &lt;bean id=&quot;configurator&quot; class=&quot;fit.gja.beans.Configurator&quot;&gt;
    &lt;property name=&quot;option1&quot; value=&quot;Nastaveni hodnoty 1&quot;/&gt;
    &lt;property name=&quot;option2&quot; value=&quot;Nastaveni hodnoty 2&quot;/&gt;
  &lt;/bean&gt;

&lt;/beans&gt;</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">
                                Nejjednodušší přístup k našemu vytvořenému objektu získáme pomocí instance třídy <i>ClassPathXmlApplicationContext</i>.
                            </p>     
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">ApplicationContext context = new ClassPathXmlApplicationContext("beans.xml");
        
// Nase trida Configurator 
Configurator confg = (Configurator) context.getBean("configurator");
model.addAttribute("option1", confg.getOption1());
model.addAttribute("option2", confg.getOption2());</code>
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
