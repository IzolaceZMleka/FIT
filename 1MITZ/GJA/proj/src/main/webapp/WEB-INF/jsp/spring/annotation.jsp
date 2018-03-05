<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Anotace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Anotace</li>
                </ol>
            </div><!--/.row-->

            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Spring: Anotace</h1>
                </div>
            </div><!--/.row-->

            <div clas="row">
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">Co jsou to Spring anotace?</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Anotace umožňuji provádět konfiguraci bez nutnosti zavádět další XML soubory. Mezi základní prostředky patří definice beans, nastavení dependency injection nebo správa zdrojů. 
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading"><i>@Bean anotace</i></div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Alternativní cestou definice nových beans může být vytvoření speciální třídy s metodami, které takový objekt budou poskytovat. V rámci této třídy využijeme
                                nejdříve anotaci <i>@Configuration</i>, která říká že jedná nebo více metod označených <i>@Bean</i> mohou zpracovány Spring kontejnerem a poté již nad samotnou metodou umístíme anotaci <i>@Bean</i>.</p>                   
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">package fit.gja.configuration;

import fit.gja.beans.Configurator;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanConfig {

    @Bean
    @Qualifier("configurator")
    public Configurator configurator() {
        return new Configurator("Nastaveni hodnoty 1", "Nastaveni hodnoty 2");
    }

}</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">    
                                Anotace <i>@Qualifier</i> v tomto případě slouží pro identifikaci daného objektu.
                            </p>     
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading"><i>@Autowired</i> anotace</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Techniku auto-wiring, která byla popsána v předchozí kapitole budeme simulovat pomocí anotace <i>@Autowired</i>, kterou umístíme nad požadovanou metodu (přip. konstruktor).        
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">private Configurator config;
    
@Autowired
public void setConfigurator(Configurator config) {
    this.config = config;
}
</code>
                                </pre>
                            </div>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Příklad bez souboru bean.xml</div>
                        <div class="panel-body">
                            <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>

                        </div>
                    </div>
                </div>
            </div>

        </div>	<!--/.main-->

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>
